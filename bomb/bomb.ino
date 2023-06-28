#include<LiquidCrystal_I2C_Hangul.h>
#include<Wire.h>

LiquidCrystal_I2C_Hangul lcd(0x27,16,2); //LCD 클래스 초기화
int buzzerPin = 9;
int buzzerIdx = 4;

int idx = 0;
int time = 90;

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  if (idx < 100) {
    lcd.setCursor(0, 0);
    lcd.print("Installing...");

    lcd.setCursor(0, 1);
    lcd.print("Progress: " + String(idx) + "%");

    idx += 1;

    delay(10);
  } else if (idx == 100 && time != -1) {
    lcd.setCursor(0, 0);
    lcd.print("Bomb Installed.");

    lcd.setCursor(0, 1);
    lcd.print(String(time) + " seconds left          ");

    time -= 1;

    if (time < 10) {
      tone(buzzerPin, 493.88);
    } else {
      if ((time + 1) % 2 == 0) {
        tone(buzzerPin, 440);
      }
    }

    delay(50);
    noTone(buzzerPin);

    delay(950);
  }

  if (time == -1) {
    lcd.setCursor(0, 1);
    lcd.print("Over 60 seconds          ");

    if (buzzerIdx != 0) {
      tone(buzzerPin, 523.25);
      delay(500);
      noTone(buzzerPin);

      buzzerIdx -= 1;
    }
  }
}