#ifndef Brain_h
#define Brain_h

#include <Arduino.h>
#include "sonar.h"
#include "motors.h"

int quartTour = 60;
int pas = 100;
int vit = 3;

class Brain {
  public :
    Sonar *sonar;
    Motors *motor;
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
          handleWall();
          break;
        }
        motor->slowMove(vit);
      }
    }


    void mainish() {
      sonar->setAngle(0);
      sonar->update();
      if (sonar->distance < 100) {
          pasEnAvant();
      }
      else {
        sonar->setAngle(90);
        if (sonar->distance < 100) {
          handleWall();
        }
        else{

          
        }
      }
    }

};

#endif

