#include <LiquidCrystal.h>
#include <EEPROM.h>

const int RS = 12;
const int enable = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;

const int pinX = A0;
const int pinY = A1;
const int pinSW = A2;

const int eePromAddress = 0;
const int noOfOptions = 3;
const int maxOptionSetValue = 3;
const int maxLevelValue = 20;
const int timeDelay = 5000;

const int defaultLevel = 0;
const int defaultLives = 3;

const int nameLength = 11;
const int minValue = 80;
const int maxValue = 950;

bool pressedButton = false;
bool pressedButtonSettings = false;
bool justStarted = true;

bool nameSelected = false;
bool levelSelected = false;

char name[nameLength]= "AAAAAAAAAA";
char namehighscore[nameLength]= "UNKNOWN";

int currOption = 0;
int currOptionSet = 0;
int currOptionSetName = 0;
int currOptionSetNameLetter = 0;

int selectedCategory = 0;
int highscore = 0;
unsigned long long firstTime = 0;
unsigned long long lastTime = 0;
int lives = 3;
int level = 0;
int score = 0;
unsigned long long gameStartedMilis = 0;

int xValue = 0;
int yValue = 0;

int swState = LOW;
int lastSwState = LOW;

bool joyMovedX = false; 
bool joyMovedY = false;

LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

void setup()
{
 lcd.begin(16, 2);
 pinMode(pinSW, INPUT_PULLUP);

 //EEPROM.put(eePromAddress , 0);
 //EEPROM.put(eePromAddress + sizeof(highscore), "UNKNOWN");
 
 EEPROM.get(eePromAddress , highscore); 
 EEPROM.get(eePromAddress + sizeof(int) , namehighscore);
}

// sets the arrow on the current choice ( for the Main Menu)
void setOptionMenu(int position)
{
  switch(position)
  {
    case 0: lcd.setCursor(0,0); lcd.print(">"); break;
    case 1: lcd.setCursor(6,0); lcd.print(">"); break;
    case 2: lcd.setCursor(0,1); lcd.print(">"); break;
  }
}

// sets the arrow on the current choice ( for the Settings Menu)
void setOptionSet(int position)
{
  switch(position)
  {
    case 0 : lcd.setCursor(0,0); lcd.print(">"); break;
    case 1 : lcd.setCursor(10,0); lcd.print(">"); break;
    case 2 : lcd.setCursor(0,1); lcd.print(">"); break; 
  }
  delay(10);
}

// function that updates the level every -- timeDelay -- miliseconds
void updateLevel()
{
  lastTime = millis();
  score = level * 3;
  if(lastTime - firstTime >= timeDelay)
  {
    level++;
    firstTime = lastTime;
  }
}

// function that updates the level using the Y axis
void modifyLevel()
{  
  if( yValue > maxValue && joyMovedY == false) 
  {
    level--;
    if(level < 0)
     level = maxLevelValue;
    joyMovedY = true;
  }

  if(yValue < minValue && joyMovedY == false)
  {
    level++;
    if(level > maxLevelValue)
      level = 0;
    joyMovedY = true;
  }

   if(yValue <= maxValue && yValue >= minValue)
    joyMovedY = false;
}

// main function that displays the screen, depending on which Menu we're in
void displayScreen()
{
  lcd.setCursor(0,0);
  lcd.print("                 ");
  lcd.setCursor(0,1);
  lcd.print("                 ");
  
  switch(selectedCategory)
  {
    // Main Menu
    case 0:
      setOptionMenu(currOption); 
   
      lcd.setCursor(1,0);
      lcd.print("Start");
      
      lcd.setCursor(7,0);
      lcd.print("Highscore");
      
      lcd.setCursor(1,1);
      lcd.print("Settings");
      delay(10);
      
      justStarted = true ;
      
    break;
    
    // Game running 
    case 1:
     if(justStarted == true) // if the game just begun, remember the time it started
     {
      firstTime = millis();
      gameStartedMilis = firstTime;
      justStarted = false;
     }
     
      updateLevel(); // updates the level whenever it has to 
      
      lcd.setCursor(0,0);
      lcd.print("Lives:");
      lcd.print(lives);

      lcd.setCursor(8,0);
      lcd.print("Level:");
      lcd.print(level);

      lcd.setCursor(0,1);
      lcd.print("Score:");
      lcd.print(score);
      delay(10);
      
     if(lastTime >= gameStartedMilis + timeDelay *2) // if too much time passed, the game is over
        {
          selectedCategory = 4;
          currOption = 0; ///////
        }

    break;

    // Highscore Menu
    case 2:
      lcd.setCursor(0,0);
      lcd.print("highscore:");

    // gets the highscore from the EEPROM and prints it on the lcd
      lcd.print(highscore);
    
    // gets the highscore name from the EEPROM and prints it on the lcd
      lcd.setCursor(0,1);
      lcd.print(namehighscore);
      
      lcd.setCursor(11,1);
      lcd.print(">Exit");
      delay(10);
    
    // if the button is pressed, then we go back to the Main Menu 
      if(pressedButton == true)
      {
         selectedCategory = 0;
         currOption = 0;
      }
      justStarted = true;
      
    break;
    
    // Settings Menu 
    case 3:
  
      lcd.setCursor(0,0);
      lcd.print(" Level:");
      lcd.print(level);

      lcd.setCursor(11,0);
      lcd.print("Exit");
      
      lcd.setCursor(1,1);
      lcd.print("Name:");
      lcd.print(name);
     // delay(10);

      // When entering, set the position of the arrow on the first choice
      if(justStarted == true)
      {
        justStarted = false;
        currOptionSet = 0;
      }
   
      // Displays the arrow on the current choice
      setOptionSet(currOptionSet);

      // Modifies the level when button pressed
      if(currOptionSet == 0 && levelSelected == true)
        modifyLevel();

      // Goes back to the Main Menu when the button is pressed and the choice is the Exit one
      if(pressedButton == true && currOptionSet == 1) // to exit and go back to the Menu
        {
          selectedCategory = 0;
          currOption = 0;
        }
    break;
    
    // The game is over, if the score is bigger than the highscore, it will be memorized and waits for the button to be pressed to enter the Main Menu again
    case 4:    
   
    lcd.setCursor(0,0);
    lcd.print("Congrats! Press");
    lcd.setCursor(0,1);
    lcd.print("button to exit");
    
    if(score > highscore)
     {
      highscore = score;
      strcpy(namehighscore,name);
      EEPROM.put(eePromAddress , highscore );
      EEPROM.put(eePromAddress + sizeof(highscore), namehighscore);
     }
     
     if(pressedButton == true)
     {
        pressedButton = false;
        pressedButtonSettings = false;
        level = defaultLevel;
        lives = defaultLives;
        currOption = 0;
        score = 0;
        selectedCategory = 0;
        justStarted = true;
        gameStartedMilis = millis();
        strcpy(name,"AAAAAAAAAA");
      }
  //      delay(30);
    break;
  }
}

void readValues()
{
  // function that reads the values ( X and Y axis and whenever the button is pressed) and does some of the job too
  pressedButton = false;

  swState = digitalRead(pinSW);// delay(10);
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  
  if(swState != lastSwState)
  if(swState == LOW) // if the joystick Button is pressed
      {
         pressedButton = true;
      
        // if the selected category is Settings           
        if(selectedCategory == 3) 
          pressedButtonSettings = true;
  
        // if the button is pressed on the << Name >> choice in the Settings Menu, its state is changed
        if(pressedButtonSettings == true && currOptionSet == 2) 
          {
            nameSelected = !nameSelected;
            currOptionSetName = 0;
          }
          
         // if the button is pressed on the << Level >> choice in the Settings Menu, its state is changed
         if(pressedButtonSettings == true && currOptionSet == 0)  
           levelSelected = !levelSelected;
      }
   
  lastSwState = swState;
  
  if(xValue < minValue && joyMovedX == false)
  {
    // changes the current option on the Main Menu
      currOption--;
      if(currOption < 0)
        currOption += noOfOptions;
     
     
    // if the current category is Settings and neither Name nor Level are selected to be modified, the current option is changed in the category
    if(nameSelected == false && levelSelected == false && selectedCategory == 3)
       {
         currOptionSet--;
         if(currOptionSet < 0)
          currOptionSet = maxOptionSetValue - 1;
       }
  
    // if the name is being changed on the Settings category, it moves to the current letter
    if(nameSelected == true && selectedCategory == 3 && currOptionSet == 2)
    {
      currOptionSetName--;
      if(currOptionSetName < 0)
        currOptionSetName += nameLength - 1;
    }   
   joyMovedX = true;   
  }
  
  if(xValue > maxValue && joyMovedX == false)
  {
    // changes the current option on the Main Menu
    
      currOption++;
      if(currOption == noOfOptions)
        currOption = 0;
    
    
   // if the current category is Settings and neither Name nor Level are selected to be modified, the current option is changed in the category
    if(nameSelected == false && levelSelected == false && selectedCategory == 3)
       {
         currOptionSet++;
         if(currOptionSet == maxOptionSetValue)
          currOptionSet = 0;
       }
  
    // if the name is being changed on the Settings category, it moves to the current letter
    if(nameSelected == true && selectedCategory == 3 && currOptionSet == 2)
    {
      currOptionSetName++;
      if(currOptionSetName == nameLength - 1 )
        currOptionSetName = 0 ;
    }
   joyMovedX = true;
  }

  // in the Main Menu, when the button is pressed or moved on the Y axis, it enters the chosen category
  if( (yValue > maxValue && joyMovedY == false ||  pressedButton == true) && selectedCategory == 0)
  {
    selectedCategory = currOption+1;
    pressedButton = false;
    joyMovedY = true;
  }
  
  // in the Main Menu, when the button is pressed or is moved on the Y axis, it enters the chosen category
  if(( yValue < minValue && joyMovedY == false ||   pressedButton == true) && selectedCategory == 0)
  {
    selectedCategory = currOption+1; 
    pressedButton = false;
    joyMovedY = true;
  }
  
  // in the Settings category, when the current option is the Name one and it is being edited, the current letter is being changed
  if(yValue > maxValue && joyMovedY == false && selectedCategory == 3)
      if(nameSelected == true && currOptionSet == 2) // if I selected Name to modify 
      { 
        if(name[currOptionSetName] == 'Z')
          name[currOptionSetName] = ' ';
        else if(name[currOptionSetName] == ' ')
          name[currOptionSetName] = 'A';
        else name[currOptionSetName]++;
        
        joyMovedY = true;
      }

   // in the Settings category, when the current option is the Name one and it is being edited, the current letter is being changed
   if(yValue < minValue && joyMovedY == false && selectedCategory == 3)
      if(nameSelected == true && currOptionSet == 2) // if I selected Name to modify 
      {
        if(name[currOptionSetName] == 'A')
          name[currOptionSetName] = ' ';
        else if(name[currOptionSetName] == ' ')
          name[currOptionSetName] = 'Z';
        else name[currOptionSetName]--;
        
        joyMovedY = true;
      }
      
  if(xValue <= maxValue && xValue >= minValue)
    joyMovedX = false;
    
  if(yValue <= maxValue && yValue >= minValue)
    joyMovedY = false;  

  pressedButtonSettings = false;
}

void loop() 
{ 
  readValues();
  displayScreen();
}
