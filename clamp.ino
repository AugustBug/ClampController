// @author Ahmert
// @date 29.12.2018
// Clamp Controller

#include <Servo.h>

Servo servoHorizontal;
Servo servoVertical;
Servo servoClamp;

int posY = 0;
int posX = 0;
int posK = 0;

const int buffSize = 20;
char buff[buffSize] = {0};
int bufCount = 0;
bool started = false;

void setup() {
  Serial.begin(115200);
  servoHorizontal.attach(11);
  delay(50);
  servoVertical.attach(9);
  delay(50);
  servoClamp.attach(10);
  delay(50);
}

void loop() {
  parseSerial();
  delay(15);
}

void parseSerial() {
  while (Serial.available()) {
    char c = Serial.read();
    if(c == '$') {
      // started
      started = true;
      bufCount = 0;
    } else if(c == '#') {
      // finished
      if(started) {
        if(bufCount == 4) {
          int pos = (buff[1]-'0') * 100 + (buff[2]-'0') * 10 + (buff[3]-'0');
          if(pos<0) {
            pos = 0;
          } else if(pos>220) {
            pos = 220;
          }
          if(buff[0] == '1') {
            servoHorizontal.write(pos);
          } else if(buff[0] == '2') {
            servoVertical.write(pos);
          } else if(buff[0] == '3') {
            servoClamp.write(pos);
          }
          delay(5);
        }
        started = false;
      }
    } else if((bufCount < buffSize) && started) {
      buff[bufCount++] = c;
    }
  }
}

