#ifndef Motors_h
#define Motors_h

#include <Arduino.h>


class Motors {
  public :
    int enA;    // enable for motor A (left)
    int fwdA;   // forward pin for motor A
    int bwdA;   // backward pin for motor A

    int enB;    // enable for second motor B (right)
    int fwdB;   // forward pin for motor B
    int bwdB;    // backward pin for motor B

    int nbSpeed; // number of available speed

    Motors(int ea, int fa, int ba, int eb, int fb, int bb) {
      enA = ea;
      fwdA = fa;
      bwdA = ba;
      enB = eb;
      fwdB = fb;
      bwdB = bb;
    }

    void moveLeft(int speed, boolean forward = true) {
      analogWrite(enA, speed);
      digitalWrite(bwdA, !forward);      
      digitalWrite(fwdA, forward);
    }
    
    void moveRight(int speed, boolean forward=true) {
      analogWrite(enB, speed);
      digitalWrite(fwdB, forward);      
      digitalWrite(bwdB, !forward);
    }

    void move(int speed, boolean forward = true) {
      moveLeft(speed, forward);
      moveRight(speed, forward);
    }

    void stop() {
      analogWrite(enA, 0);
      analogWrite(enB, 0);
      digitalWrite(fwdA, 0);
      digitalWrite(fwdB, 0);
      digitalWrite(bwdA, 0);
      digitalWrite(bwdB, 0);
    }

    void turn(boolean left, int speed) {
      analogWrite(enA, speed);
      analogWrite(enB, speed);
      digitalWrite(fwdA, !left);
      digitalWrite(fwdB, left);
      digitalWrite(bwdA, left);
      digitalWrite(bwdB, !left);
    }

    void turnLeft(int n, int tps) {
      for (int i = 0; i < tps; i++) {
        turn(true, 255);
        delay(n);
        stop();
        delay(6 - n);
      }
    }

    void turnRight(int n, int tps) {
      for (int i = 0; i < tps; i++) {
        turn(false, 255);
        delay(n);
        stop();
        delay(6 - n);
      }
    }

    void slowMove(int n,boolean forward=true) {
      move(255, forward);
      delay(n);
      stop();
      delay(6 - n);
    }

    void slowMoveLeft(int n) {
      moveLeft(255);
      delay(n);
      stop();
      delay(6 - n);
    }

    void slowMoveRight(int n) {
      moveRight(255);
      delay(n);
      stop();
      delay(6 - n);
    }

};


#endif
