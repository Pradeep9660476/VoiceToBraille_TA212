#include <Servo.h>

Servo myservo;

// Input angle sequence
int angles[] = {60, 500, -60, 120, -60, 500, 60, -60, 500, -60, 60, 500, 
                60, -60, 500, -60, 120, 120, 60, 500, -1, 500, 60, 120, 
                120, 60, 500, -1, 500, 60, -120, -60};

int n = sizeof(angles) / sizeof(angles[0]);

// Servo speed calibration: assume 1000ms = 360° rotation
float ms_per_degree = 520.0 / 360.0;

int current_position = 0;  // track logical current angle

void setup() {
  Serial.begin(9600);
  myservo.attach(9);
  myservo.write(90); // stop servo initially
  delay(1000);

  Serial.println("=== Servo Rotation Sequence Started ===");
  Serial.print("Initial Position: ");
  Serial.println(current_position);
  Serial.println("--------------------------------------");
}

void loop() {
  for (int i = 0; i < n; i++) {
    int delta = angles[i];

    Serial.print("Step ");
    Serial.print(i);
    Serial.print(": Command = ");
    Serial.println(delta);

    // Skip if command = 500 or -1
    if (delta == 500 || delta == -1) {
      Serial.println("  → Skip (no movement)");
      Serial.println("--------------------------------------");
      delay(3000); // 3s pause even for skipped steps
      continue;
    }

    int target_position = current_position + delta;

    // Determine direction and rotate
    if (delta > 0) {
      Serial.print("  → Rotating +");
      Serial.print(delta);
      Serial.println(" degrees (CW)");
      myservo.write(180); // rotate clockwise
    } else {
      Serial.print("  → Rotating ");
      Serial.print(delta);
      Serial.println(" degrees (CCW)");
      myservo.write(0);   // rotate counterclockwise
    }

    // Estimate movement duration
    int move_time = abs(delta) * ms_per_degree;
    Serial.print("  Estimated move time (ms): ");
    Serial.println(move_time);

    // Rotate for calculated time
    delay(move_time);

    // Stop servo
    myservo.write(90);
    Serial.println("  → Stopped");

    // Update logical position
    current_position = target_position;
    Serial.print("  New Position: ");
    Serial.println(current_position);
    Serial.println("--------------------------------------");

    // Wait 3 seconds before next step
    delay(3000);
  }

  Serial.println("=== Sequence Completed ===");
  while (1); // stop program after one full run
}




// final working code(takes angle array as input and then gives servo+stepper commands)