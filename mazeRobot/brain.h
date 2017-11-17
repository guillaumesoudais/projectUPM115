#ifndef Brain_h
#define Brain_h

#include <Arduino.h>
#include "sonar.h"
#include "motors.h"

int quartTour = 60;
int smallTour = 6;
int S_ANGLE = 10;
int pas = 50;
int vit = 3;
int MARGIN = 100;
int MARGIN_RIGHT = 80;
int MARGIN_FRONT = 100;

class Brain {
  public :
    Sonar *sonar;
    Motors *motor;
    int counter = 0;


    Brain(Sonar *s, Motors *m) {
      sonar = s;
      motor = m;
    }

    void handleWall() {
      sonar->setAngle(0);
      delay(200);
      sonar->longUpdate();
      if (sonar->distance > 100) {
        sonar->setAngle(90);
        delay(200);
        motor->turnRight( vit, quartTour);
      }
      else {
        sonar->setAngle(180);
        delay(200);
        sonar->longUpdate();
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
      sonar->update();
      while (sonar->distance > MARGIN) {
        motor->slowMove(vit);
        sonar->update();
      }
    } 

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

};

#endif

