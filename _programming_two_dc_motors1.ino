#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Motor pins
const int ENA = 9, IN1 = 8, IN2 = 7;
const int ENB = 10, IN3 = 6, IN4 = 5;

void setup() {
  // Set motor pins as outputs
  int pins[] = {ENA, IN1, IN2, ENB, IN3, IN4};
  for (int i = 0; i < 6; i++) pinMode(pins[i], OUTPUT);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.print("Motors Ready");
  delay(2000);
  lcd.clear();
}

//move motors with timer display
void move(const char* msg, int a1, int a2, int b1, int b2, int spd, int duration) {
  lcd.clear();
  lcd.print(msg);
  
  digitalWrite(IN1, a1);
  digitalWrite(IN2, a2);
  digitalWrite(IN3, b1);
  digitalWrite(IN4, b2);
  
  analogWrite(ENA, spd);
  analogWrite(ENB, spd);

  //timer on LCD
  for (int i = 1; i <= duration; i++) {
    lcd.setCursor(0, 1);
    lcd.print("Time: ");
    lcd.print(i);
    lcd.print(" s   ");
    delay(1000); // 1 second delay
  }
  
  // Stop after the duration
  stopMotors();
}

void stopMotors() {
  // Stop both motors
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  
  lcd.clear();
  lcd.print("Stop");
}

void loop() {
  //forward for 30 seconds
  move("Forward", HIGH, LOW, HIGH, LOW, 200, 30);
  delay(1000);

  //backward for 60 seconds
  move("Backward", LOW, HIGH, LOW, HIGH, 200, 60);
  delay(1000);

  //between right and left for a total of 60 seconds (5s each)
  for (int t = 0; t < 6; t++) {
    move("Right", LOW, HIGH, HIGH, LOW, 200, 5);
    delay(500);
    move("Left", HIGH, LOW, LOW, HIGH, 200, 5);
    delay(500);
  }

  //stop and wait
  stopMotors();
  delay(3000);
}
