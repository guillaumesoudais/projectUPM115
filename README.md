# Escaping Robot (UPM115 Project)

## Introduction

The aim of the project is to make a robot that is able to find the exit of a maze.
The strategy of the robot is to take the direction that is on the most on the left.
For that we will use a ultrasonic sensor mounted on a servo motor to find this direction and two motors for the two wheels of the robot.
If we had enough time, we would have made this robot to map the maze it’s in.

## General architecture of the project

The project will have the following modules :
- The movement module which is the module that makes the robot moving. Its entry points are the direction and speed commands
- The sonar module which detects obstacles. Its entry points are angles for the servo motor and it must send the distance of the obstacle to the command module
- The command module which receive information from the sonar and decide of the direction and speed of the robot as well as its sonar orientation
Optional modules (if we have had enough time) :
- The mapping module : receives information from the servo and the command module to record obstacle position

## Stages of the project

To realise this project we will use an incremental method. Below are the stages of the project:
- Realizing the movement module : add a few tests function with recorded moves to verify that the module is working
- Realizing the sonar module : same principle as previous step
- Realizing the command module
Optional steps (if we have had enough time) :
- Mapping the path of the robot : be able to deduce the path of the robot from the commands of the wheels (admitting it will not slide)
- Mapping the obstacle : from the position of the robot obtained in the previous step and the position of the obstacle, mapping the obstacles of the maze
- Find a shorter path : after mapping the path of the robot, deduce a shorter path by eliminating dead-ends


## Realisation of the project

### Material needed
- Arduino uno
- Ultrasonic sensor
- Batteries
- DC motors
- Servo

### Construction
Schematics of the connection for the robot.
![alt text][schematics]

[schematics]: https://github.com/guillaumesoudais/projectUPM115/blob/master/schematics.png "Schematics"

Picture of the robot with all its messy wiring.

![alt text][robot]

[robot]: https://github.com/guillaumesoudais/projectUPM115/blob/master/robot.jpg "Schematics"

Here is a video of the final version of the project.

[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/FyLfuLq4kfI/0.jpg)](https://www.youtube.com/watch?v=FyLfuLq4kfI)


### Code structure

the main code is in mazeRobot.ino where we initialize the constant needed (e.g. pins).
Then we have three modules :
- sonar.h  that commands the sonar
- motors.h that commands the wheels
- brain.h that is where the escaping strategy is implemented.

#### Sonar

The sonar has a servo for attribute which can be commanded through the *setAngle* function.
It has also an *udpdate()* function that read the angle of the Servo and calculate the distance.

```{cpp}
/* sending the instructon to measure the distance */
digitalWrite(triggerPin, HIGH);
delayMicroseconds(10);
digitalWrite(triggerPin, LOW);

/* measuring time between sending and receiving of the impulsion */
long measure = pulseIn(echoPin, HIGH, MEASURE_TIMEOUT);

/* calculating the distance */
distance = measure / 2.0 * SOUND_SPEED;
distanceList[index] = distance;
index ++;
if (index == LENGTH) {
    index = 0;
}

float d = 0;
for (int i = 0; i < LENGTH; i++) {
    d += distanceList[i];
}
mean = d / LENGTH;
```

*mean* is an attribute that stores the mean of the ten last value (because sometimes values are not accurate).
*distance* is an attribute that stores the last value.
*distanceList* is an array that stores the ten last values.

the function *longUpdate* does ten *update* in order to renew the *distanceList*

#### motors

The motors controls the two motors. It can activate them together or separately in both direction of rotation.

It has a specific function : *slowMove*

Because we couldn't use the motor directly with low speed. We only had a 9V battery, so the torque was to high to go under 128 over 255 possible values of speed.

Then to recreate a low speed movement, we decided to make the wheels turn briefly and then stop in a 6 miliseconds loop.

We have the following function :

```{cpp}
void slowMove(int n,boolean forward=true) {
    move(255, forward);
    delay(n);
    stop();
    delay(6 - n);
}
```

then we have six different speeds with n going from 1 to 6.

#### brain

The brain has a object Motors and a Sonar as attributes, so it can command them.

But before looking at the code, we must understand the strategy used.

##### Escaping strategy

The strategy used to escape a maze is very simple.

Since the robot only knows what is around him and what he has been through, we can't use algorithm such as Dijkstra to find the shortest path.

So the path would not be optimal.

The strategy is to *always take the direction that is most at right*.

In mazes where the entry point and the exit are placed on the outside of the maze, this strategy doesn't suffer from loops.

In mazes where the arrival is not an exit but a point inside the maze, then loops can alter the resolution. But we won't consider this case here.

As a matter of fact, considering the amount of time we were given, we only made a robot that is able to follow a wall at its right side.

We've been through several strategies :
- first, we thought about dealing only with mazes with right-angled corners. Then we would only had to *make rotations of 90 degres* and moving forward. But the motors weren't turning at the exact same speed and a calibration would have taken too much time.
- then, we decided to use the same strategies as above but with *smaller angles* giving the robot the opportunity to easily correct the direction. So the robot was successively looking at his right, in front of him and at his left and turn from a little angle according to what he saw or moving forward and loop.
- Another technique was to always move forward and to increase the speed of one of the wheel according to the distance measured on the right. But the sonar was less efficient in movement.

So we finally used the second technique.

The strategy is basically :
- look on the right
- if there is no close obstacle on the right
    - turn on right of an 90 degres angle
    - if there is no obstacle, we move forward (function *pasEnAvant*)
    - if there is an obstacle or if it's been 5 time we turned right (meaning there is no wall near) we move forward until there is a wall (function *moveToWall()*)
- if there is an obtacle on the right
    - we look in front of the robot
    - while there is an obstacle in front of him 
        - the robot turn slightly left
        - the sonar update
    - then (there is no more obstacle in front of him) we move forward from a small distance (function *pasEnAvant*)

Here is a code of the main function *mainish* that does the above strategy :

```{cpp}
void mainish() {
    sonar->setAngle(0);
    sonar->longUpdate();
    if (sonar->mean > MARGIN) {
    motor->turnRight(1, quartTour);
    sonar->update();
    if (sonar->distance < MARGIN || counter == 5) {
        moveToWall();
        counter = 0;
    }
    else {
        pasEnAvant();
        counter++;
    }
    }
    else {
    counter = 0;
    int angle = S_ANGLE;
    sonar->setAngle(90);
    sonar->update();
    while (sonar->distance < MARGIN) {
        motor->turnLeft(1, smallTour);
        angle += S_ANGLE;
        sonar->update();
    }
    pasEnAvant();          
    }
}
```

Let's now take a look at *pasEnAvant* :

```{cpp}
void pasEnAvant() {
    sonar->setAngle(90);
    for (int i =  0; i < pas ; i++) {
        sonar->update();
        if (sonar->distance < 100) {
            break;
        }
        motor->slowMove(vit);
    }
}
```

it goes forward of a fixed distance linked to the constant *pas* and then it stops. 

If it gets close from an obstacle before reaching the fixed distance, it also stops.

We also have the function *moveToWall* :

```{cpp}
void moveToWall() {
    sonar->setAngle(90);
    sonar->update();
    while (sonar->distance > MARGIN) {
        motor->slowMove(vit);
        sonar->update();
    }
} 
```

It goes forward until it meets an obstacle. 

## Possible improvements

- The body of the robot is too long. It is possible that the sonar passes in front of a wall and that the tail is still in front of the wall. So when it tries to turn, it hits the wall. In order to improve, the sonar should be above the rotation axis, so between the wheels. We tried to stick with a small body robot but could not fit everything on it, and the motors had a too high consumption of energy.
- The moves of the robot when there were no obstacle around him were a bit hazardous. An idea was to make an *init() function* in the class *Brain* which would be lauched by the *setup* in order to find a wall and then follow it. We tried it but we hadn't the time to find a efficient version of this function because the sonar sometimes gets preposterous values.
- The movements of the robot weren't precise. We could have write a calibration routine in the setup function during with the robot would have calibrate himself by finding a wall and rolling toward it and measuring the distance with the sonar.
- With a calibrated robot, we could have mapped the maze in order to find a shorter way later
- We wrote a *getMinimumAngle* function that would have allowed us to find the angle for which the sonar measure the minimal distance. This would have helped to locate the obstacle and get a more precise measure of its distance. We didn't use it because it slowed down the robot

```{cpp}
int getMinimumAngle(int max, int pas) {
    setAngle(max);
    longUpdate();
    float dist = distance;
    int res = max;
    for (int a = max - pas; a >= 0; a -= pas) {
        setAngle(a);
        longUpdate();
        if (distance < dist) {
        res = a;
        dist = distance;
        }
    }
    setAngle(res);
    return res;
}
```


