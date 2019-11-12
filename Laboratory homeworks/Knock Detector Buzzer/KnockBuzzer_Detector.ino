const int passiveBuzzerPin = A0;
const int pushButton = 2;
const int activeBuzzerPin = 11;

int passiveBuzzerValue = 0;
int activeBuzzerValue = 50;
int buttonState = 0;
int threshold = 5;
int lastPassiveBuzzerValue = 0;

unsigned long currentTime = 0;
unsigned long lastTime = 0;
const int interval = 5000; // interval to be set at 5 secs

void setup() {

  pinMode(passiveBuzzerPin , INPUT);
  pinMode(activeBuzzerPin , OUTPUT);
  pinMode(pushButton, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() 
{
 
  passiveBuzzerValue = analogRead(passiveBuzzerPin);
 
  if(passiveBuzzerValue >= threshold)
    {
      lastTime = millis();
      lastPassiveBuzzerValue = passiveBuzzerValue;
      Serial.println("Knock!");
     }
    
  currentTime=millis(); // updating the current time
 
  if(currentTime - lastTime >= interval) // 5 secs have passed
  {
     if(lastPassiveBuzzerValue != 0)
     {
      lastTime = currentTime;
      tone(activeBuzzerPin , activeBuzzerValue);
     }
     
  }
  
  buttonState = digitalRead(pushButton);
  if(buttonState == 0)
  {
    noTone(activeBuzzerPin);
    lastPassiveBuzzerValue = 0;
  } 
}
