#ifndef Motors_h
#define Motors_h

#include <Arduino.h>

void setup() {}

void loop(){}


class Motors {
    int enA;    // enable for motor A (left)
    int fwdA;   // forward pin for motor A
    int bwdA;   // backward pin for motor A

    int enB;    // enable for second motor B (right)
    int fwdB;   // forward pin for motor B
    int bwdB;    // backward pin for motor B

    Motors(int ea, int fa, int ba, int eb, int fb, int bb) {
      enA = ea;
      fwdA = fa;
      bwdA = ba;
      enB = eb;
      fwdB = fb;
      bwdB = bb;
    }

    void move(int speed, boolean forward = true) {
      analogWrite(enA, speed);
      analogWrite(enB, speed);
      analogWrite(fwdA, forward);
      analogWrite(fwdB, forward);
      analogWrite(bwdA, !forward);
      analogWrite(bwdB, !forward);
    }

    void stop() {
      analogWrite(enA, 0);
      analogWrite(enB, 0);
      analogWrite(fwdA, 0);
      analogWrite(fwdB, 0);
      analogWrite(bwdA, 0);
      analogWrite(bwdB, 0);
    }

    void turn(boolean left, int speed) {
      analogWrite(enA, speed);
      analogWrite(enB, speed);
      analogWrite(fwdA, !left);
      analogWrite(fwdB, left);
      analogWrite(bwdA, left);
      analogWrite(bwdB, !left);
    }

    void turnLeft(int speed) {
      turn(true, speed);
    }

    void turnRight(int speed) {
      turn(false, speed);
    }

    void slowMove(int s) {
      move(128);
      delay(20);
      move(s);
    }
};

#endif
