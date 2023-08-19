#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
Servo servo;

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char password[8] = "1234567"; // Set your desired password here (including '\0' character)
char enteredPassword[8];
byte passwordIndex = 0;
boolean passwordEntered = false;
boolean isPasswordCorrect = false;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Enter password:");
  lcd.setCursor(2, 1);

  servo.attach(10); // Attach servo to pin 10

  kpd.setDebounceTime(10); // setDebounceTime(mS)
}

void loop() {
  char key = kpd.getKey();

  if (key && passwordEntered == false) {
    if (key == '#') {
      // Lock the door by resetting the servo position

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Door locked       ");
      delay(2000);
      clearDisplay();
    } else {
      if (passwordIndex < 7) {
        enteredPassword[passwordIndex] = key;
        passwordIndex++;
        lcd.print('*'); // Display an asterisk for each entered digit
      }

      if (passwordIndex == 7) {
        enteredPassword[passwordIndex] = '\0'; // Null-terminate the entered password
        isPasswordCorrect = comparePasswords();
        displayResult();
        delay(2000);
        clearDisplay();
        passwordEntered = true;
      }
    }
  }
}

boolean comparePasswords() {
  for (byte i = 0; i < 7; i++) {
    if (enteredPassword[i] != password[i]) {
      return false;
    }
  }
  return true;
}

void displayResult() {
  lcd.setCursor(0, 1);
  if (isPasswordCorrect) {
    lcd.print("Password correct  ");
    servo.write(45); // Rotate servo to 45 degrees (adjust as needed)
    delay(5000);
    servo.write(90); // Reset servo position
  } else {
    lcd.print("Incorrect Password");
    servo.write(90); // Reset servo position
    delay(5000);
  }
}

void clearDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter password:");
  lcd.setCursor(2, 1);
  passwordIndex = 0;
  memset(enteredPassword, 0, sizeof(enteredPassword));
  passwordEntered = false;
}