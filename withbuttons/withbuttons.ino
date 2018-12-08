#include <Keypad.h>

//keypad
const byte ROWS = 4;
const byte COLS = 4;
//byte rowPinsOut[ROWS] = {D1, D2, D3, D4};
//byte colPinsIn[COLS] = {D5, D6, D7, D8};
byte rowPinsOut[2] = {D1, D2};
byte colPinsIn[2] = {D5, D6};
char keysShort[2][2] = {{'1','2'}, {'5','6'}};
char keys[ROWS][COLS] = {
  {'1','2','3','4'},
  {'5','6','7','8'},
  {'9','A','B','C'},
  {'D','E','F','G'}
};
//Keypad myKeypad = Keypad( makeKeymap(keys), rowPinsOut, colPinsIn, ROWS, COLS);
Keypad myKeypad = Keypad( makeKeymap(keysShort), rowPinsOut, colPinsIn, 2, 2);

//motor
int enable2 = D4;
int in3 = D7;
int in4 = D8;

int rotate = 0;

char lastKey = NO_KEY;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(enable2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  myKeypad.setDebounceTime(10);
  myKeypad.setHoldTime(10);

  motorStop();

  Serial.begin(115200);

  blinkInternal();
}

// the loop function runs over and over again forever
void loop() {

  //TODO:
  //act itself, if no commands in 30 seconds
  //3-5 random behaviours, every 10s
  // - read book
  // - slowly rotate 180 and back
  // - look back
  // - laggy rotate

  char myKey = myKeypad.getKey();
  int myState = myKeypad.getState();

  if (myState == HOLD) {
    if (lastKey == '5') {
       motorSpinLeftSlowly();
    } else if (lastKey == '6') {
      motorSpinRightSlowly();
    }
  } else if (myKey != NO_KEY){
    Serial.print("Key pressed: ");
    Serial.println(myKey);

    lastKey = myKey;

    if (myKey == '1') {
      blinkInternal();
      motorSayNo();
    } else if (myKey == '2') {
      motorDoTrick();
    } /*else if (myKey == '5') {
       motorSpinLeftSlowly();
    } else if (myKey == '6') {
      motorSpinRightSlowly();
    } */
  } else{
    motorStop();
  }


}

void motorSayNo() {

  analogWrite(enable2, 2500);
  for (int i = 0; i < 3; i++) {
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    delay(300);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    delay(300);
  }
  motorStop();

}

void motorDoTrick() {

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  motorVerySlowly(160);
  analogWrite(enable2, 2500);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(700);
  motorStop();

}

//80 ~3s 1turn
void motorVerySlowly(int iters) {
  for (int i = 0; i < iters; i++) {
    analogWrite(enable2, 1000);
    delay(10);
    analogWrite(enable2, 0);
    delay(15);
  }
}

void motorSpinLeftSlowly() {

  //analogWrite(enable2, 1000);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  //delay(100);
  motorVerySlowly(5);
  //motorStop();
}

void motorSpinRightSlowly() {

  //analogWrite(enable2, 1000);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  motorVerySlowly(5);
  //delay(100);
  //motorStop();
}

//power should be 1000-2500 (for 5v power and yellow motor)
void motorDo(int power) {

  analogWrite(enable2, power);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(3000);
  analogWrite(enable2, power);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(3000);
  motorStop();
}

void motorStop() {
  analogWrite(enable2, 0);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void blinkInternal() {
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
}

void blinkInternalSlow() {
  digitalWrite(LED_BUILTIN, LOW);
  delay(2000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);
}