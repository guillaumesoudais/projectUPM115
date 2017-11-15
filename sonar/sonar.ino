/* 
 * Code d'exemple pour un capteur à ultrasons HC-SR04.
 */
#include <Servo.h>

class Sonar {
  /*
   * Class for the Sonar with its servo and its
   * ultrasonic sensor
   */

  public int angle;        // angle of the servo
  public int distance;     // distance of the obstacle in milimeters
  Servo sonarServo; // Servo object
  int pinServo;     // pin of the Servo
  int triggerPin;   // pin of the trigger for the sensor
  int echoPin;      // pin of the echo (response) of the sensor

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

  /*
   * Updates the orientation of the sensor
   * and the distance of the obstacle
   */
  public void update() {
    angle = sonarServo.read();
    
    /* sending the instructon to measure the distance */
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    
    /* measurin time between sending and receiving of the impulsion */
    long measure = pulseIn(ECHO_PIN, HIGH, MEASURE_TIMEOUT);
    
    /* calculating the distance */
    distance = measure / 2.0 * SOUND_SPEED;
  }

  /*
   * Sets the angle of the sensor
   */
  public void setAngle(int newAngle) {
    sonarServo.write(newAngle);
  }
}
