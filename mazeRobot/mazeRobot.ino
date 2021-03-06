
#include <Servo.h>
#include "sonar.h"
#include "motors.h"
#include "brain.h"

Motors *motors;
Sonar *sonar;
Brain *brain;
int enA = 10;
int enB = 9;
int fwdA = 4;
int bwdA = 5;
int fwdB = 6;
int bwdB = 7;
int startSpeed = 128;
Servo myServo;
int pinServo = 8;
int trigger = 2;
int echo = 3;
boolean isRolling = false;
int time;
int delta = 500;

void setup() {
  motors = new Motors(enA, fwdA, bwdA, enB, fwdB, bwdB);
  sonar = new Sonar(myServo, pinServo, trigger, echo);
  brain = new Brain(sonar, motors);


  pinMode(trigger, OUTPUT);
  digitalWrite(trigger, LOW);
  pinMode(echo, INPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(fwdA, OUTPUT);
  pinMode(fwdB, OUTPUT);
  pinMode(bwdA, OUTPUT);
  pinMode(bwdB, OUTPUT);
  pinMode(pinServo, OUTPUT);
}

void loop() {
  brain->mainish();

}

