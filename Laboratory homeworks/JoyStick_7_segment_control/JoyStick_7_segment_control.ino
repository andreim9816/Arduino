const int pinA = 12;
const int pinB = 8;
const int pinC = 5;
const int pinD = 3;
const int pinE = 2;
const int pinF = 11;
const int pinG = 6;
const int pinDP = 4;
const int pinD1 = 7;
const int pinD2 = 9;
const int pinD3 = 10;
const int pinD4 = 13;
const int pinX = A0;
const int pinY = A1;
const int pinSW = A2;

const int segSize = 8;
const int digitsSize = 4;
const int noOfDisplays = 4;
const int noOfDigits = 10;


int cifra[digitsSize] = {0,0,0,0};

int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

// digits array, to switch between them easily
int digits[noOfDisplays] = {
  pinD4, pinD3, pinD2, pinD1
 };   
 
byte digitMatrix[noOfDigits][segSize - 1] = {
// a  b  c  d  e  f  g
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};


void displayNumber(byte digit, byte decimalPoint) 
{
  for (int i = 0; i < segSize - 1; i++) 
    digitalWrite(segments[i], digitMatrix[digit][i]);

  // write the decimalPoint to DP pin
  digitalWrite(segments[segSize - 1], decimalPoint);
}

// activate the display no. received as param
void showDigit(int num)
{
  for (int i = 0; i < noOfDisplays; i++) 
  digitalWrite(digits[i], HIGH);
 
  digitalWrite(digits[num], LOW);

}

void setup() {
  for (int i = 0; i < segSize; i++)
   pinMode(segments[i], OUTPUT);  

  for (int i = 0; i < noOfDisplays; i++)
   pinMode(digits[i], OUTPUT);  

  pinMode(pinSW, INPUT_PULLUP);
  Serial.begin(9600);
}

int xValue = 0;
int yValue = 0;
int currentPosition = 0;

int dpState = LOW;
int swState = LOW;
int lastSwState = LOW;

bool joyMovedX = false; 
bool joyMovedY = false;

int minValue = 100;
int maxValue = 900;

void loop()
{
  int i;
 
  swState = digitalRead(pinSW);
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  if(swState != lastSwState)
    if(swState == LOW) // if the joystick Button is pressed 
      dpState = !dpState;

  lastSwState = swState;
  
  // afiseaza toate cele 4 displayuri
  for(i = 0; i <currentPosition; i++) 
    {
      showDigit(i); // aprinde display-ul i 
      displayNumber(cifra[i],HIGH); // afiseza cifra 
      delay(5);
    }  
    
  showDigit(currentPosition);
  displayNumber(cifra[currentPosition],dpState);
  delay(5);
  
  for(i = currentPosition+1; i < noOfDisplays; i++) 
    {
      showDigit(i); // aprinde display-ul i 
      displayNumber(cifra[i],HIGH); // afiseza cifra 
      delay(5);
    }  
 
  if(xValue < minValue && joyMovedX == false && dpState == LOW)
  {
    currentPosition--;
    if(currentPosition<0)
      currentPosition += noOfDisplays;
    joyMovedX = true;
  }

   if(xValue > maxValue && joyMovedX == false && dpState == LOW)
  {
    currentPosition++;
    if(currentPosition >= noOfDisplays)
      currentPosition=0;
    joyMovedX = true;
  }
  
  if(yValue < minValue && joyMovedY == false && dpState == HIGH)
  {
    if(cifra[currentPosition] >0)
      cifra[currentPosition]--;
    else cifra[currentPosition] = 9;

    joyMovedY = true;
  }

  if(yValue > maxValue && joyMovedY == false && dpState == HIGH)
  {
    if(cifra[currentPosition]<9)
      cifra[currentPosition]++;
    else cifra[currentPosition] = 0;
    
    joyMovedY = true;
  }

  if(xValue <= maxValue && xValue >= minValue)
    joyMovedX = false;
    
  if(yValue <= maxValue && yValue >= minValue)
    joyMovedY = false;
}
