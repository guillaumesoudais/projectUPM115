/*
   Code d'exemple pour un capteur à ultrasons HC-SR04.
*/
#ifndef Sonar_h
#define Sonar_h

#include <Arduino.h>

#include <Servo.h>

const int LENGTH = 15;

class Sonar {
    /*
       Class for the Sonar with its servo and its
       ultrasonic sensor
    */

  public :
    int angle;        // angle of the servo
    float distance;    // distance of the obstacle in milimeters
    float mean;
    Servo sonarServo; // Servo object
    int pinServo;     // pin of the Servo
    int triggerPin;   // pin of the trigger for the sensor
    int echoPin;      // pin of the echo (response) of the sensor
    float distanceList[LENGTH];
    int index = 0;

    /* constant for the timeout */
    const unsigned long MEASURE_TIMEOUT = 25000UL; // 25ms = ~8m à 340m/s

    /* sound speed */
    const float SOUND_SPEED = 340.0 / 1000;

    Sonar(Servo myServo, int pinS, int trigger, int echo) {
      sonarServo = myServo;
      pinServo = pinS;
      sonarServo.attach(pinServo);
      angle = sonarServo.read();
      triggerPin = trigger;
      echoPin = echo;
    }

    void longUpdate() {
      for (int i=0; i<LENGTH; i++) {
        update();
      }
    }
    
    
    /*
      Updates the orientation of the sensor
      and the distance of the obstacle
    */
    void update() {
      angle = sonarServo.read();

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
    }

    /*
      Sets the angle of the sensor
    */
    void setAngle(int na ) {
      sonarServo.write(na);
      delay(100);
    }


    int getMinimumAngle() {
      setAngle(180);
      int a = 180;
      update();
      int tmp;
      while (distance <= tmp && a>0) {
        a--;
        tmp = distance;
        setAngle(a);
        update();
      }
      setAngle(a+1);
      update();
      return a+1;
    }
};

#endif
