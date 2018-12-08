#include <Keypad.h>
#include <elapsedMillis.h>

//keypad
const byte ROWS = 4;
const byte COLS = 4;
//byte rowPinsOut[ROWS] = {D1, D2, D3, D4};
//byte colPinsIn[COLS] = {D5, D6, D7, D8};
byte rowPinsOut[2] = {D1, D2};
byte colPinsIn[2] = {D3, D4};
char keysShort[2][2] = {{'2', '6'}, {'1', '5'}};
char keys[ROWS][COLS] = {
  {'1', '2', '3', '4'},
  {'5', '6', '7', '8'},
  {'9', 'A', 'B', 'C'},
  {'D', 'E', 'F', 'G'}
};
//Keypad myKeypad = Keypad( makeKeymap(keys), rowPinsOut, colPinsIn, ROWS, COLS);
Keypad myKeypad = Keypad( makeKeymap(keysShort), rowPinsOut, colPinsIn, 2, 2);

//ultrasonic
int sonarTrig = D8;
int sonarEcho = D0;

//motor
int mEnable = D5;
int mL = D6;
int mR = D7;
int rotate = 0;

char lastKey = NO_KEY;

int ideleDelay = 60*1000; // 1mi in ms
elapsedMillis timeElapsed; //declare global if you don't want it reset every time loop runs

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(sonarTrig, OUTPUT); // Sets the trigPin as an Output
  pinMode(sonarEcho, INPUT); // Sets the echoPin as an Input

  pinMode(mEnable, OUTPUT);
  pinMode(mL, OUTPUT);
  pinMode(mR, OUTPUT);

  myKeypad.setDebounceTime(10);
  myKeypad.setHoldTime(10);

  motorStop();

  randomSeed(analogRead(A0));

  digitalWrite(sonarTrig, LOW);
  delayMicroseconds(2);

  blinkInternal();

  Serial.begin(115200);
  Serial.println("STARTED");

}

// the loop function runs over and over again forever
void loop() {

  Serial.println("debug 1: "+timeElapsed);

  char myKey = myKeypad.getKey();
  int myState = myKeypad.getState();

  if (myKey){
    //Serial.print(myKey);
    //Serial.print(" - ");
    //Serial.println(myState);
  }

  if( myState == HOLD) {
    //reset activity
    timeElapsed = 0;

    if (lastKey == '5') {
      motorSpinLeftSlowly();
    } else if (lastKey == '6') {
      motorSpinRightSlowly();
    }
  } else if (myKey != NO_KEY) {
    //reset activity
    timeElapsed = 0;

    //set last kye for HOLDd
    lastKey = myKey;

    if (myKey == '1') {
      blinkInternal();
      motorSayNo();
    } else if (myKey == '2') {
      //readBook();
      //turnSideSlowlyWithLag();
      motorDoTrick();
    }
  } else {
    motorStop();
  }

  Serial.println("debug 2: "+timeElapsed);

  //check sonar if exist
  int distance = getDistance();
  if (distance>5) {
    //use distance instead of random action
    timeElapsed = 0;

    if (distance<10) {
      motorSpinRightSlowly();
    } else if (distance<15) {
      motorSpinLeftSlowly();
    } else if (distance<20) {
      motorSpinLeftFast();
    } else if (distance<25) {
      motorSpinRightFast();
    } else if (distance<150) {
      //do nothing
    } else if (distance<200) {
      motorSayNo();
    } /*else if (distance<200) {
      motorDoTrick();
    } */
  }

  //random action if idle for long (~1 minute?)
  Serial.println(timeElapsed/1000);
  if ( timeElapsed > ideleDelay) {

    // 0-4
    switch (random(5)) {
      case 1:
        readBook();
        break;
      case 2:
        motorDoTrick();
        break;
      case 3:
        turnSideSlowlyWithLag();
        break;
      case 4:
        // nothing
        break;
      default:
        // nothing, just idle, e.g. if 0
        break;
    }

    //decrese last active
    timeElapsed = ideleDelay*0.75; //reduce delay by 1/4
  }

}

void motorSayNo() {

  analogWrite(mEnable, 2500);
  for (int i = 0; i < 3; i++) {
    digitalWrite(mL, HIGH);
    digitalWrite(mR, LOW);
    delay(300);
    digitalWrite(mL, LOW);
    digitalWrite(mR, HIGH);
    delay(300);
  }
  motorStop();

}

void motorDoTrick() {

  digitalWrite(mL, HIGH);
  digitalWrite(mR, LOW);

  motorVerySlowly(160);
  analogWrite(mEnable, 2500);
  digitalWrite(mL, LOW);
  digitalWrite(mR, HIGH);
  delay(750);
  motorStop();

}

//80 ~3s 1turn
void motorVerySlowly(int iters) {
  for (int i = 0; i < iters; i++) {
    analogWrite(mEnable, 1000);
    delay(15);
    analogWrite(mEnable, 0);
    delay(15);
  }
}

void motorSpinLeftSlowly() {

  //analogWrite(mEnable, 1000);
  digitalWrite(mL, LOW);
  digitalWrite(mR, HIGH);
  //delay(100);
  motorVerySlowly(5);
  motorStop();
}

void motorSpinRightSlowly() {

  //analogWrite(mEnable, 1000);
  digitalWrite(mL, HIGH);
  digitalWrite(mR, LOW);
  motorVerySlowly(5);
  //delay(100);
  motorStop();
}

void motorFast(int iters) {
  for (int i = 0; i < iters; i++) {
    analogWrite(mEnable, 1000);
    delay(15);
  }
}

void motorSpinLeftFast() {
  digitalWrite(mL, LOW);
  digitalWrite(mR, HIGH);
  motorFast(5);
  motorStop();
}

void motorSpinRightFast() {
  digitalWrite(mL, HIGH);
  digitalWrite(mR, LOW);
  motorFast(5);
  motorStop();
}

//power should be 1000-2500 (for 5v power and yellow motor)
void motorDo(int power) {

  analogWrite(mEnable, power);
  digitalWrite(mL, HIGH);
  digitalWrite(mR, LOW);
  delay(3000);
  analogWrite(mEnable, power);
  digitalWrite(mL, LOW);
  digitalWrite(mR, HIGH);
  delay(3000);
  motorStop();
}

void motorStop() {
  analogWrite(mEnable, 0);
  digitalWrite(mL, LOW);
  digitalWrite(mR, LOW);
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

/**
 * Random actions.
 * - read book
 * - slowly rotate 180 and back
 * - look back
 * - laggy rotate
 */
 void readBook() {

  //5 rows
  for (int i = 0; i < 5; i++) {
    //one side
    digitalWrite(mL, LOW);
    digitalWrite(mR, HIGH);
    motorVerySlowly(15);
    //back
    digitalWrite(mL, HIGH);
    digitalWrite(mR, LOW);
    analogWrite(mEnable, 2500);
    delay(290);
  }
  motorStop();
}

void turnSideSlowlyWithLag() {
  digitalWrite(mL, LOW);
  digitalWrite(mR, HIGH);

  for (int i = 0; i < 7; i++) {
    motorVerySlowly(5 );
    delay(500);
  }

  motorStop();
}

int getDistance() {
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(sonarTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(sonarTrig, LOW);
  int duration = pulseIn(sonarEcho, HIGH);
  int distance= duration*0.034/2;
  Serial.println((String)"Distance: " + distance + ", duration: " + duration);
  return distance;
}





