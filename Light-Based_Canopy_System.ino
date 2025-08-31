#include <LiquidCrystal.h>
#include <Servo.h>

Servo myservo;

#define LDRpin A0
#define LED1 13
#define LED2 12

const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int stepDelay = 10;
const int transitionTolerance = 20;

int light = 0;
int minLight = 1023, maxLight = 0;
int state = -1;
bool initialized = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  myservo.attach(9);
  delay(100);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  Serial.println("System Initializing...");
  delay(1000);
}

void loop() {
  // Read and track LDR light value
  light = analogRead(LDRpin);
  if (light < minLight) minLight = light;
  if (light > maxLight) maxLight = light;

  int midpoint = (minLight + maxLight) / 2;

  int newState;
  if (light > midpoint + transitionTolerance) newState = 1;
  else if (light < midpoint - transitionTolerance) newState = 0;
  else newState = state;

  // Print to Serial Monitor (live data)
  Serial.print("LDR: ");
  Serial.print(light);
  Serial.print(" | Min: ");
  Serial.print(minLight);
  Serial.print(" | Max: ");
  Serial.print(maxLight);
  Serial.print(" | Midpoint: ");
  Serial.print(midpoint);
  Serial.print(" | State: ");
  Serial.print((state == 1) ? "Open" : (state == 0) ? "Close" : "Unknown");
  Serial.print(" | Servo Angle: ");
  Serial.println(myservo.read());

  if (!initialized || newState != state) {
    state = newState;
    initialized = true;

    if (state == 1) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Status: Opening...");
      Serial.println("Action: Opening Canopy");
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      moveServoSmoothly(myservo.read(), 175);
      lcd.setCursor(0, 0);
      lcd.print("Status: Open     ");
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Status: Closing...");
      Serial.println("Action: Closing Canopy");
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      moveServoSmoothly(myservo.read(), 0);
      lcd.setCursor(0, 0);
      lcd.print("Status: Close    ");
    }

    lcd.setCursor(0, 1);
    lcd.print("Angle: ");
    lcd.print(myservo.read());
    lcd.print(" deg     ");
  }

  delay(200);
}

void moveServoSmoothly(int currentAngle, int targetAngle) {
  if (currentAngle < targetAngl[e) {
    for (int pos = currentAngle; pos <= targetAngle; pos++) {
      myservo.write(pos);
      delay(stepDelay);
    }
  } else {
    for (int pos = currentAngle; pos >= targetAngle; pos--) {
      myservo.write(pos);
      delay(stepDelay);
    }
  }
}
