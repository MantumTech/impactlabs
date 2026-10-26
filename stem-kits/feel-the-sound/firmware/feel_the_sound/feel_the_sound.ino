// Feel the Sound — MVP firmware (Arduino Nano)
// Louder sound -> stronger vibration. The knob on A1 sets sensitivity.
// Wiring: mic OUT -> A0, pot wiper -> A1, D9 -> 1k -> NPN base, motor between 5V and collector,
// diode across the motor (stripe to 5V). See ../../PLAN.md.

const int MIC_PIN   = A0;
const int KNOB_PIN  = A1;
const int MOTOR_PIN = 9;               // PWM pin

const unsigned long WINDOW_MS = 50;    // listen this long per reading
const int MAX_PWM   = 150;             // ~60% of 255: keeps a 3 V motor at ~3 V average on 5 V
const int LOUD_SPAN = 400;             // peak-to-peak above threshold that counts as "full buzz"

int level = 0;                         // smoothed motor strength

void setup() {
  pinMode(MOTOR_PIN, OUTPUT);
  Serial.begin(9600);                  // open Serial Plotter to watch the numbers
}

void loop() {
  // 1. Measure loudness: biggest minus smallest sample in the window.
  int lo = 1023, hi = 0;
  unsigned long start = millis();
  while (millis() - start < WINDOW_MS) {
    int s = analogRead(MIC_PIN);
    if (s < lo) lo = s;
    if (s > hi) hi = s;
  }
  int loudness = hi - lo;

  // 2. Knob sets the threshold: turn it up to ignore more background noise.
  int threshold = map(analogRead(KNOB_PIN), 0, 1023, 10, 500);

  // 3. Map loudness above the threshold to motor strength.
  int target = 0;
  if (loudness > threshold) {
    target = map(constrain(loudness - threshold, 0, LOUD_SPAN), 0, LOUD_SPAN, 60, MAX_PWM);
  }

  // 4. Smooth: react fast to new sounds, fade out gently.
  level = (target > level) ? target : (level * 3 + target) / 4;
  analogWrite(MOTOR_PIN, level);

  Serial.print("loudness:");  Serial.print(loudness);
  Serial.print(" threshold:"); Serial.print(threshold);
  Serial.print(" motor:");     Serial.println(level);
}
