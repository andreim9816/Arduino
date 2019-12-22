#include<LedControl.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

const int pinBuzzer = A4;
const int pinSW = A2;
const int pinY = A1;
const int pinX = A0;
const int pinGreen = 9;
const int pinRed = 8;
const int enable = 7;
const int RS = 6;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;

int pinGreenState = HIGH;
int pinRedState = LOW;

unsigned long long pinGreenTime = 0;
unsigned long long pinRedTime = 0;

LedControl lc = LedControl(12, 11, 10, 1); // DIN, CLK, LOAD, No of drivers
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

const int heightMatrix = 8;
const int widthMatrix = 8;
const int paddleWidth = 2;

int posPaddleAI = widthMatrix / 2;
int posPaddle = widthMatrix / 2;
int posXBall = widthMatrix / 2;
int posYBall = heightMatrix / 2;

const int moveXBallStart = -1;
const int moveYBallStart = 0;
int moveXBall = -1;
int moveYBall = 0;

const int maxValue = 900;
const int minValue = 100;

const int intervalMovePaddle = 140; // if 140s have passed and i keep moving the paddle
const int intervalMoveBall = 250;
const int intervalIncrementLevel = 8000;

int speedMovePaddle = intervalMovePaddle;
int speedMoveBall = intervalMoveBall;
int incrementLevel = intervalIncrementLevel;

unsigned long long timePressedX = 0;
unsigned long  timeMovedBall = 0;

int xValue = 0;
int yValue = 0;

bool newHighscore = false;
bool welcomeMessageDisplayed;

bool xMoved = false;
bool changedMoveX = false;
bool changedMoveY = false;

bool firstLifeGenerated = false;
bool secondLifeGenerated = false;
bool thirdLifeGenerated = false;

int randomRow;
int randomCol;

const int eePromAddress = 0;
const int noOfOptions = 4;
const int maxOptionSetValue = 3;
const int maxLevelValue = 30;

const int defaultLevel = 1;
const int defaultLives = 3;
const int defaultScore = 0;

int startedLevel = defaultLevel;

int lives = defaultLives;
int level = defaultLevel;
int score = 0;
unsigned long long gameStartedMilis = 0;

const int levelFirstObstacle = 8;
const int levelSecondObstacle = 13;

const int rowFirstObstacle = heightMatrix / 2;
const int colFirstObstacle = 0;

const int rowSecondObstacle = heightMatrix / 2 ;
const int colSecondObstacle = widthMatrix - 2;

const int nameLength = 11;

bool pressedButton = false;
bool pressedButtonSettings = false;
bool justStarted = true;

bool nameSelected = false;
bool levelSelected = false;

char name[nameLength] = "AAAAAAAAAA";
char namehighscore[nameLength] = "UNKNOWN";

int currOption = 0;
int currOptionSet = 0;
int currOptionSetName = 0;
int currOptionSetNameLetter = 0;
int currOptionScreen2 = 0;

int selectedCategory = 0;
int highscore = 0;
bool tryAgain = false;

unsigned long long firstTime = 0;
unsigned long long lastTime = 0;
unsigned long long lastTimeGameOver = 0;
unsigned long long lastTimeStartGame = 0;
unsigned long long lastTimeExtraLifeGenerated = 0;
unsigned long long lastTimeExtraLifeDisplayed = 0;
unsigned long long timeFinishedGame = 0;

int swState = LOW;
int lastSwState = LOW;

bool joyMovedX = false;
bool joyMovedY = false;

const int mainMenu = 0;
const int inGame = 1;
const int highscoreCat = 2;
const int settings = 3;
const int info = 4;
const int gameOver = 5;
const int screen1Highscore = 6;
const int screen1NoHighscore = 7;
const int screen2 = 8;

int currColMatrixOver = 0;
int currColMatrixStart = 0;
int currColWelcomeMatrix = 0;

unsigned long long timeHighscore = 0;
unsigned long long lastTimeWelcomeMatrix = 0;

// inital matrix that every level starts with
const int matrixStart [widthMatrix][heightMatrix] = {
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 2, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
};

// the current matrix ( while i'm playing)
int matrix[heightMatrix][widthMatrix]  = {
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 2, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
};

// "START GAME" message that will be displayed on the matrix, while i'm scrolling through the menu
const bool startGameMatrix[8][62] PROGMEM = {
  {0, 0, 0, 0, 0,   0, 0, 0, 0 ,   0,   0, 0, 0, 0 , 0,   0,   0, 0, 0, 0 ,   0,   0, 0, 0, 0 ,   0,  0, 0, 0,  0 , 0,  0, 0, 0, 0,   0, 0, 0, 0 ,   0,   0, 0, 0, 0  , 0,   0, 0, 0, 0    , 0,   0, 0, 0   , 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  {0, 0, 0, 0, 0,   0, 1, 1, 1 ,   0,   1, 1, 1, 1 , 1,   0,   0, 1, 1, 0 ,   0,   1, 1, 1, 0 ,   0,  1, 1, 1,  1 , 1,  0, 0, 0, 0,   0, 1, 1, 0 ,   0,   0, 1, 1, 0  , 0,   1, 0, 0, 1    , 0,   1, 1, 1   , 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  {0, 0, 0, 0, 0,   1, 0, 0, 0 ,   0,   0, 0, 1, 0 , 0,   0,   1, 0, 0, 1 ,   0,   1, 0, 0, 1 ,   0,  0, 0, 1,  0 , 0,  0, 0, 0, 0,   1, 0, 0, 1 ,   0,   1, 0, 0, 1  , 0,   1, 1, 1, 1    , 0,   1, 0, 0   , 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  {0, 0, 0, 0, 0,   0, 1, 0, 0 ,   0,   0, 0, 1, 0 , 0,   0,   1, 0, 0, 1 ,   0,   1, 1, 1, 0 ,   0,  0, 0, 1,  0 , 0,  0, 0, 0, 0,   1, 0, 0, 0 ,   0,   1, 0, 0, 1  , 0,   1, 1, 1, 1    , 0,   1, 1, 1   , 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  {0, 0, 0, 0, 0,   0, 0, 1, 0 ,   0,   0, 0, 1, 0 , 0,   0,   1, 1, 1, 1 ,   0,   1, 0, 1, 0 ,   0,  0, 0, 1,  0 , 0,  0, 0, 0, 0,   1, 0, 1, 1 ,   0,   1, 1, 1, 1  , 0,   1, 0, 0, 1    , 0,   1, 0, 0   , 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  {0, 0, 0, 0, 0,   0, 0, 0, 1 ,   0,   0, 0, 1, 0 , 0,   0,   1, 0, 0, 1 ,   0,   1, 0, 1, 0 ,   0,  0, 0, 1,  0 , 0,  0, 0, 0, 0,   1, 0, 0, 1 ,   0,   1, 0, 0, 1  , 0,   1, 0, 0, 1    , 0,   1, 0, 0   , 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  {0, 0, 0, 0, 0,   1, 1, 1, 0 ,   0,   0, 0, 1, 0 , 0,   0,   1, 0, 0, 1 ,   0,   1, 0, 0, 1 ,   0,  0, 0, 1,  0 , 0,  0, 0, 0, 0,   0, 1, 1, 0 ,   0,   1, 0, 0, 1  , 0,   1, 0, 0, 1    , 0,   1, 1, 1   , 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  {0, 0, 0, 0, 0,   0, 0, 0, 0 ,   0,   0, 0, 0, 0 , 0,   0,   0, 0, 0, 0 ,   0,   0, 0, 0, 0 ,   0,  0, 0, 0,  0 , 0,  0, 0, 0, 0,   0, 0, 0, 0 ,   0,   0, 0, 0, 0  , 0,   0, 0, 0, 0    , 0,   0, 0, 0   , 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

// "GAME OVER" message that is displayed on the matrix after the game is over
const bool gameOverMatrix[8][54] PROGMEM = {
  {0, 0, 0, 0, 0,   0, 0, 0, 0 ,  0,  0, 0, 0, 0 ,  0,  0, 0, 0, 0 ,  0,  0, 0, 0 ,  0, 0, 0, 0,  0, 0, 0, 0 ,  0,  0, 0, 0, 0 ,  0,  0, 0, 0 ,  0,  0, 0, 0, 0 ,  0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0,   0, 1, 1, 0 ,  0,  0, 1, 1, 0 ,  0,  1, 0, 0, 1 ,  0,  1, 1, 1 ,  0, 0, 0, 0,  0, 1, 1, 0 ,  0,  1, 0, 0, 1 ,  0,  1, 1, 1 ,  0,  1, 1, 1, 0 ,  0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0,   1, 0, 0, 1 ,  0,  1, 0, 0, 1 ,  0,  1, 1, 1, 1 ,  0,  1, 0, 0 ,  0, 0, 0, 0,  1, 0, 0, 1 ,  0,  1, 0, 0, 1 ,  0,  1, 0, 0 ,  0,  1, 0, 0, 1 ,  0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0,   1, 0, 0, 0 ,  0,  1, 0, 0, 1 ,  0,  1, 1, 1, 1 ,  0,  1, 1, 1 ,  0, 0, 0, 0,  1, 0, 0, 1 ,  0,  1, 0, 0, 1 ,  0,  1, 1, 1 ,  0,  1, 0, 1, 0 ,  0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0,   1, 0, 1, 1 ,  0,  1, 1, 1, 1 ,  0,  1, 0, 0, 1 ,  0,  1, 0, 0 ,  0, 0, 0, 0,  1, 0, 0, 1 ,  0,  1, 0, 0, 1 ,  0,  1, 0, 0 ,  0,  1, 1, 1, 0 ,  0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0,   1, 0, 0, 1 ,  0,  1, 0, 0, 1 ,  0,  1, 0, 0, 1 ,  0,  1, 0, 0 ,  0, 0, 0, 0,  1, 0, 0, 1 ,  0,  1, 0, 0, 1 ,  0,  1, 0, 0 ,  0,  1, 0, 1, 0 ,  0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0,   0, 1, 1, 0 ,  0,  1, 0, 0, 1 ,  0,  1, 0, 0, 1 ,  0,  1, 1, 1 ,  0, 0, 0, 0,  0, 1, 1, 0 ,  0,  0, 1, 1, 0 ,  0,  1, 1, 1 ,  0,  1, 0, 0, 1 ,  0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0,   0, 0, 0, 0 ,  0,  0, 0, 0, 0 ,  0,  0, 0, 0, 0 ,  0,  0, 0, 0 ,  0, 0, 0, 0,  0, 0, 0, 0 ,  0,  0, 0, 0, 0 ,  0,  0, 0, 0 ,  0,  0, 0, 0, 0 ,  0, 0, 0, 0, 0, 0, 0, 0, 0}
};

// "WELCOME" message that is displayed on the matrix after the Arduino is pluged in ( for about 5 seconds just to show the whole message).
const bool welcomeMatrix[8][43] = {
  {0, 0, 0, 0, 0,   0, 0, 0, 0 ,  0,  0, 0, 0,   0,   0, 0, 0, 0  ,  0, 0, 0, 0    , 0, 0, 0, 0  , 0,  0, 0, 0, 0  , 0,  0, 0, 0, 0,   0, 0, 0 , 0, 0, 0, 0},
  {0, 0, 0, 0, 0,   0, 1, 0, 1 ,  0,  1, 1, 1,   0,   1, 0, 0, 0  ,  0, 1, 1, 0    , 0, 1, 1, 0  , 0,  1, 0, 0, 1  , 0,  1, 1, 1, 0,   0, 0, 1 , 0, 0, 0, 0},
  {0, 0, 0, 0, 0,   0, 1, 0, 1 ,  0,  1, 0, 0,   0,   1, 0, 0, 0  ,  1, 0, 0, 0    , 1, 0, 0, 1  , 0,  1, 1, 1, 1  , 0,  1, 0, 0, 0,   0, 0, 1 , 0, 0, 0, 0},
  {0, 0, 0, 0, 0,   0, 1, 0, 1 ,  0,  1, 1, 1,   0,   1, 0, 0, 0  ,  1, 0, 0, 0    , 1, 0, 0, 1  , 0,  1, 1, 1, 1  , 0,  1, 1, 1, 0,   0, 0, 1 , 0, 0, 0, 0},
  {0, 0, 0, 0, 0,   0, 1, 1, 1 ,  0,  1, 0, 0,   0,   1, 0, 0, 0  ,  1, 0, 0, 0    , 1, 0, 0, 1  , 0,  1, 0, 0, 1  , 0,  1, 0, 0, 0,   0, 0, 1 , 0, 0, 0, 0},
  {0, 0, 0, 0, 0,   0, 1, 1, 1 ,  0,  1, 0, 0,   0,   1, 0, 0, 0  ,  1, 0, 0, 0    , 1, 0, 0, 1  , 0,  1, 0, 0, 1  , 0,  1, 0, 0, 0,   0, 0, 0 , 0, 0, 0, 0},
  {0, 0, 0, 0, 0,   0, 1, 0, 1 ,  0,  1, 1, 1,   0,   1, 1, 1, 0  ,  0, 1, 1, 0    , 0, 1, 1, 0  , 0,  1, 0, 0, 1  , 0,  1, 1, 1, 0,   0, 0, 1 , 0, 0, 0, 0},
  {0, 0, 0, 0, 0,   0, 0, 0, 0 ,  0,  0, 0, 0,   0,   0, 0, 0, 0  ,  0, 0, 0, 0    , 0, 0, 0, 0  , 0,  0, 0, 0, 0  , 0,  0, 0, 0, 0,   0, 0, 0 , 0, 0, 0, 0}

};

// sets the arrow on the current choice ( for the Main Menu)
void setOptionMenu(int position)
{
  switch (position)
  {
    case 0: lcd.setCursor(0, 0); lcd.print(">"); break;
    case 1: lcd.setCursor(6, 0); lcd.print(">"); break;
    case 2: lcd.setCursor(0, 1); lcd.print(">"); break;
    case 3: lcd.setCursor(10, 1); lcd.print(">"); break;
  }
}

// sets the arrow on the current choice ( for the Settings Menu)
void setOptionSet(int position)
{
  switch (position)
  {
    case 0 : lcd.setCursor(0, 0); lcd.print(">"); break;
    case 1 : lcd.setCursor(10, 0); lcd.print(">"); break;
    case 2 : lcd.setCursor(0, 1); lcd.print(">"); break;
  }

}

/*Function that updates the level every -- timeIncrementLevel -- miliseconds and updates the speed of the ball, paddle and timeIncremenetLevel */
void updateLevel()
{
  if (level >= 5)
  {
    incrementLevel = 7500;
    speedMoveBall = 200;
    speedMovePaddle = 130;
  }

  if (level >= 10)
  {
    incrementLevel = 6000;
    speedMoveBall = 170;
    speedMovePaddle = 120;
  }

  if (level >= 13)
  {
    incrementLevel = 5500;
    speedMoveBall = 155;
    speedMovePaddle = 105;
  }

  if (level >= 16)
  {
    incrementLevel = 5000;
    speedMoveBall = 130;
    speedMovePaddle = 95;
  }

  lastTime = millis();

  if (lastTime - firstTime >= incrementLevel)
  {
    level++;
    firstTime = lastTime;
  }
}

/* Function that updates the player's score, by adding the level to the current score */
void updateScore()
{
  score = score + level;
}

/* Function that updates the level using the Y axis */
void modifyLevel() 
{
  if ( yValue > maxValue && joyMovedY == false)
  {
    level--;
    if (level < 1)
      level = maxLevelValue;
    joyMovedY = true;
  }

  if (yValue < minValue && joyMovedY == false)
  {
    level++;
    if (level > maxLevelValue)
      level = 1;
    joyMovedY = true;
  }

  startedLevel = level;

  if (yValue <= maxValue && yValue >= minValue)
    joyMovedY = false;
}

/* Function that turns on the Green Led */
void lightGreenLed()
{
  pinGreenState = HIGH;
  digitalWrite(pinGreen, pinGreenState);
  pinGreenTime = millis();
}

/* Function that turns on the Red Led */
void lightRedLed()
{
  pinRedState = HIGH;
  digitalWrite(pinRed, pinRedState);
  pinRedTime = millis();
}

/* Function that turns off the Green Led */
void unlightGreenLed()
{
  pinGreenState = LOW;
  digitalWrite(pinGreen, pinGreenState);
}

/* Function that turns off the Red Led */
void unlightRedLed()
{
  pinRedState = LOW;
  digitalWrite(pinRed, pinRedState);
}

/* Function that turns off one led and turns on the other on */
void lightGreenAndRed()
{
  if (pinRedState == HIGH && millis() - pinRedTime >= 500)
  {
    unlightRedLed();
    lightGreenLed();
  }
  else if (pinGreenState == HIGH && millis() - pinGreenTime >= 500)
  {
    unlightGreenLed();
    lightRedLed();
  }
}

/* Function that displays on the 8x8 matrix the "Start Game" message */
void displayMainMenuMessage()
{

  lastTimeStartGame = -1000;

  for (int row = 0 ; row < widthMatrix ; row++)
    for (int col = 0 ; col < heightMatrix ; col++)
    {
      bool val = pgm_read_byte_near( startGameMatrix[row] + (col + currColMatrixStart) % 61);
      lc.setLed(0, heightMatrix - 1 - row , widthMatrix - 1 - col, val);
    }

  if (millis() - lastTimeStartGame >= 1500)
  {
    delay(70); // delay so the message on the matrix will be easy to read
    currColMatrixStart++;
    lastTimeStartGame = millis();
  }
}

/* Function that displays on the 8x8 matrix the "Game Over" message */
void displayGameOverMessage()
{
  for (int row = 0 ; row < widthMatrix ; row++)
    for (int col = 0 ; col < heightMatrix ; col++)
    {
      bool val = pgm_read_byte_near( gameOverMatrix[row] + (col + currColMatrixOver) % 56);
      lc.setLed(0, heightMatrix - 1 - row , widthMatrix - 1 - col, val);
    }

  if (millis() - lastTimeGameOver >= 100)
  {
    delay(20); // delay so the message on the matrix can be easly read
    currColMatrixOver++;
    lastTimeGameOver = millis();
  }
}

/* Main function that displays the LCD screen, depending on the category we're in */
void displayScreen()
{
  lcd.setCursor(0, 0);
  lcd.print("                 ");
  lcd.setCursor(0, 1);
  lcd.print("                 ");

  switch (selectedCategory)
  {
    case mainMenu:
      
      // Sets the arrow on the current option 
      setOptionMenu(currOption);

      lcd.setCursor(1, 0);
      lcd.print("Start");

      lcd.setCursor(7, 0);
      lcd.print("Highscore");

      lcd.setCursor(1, 1);
      lcd.print("Settings");

      lcd.setCursor(11, 1);
      lcd.print("Info");

      delay(1); // delay so the info on the lcd won't be blurred

      justStarted = true;

      if (welcomeMessageDisplayed == true)
      {
        displayMainMenuMessage();
        lightGreenAndRed();
      }
      else  displayWelcomeMessage();

      break;

    case inGame:
      if (justStarted == true) // if the game has just begun, remember the time it started
      {
        justStarted = false;
        firstTime = millis();
        gameStartedMilis = firstTime;

        changedMoveX = false;
        changedMoveY = false;

        //init the matrix 
        int row, col;
        for (row = 0 ; row < heightMatrix ; row++)
          for (col = 0 ; col < widthMatrix ; col++)
            matrix[row][col] = matrixStart[row][col];

        // init the positions of the paddles
        posPaddleAI = widthMatrix / 2;
        posPaddle = widthMatrix / 2 ;
        
        // init the position of the ball
        posXBall = 4 ;
        posYBall = 4 ;

        // init the movement of the ball
        moveXBall = moveXBallStart;
        moveYBall = moveYBallStart;
        
        timeMovedBall = 0;
        lastTimeExtraLifeGenerated = millis();

        // turn off the leds
        unlightRedLed();
        unlightGreenLed();

        if (tryAgain == true)
          level = startedLevel;
      }

      Game();

      lcd.setCursor(0, 0);
      lcd.print("Lives:");
      lcd.print(lives);

      lcd.setCursor(8, 0);
      lcd.print("Level:");
      lcd.print(level);

      lcd.setCursor(0, 1);
      lcd.print("Score:");
      lcd.print(score);
      delay(10); // delay so the info on the lcd won't be blurred

      break;

    case highscoreCat:
    
      lcd.setCursor(0, 0);
      lcd.print("Highscore:");
      lcd.print(highscore);

      lcd.setCursor(0, 1);
      lcd.print(namehighscore);

      lcd.setCursor(11, 1);
      lcd.print(">Exit");
      delay(1); //delay so the info on the lcd won't be blurred

      // if the button is pressed, then we go back to the Main Menu
      if (pressedButton == true)
      {
        selectedCategory = mainMenu;
        currOption = 0;
      }
      justStarted = true;

      // if the "welcome" message was displayed, then we can display on the 8x8 matrix "Start game" message
      if (welcomeMessageDisplayed == true)
      {
        displayMainMenuMessage();
        lightGreenAndRed();
      }

      break;

    case settings:

      lcd.setCursor(0, 0);
      lcd.print(" Level:");
      lcd.print(level);

      lcd.setCursor(11, 0);
      lcd.print("Exit");

      lcd.setCursor(1, 1);
      lcd.print("Name:");
      lcd.print(name);
      delay(3); // delay so the info on the lcd won't be blurred

      // When entering, set the position of the arrow on the first choice
      if (justStarted == true)
      {
        justStarted = false;
        currOptionSet = 0;
      }

      // Displays the arrow on the current choice
      setOptionSet(currOptionSet);

      // Modifies the level when button pressed
      if (currOptionSet == 0 && levelSelected == true)
        modifyLevel();

      // Goes back to the Main Menu when the button is pressed and the choice is the Exit one
      if (pressedButton == true && currOptionSet == 1) // to exit and go back to the Menu
      {
        selectedCategory = mainMenu;
        currOption = 0;
      }

      // if the "welcome" message was displayed, then we can display on the 8x8 matrix "Start game" message
      if (welcomeMessageDisplayed == true)
      {
        displayMainMenuMessage();
        lightGreenAndRed();
      }

      break;
    
    case info:

      lcd.setCursor(0, 0);
      lcd.print("Pong Game >Exit");
      lcd.setCursor(0, 1);
      lcd.print("@UnibucRobotics");

      // if the joystick was pressed, we go back to the main menu
      if (pressedButton == true)
      {
        selectedCategory = mainMenu;
        currOption = 0;
      }
      justStarted = true;

      // if the "welcome" message was displayed, then we can display on the 8x8 matrix "Start game" message
      if (welcomeMessageDisplayed == true)
      {
        displayMainMenuMessage();
        lightGreenAndRed();
      }

      break;

    // The game is over, if the score is bigger than the highscore, it will be memorized and waits for the button to be pressed to enter the Main Menu again
    case gameOver:

      timeFinishedGame = millis();

      // if there is a new highscore, it will be memorized in the EEPROM, along with the name 
      if (score > highscore)
      {
        highscore = score;
        strcpy(namehighscore, name);
        EEPROM.put(eePromAddress , highscore );
        EEPROM.put(eePromAddress + sizeof(highscore), namehighscore);

        selectedCategory = screen1Highscore;
      }
      else selectedCategory = screen1NoHighscore;

      break;

    
    case screen1Highscore:

      displayGameOverMessage();

      lcd.setCursor(0, 0);
      lcd.print("New highscore!!!");
      lcd.setCursor(0, 1);
      lcd.print("   !!!");
      lcd.print(highscore);
      lcd.print(" !!!!");
      delay(10); //delay so the info on the lcd won't be blurred

      /*
            if (millis() - timeFinishedGame >= 5000) // after 5 secs, 2nd screen appears
              selectedCategory = screen2;
      */  
      
      // to advance to the next screen, you need to press the button
      if(pressedButton == true)
        selectedCategory = screen2;
   
      break;

    case screen1NoHighscore:

      displayGameOverMessage();

      lcd.setCursor(0, 0);
      lcd.print(name);
      lcd.print(", your");
      lcd.setCursor(0, 1);
      lcd.print("Score was ");
      lcd.print(score);
      delay(10); // delay so the info on the lcd won't be blurred

   /*   if (millis() - timeFinishedGame >= 5000) // after 5 secs, 2nd screen appears
        selectedCategory = screen2; 
   */

   // to advance to the next screen, you need to press the button
    if(pressedButton == true)
        selectedCategory = screen2;
      break;

    case screen2:

      displayGameOverMessage();

      lcd.setCursor(0, 0);
      lcd.print(" Exit");
      lcd.setCursor(1, 1);
      lcd.print("Try again?");

      // displays the ">"
      if (currOptionScreen2 == 0)
      {
        lcd.setCursor(0, 0);
        lcd.print(">");
      }
      else if (currOptionScreen2 == 1)
      {
        lcd.setCursor(0, 1);
        lcd.print(">");
      }

      delay(10); // delay so the info on the lcd won't be blurred

      // if the joystick is pressed, then it enters the selected category
      if (pressedButton == true)
      {
        // initializes some variables
        pressedButton = false;
        pressedButtonSettings = false;
        justStarted = true;

        level = defaultLevel;
        lives = defaultLives;
        score = defaultScore;

        firstLifeGenerated = false;
        secondLifeGenerated = false;
        thirdLifeGenerated = false;

        incrementLevel = intervalIncrementLevel;
        speedMovePaddle = intervalMovePaddle;
        speedMoveBall = intervalMoveBall;

        currOption = 0;
        gameStartedMilis = millis();

        currColMatrixOver = 0;
        currColMatrixStart = 0;

        lightRedLed();
        lastTimeGameOver = 0;

        // if we exit to the main menu
        if (currOptionScreen2 == 0) // Exit
        {
           strcpy(name, "AAAAAAAAAA");
          selectedCategory = mainMenu;
          currOption = 0;
          level = defaultLevel;
          startedLevel = defaultLevel;
          tryAgain = false;
        }
        // Try again option
        else if (currOptionScreen2 == 1) 
        {
          selectedCategory = inGame;
          level = startedLevel; // the game starts at the same level the player set it last game, and with the same name
          tryAgain = true;
        }

        currOptionScreen2 = 0;
      }

      break;
  }
}


void readValues()
{
  // function that reads the values ( X and Y axis and whenever the button is pressed) and does some of the job, too
  pressedButton = false;

  swState = digitalRead(pinSW);
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  if (swState != lastSwState)
    if (swState == LOW) // if the joystick Button is pressed
    {
      pressedButton = true;

      // if the selected category is Settings
      if (selectedCategory == settings)
        pressedButtonSettings = true;

      // if the button is pressed on the << Name >> choice in the Settings Menu, its state is changed
      if (pressedButtonSettings == true && currOptionSet == 2)
      {
        nameSelected = !nameSelected;
        currOptionSetName = 0;
      }

      // if the button is pressed on the << Level >> choice in the Settings Menu, its state is changed
      if (pressedButtonSettings == true && currOptionSet == 0)
        levelSelected = !levelSelected;
    }

  lastSwState = swState;

  if (xValue < minValue && joyMovedX == false)
  {
    // changes the current option on the Main Menu
    currOption--;
    if (currOption < 0)
      currOption += noOfOptions;

    // changes the current option on the game Over menu, 2nd screen
    if (selectedCategory == screen2)
    {
      currOptionScreen2--;
      if (currOptionScreen2 < 0)
        currOptionScreen2 = 1;
    }

    // if the current category is Settings and neither Name nor Level are selected to be modified, the current option is changed in the category
    if (nameSelected == false && levelSelected == false && selectedCategory == settings)
    {
      currOptionSet--;
      if (currOptionSet < 0)
        currOptionSet = maxOptionSetValue - 1;
    }

    // if the name is being changed on the Settings category, it moves to the current letter
    if (nameSelected == true && selectedCategory == settings && currOptionSet == 2)
    {
      currOptionSetName--;
      if (currOptionSetName < 0)
        currOptionSetName += nameLength - 1;
    }
    joyMovedX = true;
  }

  if (xValue > maxValue && joyMovedX == false)
  {
    // changes the current option on the Main Menu
    currOption++;
    if (currOption == noOfOptions)
      currOption = 0;

    // changes the current option on the game Over menu, 2nd screen
    if (selectedCategory == screen2)
    {
      currOptionScreen2++;

      if (currOptionScreen2 > 1)
        currOptionScreen2 = 0;
    }

    // if the current category is Settings and neither Name nor Level are selected to be modified, the current option is changed in the category
    if (nameSelected == false && levelSelected == false && selectedCategory == settings)
    {
      currOptionSet++;
      if (currOptionSet == maxOptionSetValue)
        currOptionSet = 0;
    }

    // if the name is being changed on the Settings category, it moves to the current letter
    if (nameSelected == true && selectedCategory == settings && currOptionSet == 2)
    {
      currOptionSetName++;
      if (currOptionSetName == nameLength - 1 )
        currOptionSetName = 0 ;
    }
    joyMovedX = true;
  }

  // in the Main Menu, when the button is pressed or moved on the Y axis, it enters the chosen category
  if ( (yValue > maxValue && joyMovedY == false ||  pressedButton == true) && selectedCategory == mainMenu)
  {
    selectedCategory = currOption + 1;
    pressedButton = false;
    joyMovedY = true;
  }

  // in the Main Menu, when the button is pressed or is moved on the Y axis, it enters the chosen category
  if (( yValue < minValue && joyMovedY == false ||   pressedButton == true) && selectedCategory == mainMenu)
  {
    selectedCategory = currOption + 1;
    pressedButton = false;
    joyMovedY = true;
  }

  // in the Settings category, when the current option is the Name one and it is being edited, the current letter is being changed
  if (yValue > maxValue && joyMovedY == false && selectedCategory == settings)
    if (nameSelected == true && currOptionSet == 2) // if I selected Name to modify
    {
      if (name[currOptionSetName] == 'Z')
        name[currOptionSetName] = ' ';
      else if (name[currOptionSetName] == ' ')
        name[currOptionSetName] = 'A';
      else name[currOptionSetName]++;

      joyMovedY = true;
    }

  // in the Settings category, when the current option is the Name one and it is being edited, the current letter is being changed
  if (yValue < minValue && joyMovedY == false && selectedCategory == settings)
    if (nameSelected == true && currOptionSet == 2) // if I selected Name to modify
    {
      if (name[currOptionSetName] == 'A')
        name[currOptionSetName] = ' ';
      else if (name[currOptionSetName] == ' ')
        name[currOptionSetName] = 'Z';
      else name[currOptionSetName]--;

      joyMovedY = true;
    }

  if (xValue <= maxValue && xValue >= minValue)
    joyMovedX = false;

  if (yValue <= maxValue && yValue >= minValue)
    joyMovedY = false;

  pressedButtonSettings = false;
}

/* Function that moves the paddle, depending the movement of the joystick */
void movePaddle()
{
  if (xValue > maxValue && xMoved == false)
  {
    xMoved = true;
    timePressedX = millis();
  }

  if (xValue > maxValue && xMoved == true)
  {
    if (millis() - timePressedX >= speedMovePaddle) // if i can move de paddle
    {
      timePressedX = millis();
      matrix[0][posPaddle] = matrix[0][posPaddle - 1] = 0;

      posPaddle --;
      if (posPaddle == 0)
        posPaddle = 1 ;

      matrix[0][posPaddle] = matrix[0][posPaddle - 1] = 1;
    }
  }

  if (xValue < minValue && xMoved == false)
  {
    xMoved = true;
    timePressedX = millis();
  }

  if (xValue < minValue && xMoved == true)
  {
    if (millis() - timePressedX >= speedMovePaddle) // if 100ms have passed, i can move the paddle
    {
      timePressedX = millis();

      matrix[0][posPaddle] = matrix[0][posPaddle - 1] = 0;
      posPaddle++;

      if (posPaddle >= widthMatrix)
        posPaddle = widthMatrix - 1;

      matrix[0][posPaddle] = matrix[0][posPaddle - 1] = 1;
    }
  }

  if (xValue >= minValue && xValue <= maxValue)
    xMoved = false;
}

/* Function that moves the AI's paddle */
void moveAI()
{
  // sets to 0 the the last line of the matrix
  for (int i = 0 ; i < heightMatrix; i++)
    matrix[heightMatrix - 1][i] = 0;

  if (moveYBall == -1) // if the ball is going right
    posPaddleAI = posYBall;
  else posPaddleAI = posYBall + 1;

  if (posYBall == 0)
    posPaddleAI = 1;

  matrix[heightMatrix - 1][posPaddleAI] = matrix[heightMatrix - 1][posPaddleAI - 1] = 1;
}

/* Function that moves the ball to the next position */
void moveBall()
{
  if (millis() - timeMovedBall >= speedMoveBall)
  {
    timeMovedBall = millis();
    matrix[posXBall][posYBall] = 0;

    if (posXBall == 1 && posYBall == 0 && posPaddle <= 2)
    {
      matrix[posXBall][posYBall] = 0;
      posXBall++;
      posYBall++;
      matrix[posXBall][posYBall] = 2;
      moveXBall = 1;
      moveYBall = 1;
    }
    else if (posXBall == 1 && posYBall == widthMatrix - 1 && posPaddle >= heightMatrix - 2 && posPaddle <= heightMatrix - 1 )
    {
      matrix[posXBall][posYBall] = 0;
      posXBall++;
      posYBall--;
      matrix[posXBall][posYBall] = 2;
      moveXBall = 1;
      moveYBall = -1;
    }
    else

    {
      changedMoveX = false;
      changedMoveY = false;

      posXBall += moveXBall;
      posYBall += moveYBall;
      matrix[posXBall][posYBall] = 2;
    }
  }
}

/* If the ball is about to hit a paddle */
void ballHitSomething()
{
  if (millis() - pinGreenTime >= 500 && pinGreenState == HIGH)
  {
    pinGreenState = LOW;
    digitalWrite(pinGreen, pinGreenState);
  }

  if (posXBall == 1) // if we about to hit the player's paddle
    if ( ( (posPaddle == (posYBall + moveYBall)) || ((posPaddle - 1) == (posYBall + moveYBall))) && changedMoveY == false)
    {
      updateScore();
      lightGreenLed();
      changedMoveY = true;
      moveXBall = -moveXBall;

      int randomNumber = random(-3, 4); // generates a random number {-3,-2,-1,0,1,2,3} to choose the ball's new direction
      if (randomNumber == 0)
        moveYBall = 0;
      else if (randomNumber < 0)
        moveYBall = -1;
      else moveYBall = 1;

      if (posYBall == widthMatrix - 1)
        moveYBall = -1;
      if (posYBall == 0)
        moveYBall = 1;

      moveBall();
    }

  if (posXBall == heightMatrix - 2) // if we about to hit the AI's paddle
    if ( ( (posPaddleAI == (posYBall + moveYBall)) || ((posPaddleAI - 1) == (posYBall + moveYBall)) ) && changedMoveY == false)
    {
      changedMoveY = true;
      moveXBall = -moveXBall;

      int randomNumber = random(-3, 4); // generates a random number {-3,-2,-1,0,1,2,3} to choose the ball's new direction
      if (randomNumber == 0)
        moveYBall = 0;
      else if (randomNumber < 0)
        moveYBall = -1;
      else moveYBall = 1;

      if (posYBall == widthMatrix - 1)
        moveYBall = -1;
      if (posYBall == 0)
        moveYBall = 1;

      moveBall();
    }

    
  if ((posYBall <= 0 || posYBall >= heightMatrix - 1) && changedMoveX == false)
  {
    moveYBall = -moveYBall;
    changedMoveX = true;
  }
}

/* if the ball is going to hit an obstacle */
void ballHitObstacle()
{
  if (level >= levelFirstObstacle) // if there is at least one obstacle
    if (posXBall != 1 && posXBall != heightMatrix - 2) // to be sure we are not about to hit a paddle
      if ( matrix[posXBall + moveXBall][posYBall + moveYBall] == 1 || ( posXBall == heightMatrix / 2) && posYBall + moveYBall <= 0 || ( level >= levelSecondObstacle && posXBall == heightMatrix / 2 - 1 && posYBall + moveYBall >= widthMatrix ) ) // if the ball hits an obstacle
      {

        moveXBall = -moveXBall;

        int randomNumber = random(-3, 4); // generates random number {-3,-2,-1,0,1,2,3}

        if (randomNumber == 0)
          moveYBall = 0;
        else if (randomNumber < 0)
          moveYBall = -1;
        else moveYBall = 1;

        if (posYBall <= 0)
          moveYBall = 1;

        if ( level >= levelSecondObstacle && posYBall >= widthMatrix - 1)
          moveYBall = -1;

        moveBall();
      }
}

/* Function that adds obstacles, depending on the level we're in */
void addObstacle()
{
  if ( level >= levelFirstObstacle)
    matrix[rowFirstObstacle][colFirstObstacle] = matrix[rowFirstObstacle][colFirstObstacle + 1] = 1;

  if ( level >= levelSecondObstacle)
    matrix[rowSecondObstacle][colSecondObstacle] = matrix[rowSecondObstacle][colSecondObstacle + 1] = 1;
}

/* Function that decrements the number of lives when the player failed to hit the ball when he had to */
void roundOver()
{
  if ( posXBall == 0) // if the ball didnt hit the paddle and is on the player's level, he loses one life
  {
    lives--;
    lastTimeExtraLifeGenerated = millis();
    tone(pinBuzzer, 10000); // the buzzer vibrates

    // for 3 seconds, the matrix freezes, the red led is turned on and the buzzer virbates
    if (lives != 0)
    {
      unsigned long long  currTime = millis();

      while (millis() - currTime <= 3000)
      {

       // turns on the red led
        digitalWrite(pinRed, HIGH); 
       
       // prints the info on the LCD
        lcd.setCursor(0, 0);
        lcd.print("Lives:");
        lcd.print(lives);

        lcd.setCursor(8, 0);
        lcd.print("Level:");
        lcd.print(level);

        lcd.setCursor(0, 1);
        lcd.print("Score:");
        lcd.print(score);
       
        displayMatrix();
      }

      // turns off the red led
      digitalWrite(pinRed, LOW);

      justStarted = true;
      displayScreen();
    }
    // turns of the buzzer
    noTone(pinBuzzer);

    for (int col = 0 ; col < widthMatrix; col++)
      matrix[0][col] = 0;

    matrix[0][widthMatrix / 2] = matrix[0][widthMatrix / 2 - 1 ] = 2;
    posPaddle = widthMatrix / 2;

  }
}

/* Function that returns true if the game is over (i.e. player ran out of lives) */
bool GameOver()
{
  if (lives == 0) // if the player ran out of lives, the game is over
    return true;
  return false;
}

void generateExtraLife()
{
  /*  Extra lives can be generated, randomly.
      When reached level 6, there will be a 1% chance for an extra life to be generated
      After you reach level 10, there is a 3% chance for an extra life to be geenrated, but AFTER THE LAST EXTRA LIFE WAS GENERATED
      After you reach level 14,there is a 5% chance for an extra life to be geenrated, but AFTER THE LAST EXTRA LIFE WAS GENERATED
  */

  if (level >= 6 && level < 10 && firstLifeGenerated == false)
    if (millis() - lastTimeExtraLifeGenerated >= 15000)
    {
      int randomNumber = random(0, 1000); // generates a random number

      if ( randomNumber >= 0 && randomNumber <= 9) // a new extra life is generated
      {
        lastTimeExtraLifeGenerated = millis();
        firstLifeGenerated = true;
        do
        {
          randomRow = random(2, heightMatrix - 2);
          randomCol = random(1, widthMatrix - 1);

        } while (matrix[randomRow][randomCol] != 0);

        matrix[randomRow][randomCol] = 3;
      }
    }

  // after 4 secs, the extra life will dissapear
  if (millis() - lastTimeExtraLifeGenerated >= 4000 && firstLifeGenerated == true && randomRow != -1 && randomCol != -1)
  {
    matrix[randomRow][randomCol] = 0;
    randomRow = randomCol = -1;
  }

  if (level >= 10 && level < 14  && secondLifeGenerated == false)
    if (millis() - lastTimeExtraLifeGenerated >= 11000)
    {
      int randomNumber = random(0, 1000); // generates a random number

      if ( randomNumber >= 0 && randomNumber <= 29) // a new extra life is generated
      {
        lastTimeExtraLifeGenerated = millis();
        secondLifeGenerated = true;
        do
        {
          randomRow = random(2, heightMatrix - 2);
          randomCol = random(1, widthMatrix - 1);

        } while (matrix[randomRow][randomCol] != 0);

        matrix[randomRow][randomCol] = 3;
      }
    }

  if (millis() - lastTimeExtraLifeGenerated >= 4000 && secondLifeGenerated == true && randomRow != -1 && randomCol != -1)
  {
    matrix[randomRow][randomCol] = 0;
    randomRow = randomCol = -1;
  }

  if (level >= 14 && thirdLifeGenerated == false)
    if (millis() - lastTimeExtraLifeGenerated >= 11000)
    {
      int randomNumber = random(0, 1000); // generates a random number

      if ( randomNumber >= 0 && randomNumber <= 49) // a new extra life is generated
      {
        lastTimeExtraLifeGenerated = millis();
        thirdLifeGenerated = true;
        do
        {
          randomRow = random(2, heightMatrix - 2);
          randomCol = random(1, widthMatrix - 1);

        } while (matrix[randomRow][randomCol] != 0);

        matrix[randomRow][randomCol] = 3;
      }
    }

  if (millis() - lastTimeExtraLifeGenerated >= 4000 && thirdLifeGenerated == true && randomRow != -1 && randomCol != -1)
  {
    matrix[randomRow][randomCol] = 0;
    randomRow = randomCol = -1;
  }
}

/* Function the increments the number of lives if the ball reached that extra life */
void hitExtraLife()
{
  if (posXBall == randomRow && posYBall == randomCol)
  {
    lives++;
    matrix[randomRow][randomCol] = 0;
    randomCol = randomRow = -1;
  }
}

/* Function that displays the 8x8 matrix */
void displayMatrix()
{
  int row, col;

  for (row = 0 ; row < widthMatrix ; row++)
    for (col = 0 ; col < heightMatrix ; col++)
    {
      if ( (matrix[row][col] == 3 || matrix[row][col] == -3) && ( (firstLifeGenerated || secondLifeGenerated || thirdLifeGenerated) == true) ) // if it's an extra life on the current position, it will be blinking 
      {
        if (millis() - lastTimeExtraLifeDisplayed >= 30) // changes it's state every 30 ms
        {
          lc.setLed(0, row, col, matrix[row][col] > 0 ? 1 : 0);
          matrix[row][col] = -matrix[row][col];
          lastTimeExtraLifeDisplayed = millis();
        }
      }
      else lc.setLed(0, row, col, matrix[row][col]);
    }
}

void Game()
{
  movePaddle(); // moves the paddle
  updateLevel(); // updates the level whenever it has to
  addObstacle(); // adds and obstacle if it has to

  generateExtraLife(); // generates and extra life 
  hitExtraLife(); // if the ball hits that extra life

  moveAI(); // moves the AI's paddle
  moveBall(); // moves the ball

  ballHitSomething(); // calculates the ball's new position after it hits the paadle
  ballHitObstacle(); // calculates the ball's new position after it hits an obstacle

  roundOver(); // if the player didnt hit the ball, he loses one life
  displayMatrix(); // displays the matrix

  if (GameOver()) // if there are no more lives, the players loses
    selectedCategory = gameOver; 
}

void setup()
{
  lc.shutdown(0, false); // turn driver 0 (the only one) off
  lc.setIntensity(0, 2); // 0 15
  lc.clearDisplay(0);
  lcd.begin(16, 2);
  randomSeed(analogRead(A5)); // seed is the input from an unconntected pin

  welcomeMessageDisplayed = false;

  pinMode(pinRed, OUTPUT);
  pinMode(pinGreen, OUTPUT);

  pinMode(pinSW, INPUT_PULLUP);

//  EEPROM.put(eePromAddress , 0);
//  EEPROM.put(eePromAddress + sizeof(highscore), "UNKNOWN");

  EEPROM.get(eePromAddress , highscore);
  EEPROM.get(eePromAddress + sizeof(int) , namehighscore);
  Serial.begin(9600);
}

void displayWelcomeMessage()
{
  if (!welcomeMessageDisplayed)
  {
    int row, col;
    lastTimeWelcomeMatrix = -1000;

    for (row = 0 ; row < widthMatrix ; row++)
      for (col = 0 ; col < heightMatrix ; col++)
        lc.setLed(0, heightMatrix - 1 - row , widthMatrix - 1 - col, welcomeMatrix[row][(col + currColWelcomeMatrix) % 47]);


    if (millis() - lastTimeWelcomeMatrix >= 1000)
    {
      delay(80); // delay so the message on the matrix will be easy to read  
      currColWelcomeMatrix++;

      if (millis() >= 4500) // after 4.5 seconds, the "Welcome" message has been displayed on the matrix and you can enter on the Menu
        welcomeMessageDisplayed = true;

      lastTimeWelcomeMatrix = millis();
    }
  }
}

void loop()
{

  if (welcomeMessageDisplayed == true)
  {
    readValues();
    displayScreen();
  }
  else
  {
    displayWelcomeMessage();
    lcd.setCursor(0, 0);
    lcd.print(" Welcome to the");
    lcd.setCursor(0, 1);
    lcd.print("Pong game.Enjoy!");
  }
}
