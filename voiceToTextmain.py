import speech_recognition as sr

# Initialize the recognizer
recognizer = sr.Recognizer()

with sr.Microphone() as source:
    print("Speak something...")
    recognizer.adjust_for_ambient_noise(source)    # optional
    audio = recognizer.listen(source)

try:
    print("Recognizing...")
    text = recognizer.recognize_google(audio)
    print("You said:", text)

except sr.UnknownValueError:
    print("Sorry, I could not understand the audio.")
except sr.RequestError:
    print("Could not request results; check your internet connection.")
