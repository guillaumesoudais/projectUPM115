
#include <Servo.h>

class Motors {
  public :
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
      digitalWrite(fwdA, forward);
      digitalWrite(fwdB, forward);
      digitalWrite(bwdA, !forward);
      digitalWrite(bwdB, !forward);
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

    void slowturn(boolean l, int s) {
      turn(t,s);
      delay(5);
      stop();
      delay(25)
    }

    void turnLeft(int speed) {
      slowturn(true, speed);
    }

    void turnRight(int speed) {
      slowturn(false, speed);
    }

    void slowMove(int n) {
      move(255);
      delay(5);
      stop();
      delay(n);
    }
};

class Sonar {
    /*
       Class for the Sonar with its servo and its
       ultrasonic sensor
    */

  public :
    int angle;        // angle of the servo
    float distance;     // distance of the obstacle in milimeters
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
      Updates the orientation of the sensor
      and the distance of the obstacle
    */
    void update() {
      angle = sonarServo.read();

      /* sending the instructon to measure the distance */
      digitalWrite(triggerPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(triggerPin, LOW);

      /* measurin time between sending and receiving of the impulsion */
      long measure = pulseIn(echoPin, HIGH, MEASURE_TIMEOUT);

      /* calculating the distance */
      distance = measure / 2.0 * SOUND_SPEED;
    }

    /*
      Sets the angle of the sensor
    */
    void setAngle(int newAngle) {
      sonarServo.write(newAngle);
    }
};

Motors *motors;
Sonar *sonar;
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
  sonar->update();
  if (sonar->distance < 100) {
    motors->stop();
  }
  else {
    motors->slowMove(25);
  }

}

