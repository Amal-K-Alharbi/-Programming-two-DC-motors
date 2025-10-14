# Programming two DC motors
[simulation](https://www.tinkercad.com/things/7LKJLGIL0C9-programming-two-dc-motors)
### - Program two DC motors using the L298N motor driver to perform the following movements:
   1. Forward for 30 seconds
   2. Backward for 1 minute
   3. Alternate left and right for 1 minute
       
### - After programming the motors, connect the Arduino to the LCD so that the name of the movement appears on the screen in conjunction with the motor movement. Use one of the communication protocols.

for that we need
| Name  | Quantity | Component |
|-------|-----------|------------|
| M1 M4 | 2 | DC Motor |
| U1 | 1 | H-bridge Motor Driver |
| U2 | 1 | Arduino Uno R3 |
| U4 | 1 | PCF8574-based, 39 (0x27) LCD 16 x 2 (I2C) |
| Bat4 | 1 | 4 batteries, AA, no 1.5V Battery |

<ins> There are other ways but this is what worked for me.</ins>

![Design](Programming_two_DC_motors.png)

Four AA batteries (1.5V each) are used to provide sufficient power (6V) for the DC motors.

The batteries are connected to the breadboard power rails, where the red wire is positive (+) and the black wire is ground (−).

The L293D (I did not find L298N ) chip is placed in the middle of the breadboard and acts as an H-Bridge motor driver to control two DC motors.

Control pins from the Arduino are connected to the input pins of the L293D to control motor direction and speed.

Each motor is connected to one side of the chip outputs.

The Arduino Uno sends control signals to the L293D through its digital (PWM) pins.

An LCD display (with I2C interface) is connected to the Arduino using SDA and SCL lines to show information such as motor status or direction.

Wire color meanings:
Red = Power (VCC)
Black = Ground (GND)
Yellow/Green = Control signals between Arduino, L293D, and LCD

The purpose of this setup is to control the rotation, direction, and speed of two DC motors using Arduino while displaying relevant information on the LCD.
```
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
```
