#ifndef Brain_h
#define Brain_h

#include <Arduino.h>
#include "sonar.h"
#include "motors.h"

int quartTour = 60;
int smallTour = 6;
int S_ANGLE = 10;
int pas = 100;
int vit = 3;
int MARGIN = 100;
int MARGIN_RIGHT = 60;
int MARGIN_FRONT = 100;

class Brain {
  public :
    Sonar *sonar;
    Motors *motor;
    int counter;


    Brain(Sonar *s, Motors *m) {
      sonar = s;
      motor = m;
    }

    void handleWall() {
      sonar->setAngle(0);
      delay(200);
      sonar->update();
      if (sonar->distance > 100) {
        sonar->setAngle(90);
        delay(200);
        motor->turnRight( vit, quartTour);
      }
      else {
        sonar->setAngle(180);
        delay(200);
        sonar->update();
        if (sonar->distance > 100) {
          sonar->setAngle(90);
          delay(200);
          motor->turnLeft( vit, quartTour);
        }
        else {
          sonar->setAngle(90);
          delay(200);
          motor->turnLeft( vit, 2 * quartTour);

        }
      }
    }
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

    void moveToWall() {
      sonar->setAngle(90);
      while (sonar->distance > MARGIN) {
        motor->slowMove(vit);
      }
    } 

    void mainish() {
      sonar->setAngle(0);
      sonar->update();
      if (sonar->distance > MARGIN) {
        motor->turnRight(1, quartTour);
        sonar->update();
        if (sonar->distance < MARGIN) {
          moveToWall();
        }
        else {
          pasEnAvant();
        }
      }
      else {
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

    void followWall() {
      sonar->setAngle(40);
      sonar->update();
      counter = 0;
      for (int i=0; i<pas; i++) {
        if (sonar->distance > MARGIN_RIGHT) {
          motor->moveRight(vit + 1);
          motor->moveLeft(vit);
          counter ++;
        }
        else {
          motor->moveRight(vit);
          motor->moveLeft(vit+1);
          counter --;
        }
      }
    }

    void reorientation() {
      sonar->setAngle(90);
      sonar->update();
      while (sonar->distance < MARGIN_FRONT) {
        motor->moveLeft(2);
        sonar->update();
      }
    }


    void mainish2() {
      followWall();
      if (counter == pas) {
        moveToWall();
      }
      else if (counter == -pas) {
        motor.slowMove(1, false);
        reorientation();
      }
    }

};

#endif

