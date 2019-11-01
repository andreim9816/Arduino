
/*
 * BEC RGB cu potentiometre
 */

const int pinRed=11;
const int potPinRed=A0;

const int pinGreen=10 ;
const int potPinGreen=A1;

const int pinBlue=9;
const int potPinBlue= A2;

int readPotValueRed;
int readPotValueGreen;
int readPotValueBlue;

int writeValueRed;
int writeValueGreen;
int writeValueBlue;

void setup() {

  pinMode(potPinBlue,INPUT);
  pinMode(pinBlue,OUTPUT);

  pinMode(potPinRed,INPUT);
  pinMode(pinRed,OUTPUT);

  pinMode(potPinGreen,INPUT);
  pinMode(pinGreen,OUTPUT);
}

void loop() {

readPotValueRed=analogRead(potPinRed);
readPotValueGreen=analogRead(potPinGreen);
readPotValueBlue=analogRead(potPinBlue);

writeValueRed=map(readPotValueRed,0,1023,0,255);
writeValueBlue=map(readPotValueBlue,0,1023,0,255);
writeValueGreen=map(readPotValueGreen,0,1023,0,255);

analogWrite(pinRed,writeValueRed);
analogWrite(pinGreen,writeValueGreen);
analogWrite(pinBlue,writeValueBlue);

}
