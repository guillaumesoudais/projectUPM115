#ifndef Brain_h
#define Brain_h

#include <Arduino.h>

int quartTour = 20;
int pas = 50;

class Brain {
  public :
    Soanr sonar;
    Motor motor;
    int memoire = 0;

    Brain(Sonar s, Motor m) {
      sonar = s;
      motor = m;
    }

    void handleWall() {
      sonar->setAngle(0);
      sonar->update();
      if (sonar->distance > 100) {
        sonar->setAngle(90);
        moteur->slowTurn(false, quartTour);
      }
      else {
        sonar->setAngle(180);
        sonar->update();
        if (sonar->distance > 100) {
          sonar->setAngle(90);
          moteur->slowTurn(true, quartTour);
        }
        else {
          sonar->setAngle(90);
          moteur->slowTurn(true, 2 * quartTour);

        }
      }
    }
    void pasEnAvant(() {
      memoire = 0;
      sonar->setAngle(90);
      sonar->update();
      for (int i =  0; i < pas ; i++) {
        sonar->update();
        if (sonar->distance > 100) {
          handleWall();
        }
        slowMove(25);
      }
    }

    
    void mainish() {
      sonar->setAngle(0);
      sonar->update();
      if (sonar->distance > 100) {
        moteur->slowTurn(false, quartTour);
        mmemoire++;
        if (mmemoire == 4) {
          pasEnAvant();
        }
      }
      else {
        pasEnAvant();
      }
    }

}

