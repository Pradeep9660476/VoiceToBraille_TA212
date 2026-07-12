# VoiceToBraille

Convert **spoken words into physical braille** using a single CAM-follower servo motor.

VoiceToBraille is an end-to-end pipeline that listens to speech, transcribes it, encodes each character into braille dot patterns, translates those patterns into servo rotation commands, and finally drives an Arduino-controlled servo to physically emboss/actuate the braille cells one column at a time.

```
🎤 Voice  →  📝 Text  →  ⠿ Braille encoding  →  🔄 Servo angles  →  🤖 Arduino motor
```

---

## How it works

The system is a four-stage pipeline. Each stage feeds the next.

| Stage | File | Language | Role |
|-------|------|----------|------|
| 1. Speech → Text | `voiceToTextmain.py` | Python | Captures microphone audio and transcribes it via the Google Speech Recognition API |
| 2. Text → Braille | `textTobraille.py` | Python | Maps each character to a 6-dot braille pattern and reshapes it into printable columns |
| 3. Braille → Angles | `brailleToangle.cpp` | C++ | Converts the column encodings into a sequence of servo rotation angles |
| 4. Angles → Motion | `arduino.ino` | Arduino C++ | Drives the servo, rotating it by each angle to print the braille physically |

---

## The mechanism

This project uses a **single CAM-follower servo** rather than six separate solenoids. A braille cell is a 3×2 grid of dots:

```
dot1  dot4
dot2  dot5
dot3  dot6
```

The printer works **one vertical column at a time** (three dots per column, two columns per character). The cam follower can be rotated into one of six logical positions, each representing which dot(s) in the current column should be raised:

| Position code | Meaning (dots raised in the column) |
|---------------|--------------------------------------|
| `1` | top dot |
| `2` | middle dot |
| `3` | bottom dot |
| `12` | top + middle |
| `13` | top + bottom |
| `23` | middle + bottom |

Moving between positions requires rotating the servo by a fixed angle. The C++ stage computes these:

- **±60°** — adjacent position shifts
- **±120°** — larger position shifts
- **Positive** = clockwise, **Negative** = counter-clockwise
- **`500`** — a special sentinel meaning *"advance the carriage / feed to the next column"* (no dot rotation)
- **`-1`** — an impossible/invalid transition (should never occur in practice; kept as a safety guard)

The Arduino reads this angle array, and for each entry either skips (`500` / `-1`) or rotates the continuous-rotation servo clockwise/counter-clockwise for a time proportional to the angle, then stops.

---

## Requirements

### Python stages
- Python 3.x
- [`SpeechRecognition`](https://pypi.org/project/SpeechRecognition/)
- [`PyAudio`](https://pypi.org/project/PyAudio/) (microphone access)
- `numpy`
- An active internet connection (the Google Speech API is a web service)

```bash
pip install SpeechRecognition PyAudio numpy
```

### C++ stage
- Any C++11-compatible compiler (g++, clang, MSVC)

```bash
g++ brailleToangle.cpp -o brailleToangle
```

*(A prebuilt `brailleToangle.exe` is included for Windows.)*

### Hardware stage
- Arduino (Uno or similar)
- A servo motor wired to **pin 9** (continuous-rotation servo, cam-follower assembly)
- The Arduino `Servo` library (bundled with the Arduino IDE)

---

## Usage

Run the stages in order.

**1. Capture and transcribe speech**
```bash
python voiceToTextmain.py
```
Speak when prompted. The recognized text is printed to the console.

**2. Convert text to braille encodings**
```bash
python textTobraille.py
```
Enter the transcribed text. The script prints the braille array and its reshaped, cleaned-up column form (3 rows × columns).

**3. Convert encodings to servo angles**
Feed the column encodings (as `"110"`, `"010"`, … strings) into the `data` vector in `brailleToangle.cpp`, then:
```bash
./brailleToangle
```
It prints the ordered list of angles (including `500` feed commands).

**4. Print on hardware**
Paste the resulting angle array into the `angles[]` list in `arduino.ino`, upload to the board, and open the Serial Monitor (9600 baud) to watch each step execute.

> The four stages are currently connected **manually** — output from one is copied into the next. See [Roadmap](#roadmap) for planned automation.

---

## Character support

`textTobraille.py` currently maps:

- **Letters** `a`–`z` (case-insensitive)
- **Digits** `0`–`9`
- **Space** (used as a word separator)

Braille cells are separated between words using a `[-1, -1, -1, -1, -1, -1]` marker, and the reshaping logic collapses redundant separator/space columns so the physical output stays compact. Unknown characters are skipped with a warning.

---

## Project structure

```
VoiceToBraille/
├── voiceToTextmain.py     # Stage 1: microphone → text (Google Speech API)
├── textTobraille.py       # Stage 2: text → braille dot encodings
├── brailleToangle.cpp     # Stage 3: encodings → servo angle sequence
├── brailleToangle.exe     # Prebuilt Windows binary of Stage 3
├── arduino.ino            # Stage 4: angle sequence → servo motion
└── README.md
```

---

## Calibration notes

The Arduino sketch assumes a servo speed of roughly **520 ms per 360°** (`ms_per_degree = 520.0 / 360.0`). Because this drives a continuous-rotation servo by *timing* rather than by absolute position, you will likely need to tune this constant for your specific motor and load to get accurate dot placement. A 3-second pause is inserted between steps for observation and mechanical settling.

---

## Roadmap

Potential improvements for future versions:

- **Automate the pipeline** so the four stages pass data directly instead of manual copy-paste (e.g. Python calling the C++ binary and streaming angles to the Arduino over serial).
- **Closed-loop positioning** — replace timing-based servo control with an encoder or a positional servo for reliable dot placement.
- **Expand character support** to include punctuation and contractions (Grade 2 braille).
- **Live streaming mode** — transcribe and print continuously as the user speaks.

---

## License

Released under the [MIT License](LICENSE). See the `LICENSE` file for details.
