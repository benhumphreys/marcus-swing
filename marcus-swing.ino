#define DEBUG

// The pin that buzzer is connected to
int SPEAKER_PIN = 2;

int ACCEL_Z_PIN = A0;

// 3 (+/-3g) for ADXL337
int ACCEL_SCALE = 3;

// Value from the accelerometer when the swing is in the rest position
float REST_VALUE = 0.05;

// Value from the accelerometer for swing count triggering
float TRIGGER_VALUE = -0.25;

// When the swing has returned to the rest position this is set true; then when it passes the trigger value
// it is set to false. The swing counter isn't ever re-incremented until the swing returns to the rest position
bool atRest = true;

int swingCount = 0;

void setup() {
  // Set the output pin for the speaker
  pinMode(SPEAKER_PIN, OUTPUT);

#ifdef DEBUG
  // Initialize serial communication at 115200 baud
  Serial.begin(115200);
#endif
}

void loop() {
  while (true) {
    float z = getScaledZ();
    
#ifdef DEBUG
    Serial.print("Z: "); Serial.print(z); Serial.print(" g"); Serial.print("  -  atRest: "); Serial.println(atRest);
#endif

    if (atRest) {
      if (z < TRIGGER_VALUE) {
        increment();
        atRest = false;
      }
    } else {
      if (z > REST_VALUE) {
        atRest = true;
      }
    }

    delay(100);
  }
}

float getScaledZ() {
  int rawZ = analogRead(ACCEL_Z_PIN);
  return mapf(rawZ, 0, 675, -ACCEL_SCALE, ACCEL_SCALE);
}

void beep() {
  // pin, freq in Hz, duration in ms
  tone(SPEAKER_PIN, 300, 50);
}

void increment() {
  swingCount++;
  beep();
}

// Same functionality as Arduino's standard map function, except using floats
float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
