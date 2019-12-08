# Pong game

Pong is a two-dimensional sports game that simulates table tennis. The player controls an in-game paddle by moving it horizontaly across the upper or bottom side of the screen. He competes against the AI controlling a second paddle on the opposing side. They both use the paddles to hit a ball back and forth. The goal of the player is to stay in the game as long as possible.

**Hardware**

1. 8x8 Led matrix
1. LCD
1. Joystick
1. MAX7219 Driver
1. Buzzer
1. 2x Leds ( red and greed)



**Game specifications**

1. Score - The player's score increments with every hit he makes, multiplied by the level coefficient
1. Difficulty - Each game begins with a difficulty value set by the player before starting. Then, the difficulty increases as the player levels up (every 10 seconds) and the speed of the ball increases, too. When the player reaches level 5, new obstacles will appear on the screen and when the ball hits it, it will with bounce back, making it harder for the player.
1. During the game, the LCD will display 3 major info:
   1. Score
   1. Lives
   1. Level
1. Players - The game features only one player.
1. More lives - At random moments of time during the game, a blinking dot will appear on the screen. If the player hits it before the AI does it and while it still appears on the screen( the dot will be avaiable for a short time - 4 seconds), the number of lives increments by 1.
1. Finishing the game - when the player has no more lives, the game is finished.
1. Highscore - Everytime the game is finshed, if the score is bigger than the current highscore, the player's name and his score will be stored in the EEPROM

**LCD Menu**

In the main menu, there are 3 options:
  1. Start game - the game will start with the options made in the settings menu
  1. Settings - here you can set the player's name and the starting level value
  1. Highscore - here you will be able to see the player's biggest highscore and his name ( both saved in the EEPROM)
  
