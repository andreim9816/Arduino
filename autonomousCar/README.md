# Maze solver

Maze solver robot that must find its way through a labyrinth. The algorithm monitoring the wall is one of the most popular algorithms for finding a way out of the maze, and is also known as the "Rule of the right hand". If the maze of connections, or if all its walls are connected to each other, then if the robot is inside the maze, backing to one side of the wall during the entire passage through the maze, there is a guaranteed way out of the maze, otherwise the robot would be returned to the entrance of the labyrinth and thereby make the rounds of each path in the maze at least once. The task is to go through the entire robot labyrinth and successfully complete it. Therefore, the robot passes through some mazes automatically (independently), and through some software (known in advance maze)

**Hardware / BOM**
<br>

1. Arduino Uno
1. 2x Breadboard
1. Wires
1. 20cm x 15cm plastic board (the support of the car)
1. 2x DC motor
1. IR receiver
1. L293D driver
1. 3x Ultrasonic sensor (HC-SR04)
1. 2x Resistor (220 ohm)
1. 2x Led (green and blue)
1. 2x Wheel
1. Caster Wheel
1. 4 x AAA Battery Holder
1. 4x AAA battery
1. 9V battery

[Click here of the link](https://docs.google.com/spreadsheets/d/1Htry010sDG5Vxl1XxuDkIDsEU6a6pIBbHVVmY9l-o_E/edit#gid=253948986)

<br>

**Photo of the entire project**
// de inserat poza

<br>

**Schematics for the car**

// de facut schema fritzing

<br>

**Demo video** 
[click here](https://)

<br>

**Instructions / features**

The motors of the car are turned on/off by the IR remote. To power it, you need to press the **>>|** button, and to stop it, the **">|"** button. Once the motors are powered up, the car started to move. There are 3 main cases in which the car behaves:

1. There is a new path on the right side of the car. Then, no matter it the car can 







