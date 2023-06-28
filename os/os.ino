#include<LiquidCrystal_I2C_Hangul.h>
#include <EEPROM.h>
#include<Wire.h>

LiquidCrystal_I2C_Hangul lcd(0x27,16,2); //LCD 클래스 초기화

int delayTime = 250;

int joyXPin = A0;
int joyYPin = A1;
int joyButtonPin = 2;
bool joyChange = false;

char* funcarr[] = {"home", "game", "info", "setting", "license"};

bool otherDoing = false;

int gameIdx = 0;
bool attack = false;
int lastX1 = 0;
int lastX2 = 0;
int lastX3 = 0;
int lastY = 0;
int point = 0;

int x = 1;
int y = 1;

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  pinMode(joyButtonPin, INPUT_PULLUP);
}

void loop() {
  int joyXValue = analogRead(joyXPin);
  int joyYValue = analogRead(joyYPin);
  int joyButtonValue = !digitalRead(joyButtonPin);

  if (joyChange == false) {
    if (joyXValue == 0) {
      joyChange = true;
      x -= 1;
    } else if (joyXValue > 670) {
      joyChange = true;
      x += 1;
    }
    
    if (joyYValue == 0) {
      joyChange = true;
      y -= 1;
    } else if (joyYValue > 670) {
      joyChange = true;
      y += 1;
    }
  }

  lcd.clear();

  if (otherDoing == false) {
    if (funcarr[x - 1] == "home") {
      lcd.setCursor(0, 0);
      lcd.print("Home");

      lcd.setCursor(0, 1);
      lcd.print("- Hello, world!");
    }

    if (funcarr[x - 1] == "game") {
      lcd.setCursor(0, 0);
      lcd.print("Simple INO Game");

      lcd.setCursor(0, 1);
      lcd.print("Click Joystick");

      if (joyButtonValue == 1) {
        x = 4;
        y = 0;
        otherDoing = true;
      }
    }

    if (funcarr[x - 1] == "info") {
      lcd.setCursor(0, 0);
      lcd.print("My Point: " + String(EEPROM.read(0)));
    }
    
    if (funcarr[x - 1] == "setting") {
      lcd.setCursor(0, 0);
      lcd.print("x: " + String(joyXValue) + ", y: " + String(joyYValue));
      
      lcd.setCursor(0, 1);
      lcd.print("value: " + String(joyButtonValue));
    }
    
    if (funcarr[x - 1] == "license") {
      lcd.setCursor(0, 0);
      lcd.print("MIT License 2023");
      
      lcd.setCursor(0, 1);
      lcd.print("ICe1BotMaker");
    }
    
    if (x - 1 == sizeof(funcarr) / sizeof(int) || x - 1 < 0) {
      lcd.setCursor(0, 0);
      lcd.print("404 no function");

      lcd.setCursor(0, 1);
      lcd.print("position: " + String(x - 1));
    }
  } else {
    if (x == -1) {
      x = 0;
    } else if (x == 16) {
      x = 15;
    }

    if (y == -1) {
      y = 0;
    } else if (y == 2) {
      y = 1;
    }

    if (gameIdx == 4) {
      gameIdx = 0;

      if (attack == true) {
        lcd.setCursor(0, lastY);
        lcd.print("################");

        lcd.setCursor(lastX1, lastY == 1 ? 0 : 1);
        lcd.print("#");
        lcd.setCursor(lastX2, lastY == 1 ? 0 : 1);
        lcd.print("#");
        lcd.setCursor(lastX3, lastY == 1 ? 0 : 1);
        lcd.print("#");

        if (lastY == y || lastX1 == x || lastX2 == x || lastX3 == x) {
          x = 1;
          y = 0;
          EEPROM.write(0, EEPROM.read(0) + point);
          point = 0;
          delayTime = 250;
          otherDoing = false;
        }

        delayTime -= 5;
        point += 1;

        attack = false;
      } else {
        lastX1 = x;
        lastX2 = x - random(1, 5);
        lastX3 = x + random(1, 5);
        lastY = y;

        lcd.setCursor(0, lastY);
        lcd.print("------Warning---");

        lcd.setCursor(lastX1, lastY == 1 ? 0 : 1);
        lcd.print("|");
        lcd.setCursor(lastX2, lastY == 1 ? 0 : 1);
        lcd.print("|");
        lcd.setCursor(lastX3, lastY == 1 ? 0 : 1);
        lcd.print("|");

        attack = true;
      }
    }

    lcd.setCursor(x, y);
    lcd.print("@");

    lcd.setCursor(14, 1);
    lcd.print(String(point));
  }

  if (otherDoing == true) {
    gameIdx += 1;
  }
  
  delay(delayTime);

  joyChange = false;
}