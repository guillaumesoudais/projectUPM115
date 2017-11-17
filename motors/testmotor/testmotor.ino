int enA = 10;
int enB = 9;//prob
int fwdA = 4;
int bwdA = 5;
int fwdB = 6;
int bwdB = 7;
int speed = 128;



void move(int speed, boolean forward = true) {
  
  analogWrite(enB, speed);
  analogWrite(enA, speed);
  digitalWrite(fwdA, forward);
  digitalWrite(fwdB, forward);
  digitalWrite(bwdA, !forward);
  digitalWrite(bwdB, !forward);
}

void slowMove2(int n){
  for (int i = 0; i<n;i++){
    move(speed);
    delay(10);
    stop();
    delay(50);
  }
}

void slowMove(int n){
  move(255);
  delay(50);
  move(32);
  delay(1000);
  stop();
  
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

void turnLeft(int speed) {
  turn(true, speed);
}

void turnRight(int speed) {
  turn(false, speed);
}

void setup() {
  // put your setup code here, to run once:
pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(fwdA, OUTPUT);
  pinMode(bwdA, OUTPUT);
  pinMode(fwdB, OUTPUT);
  pinMode(bwdB, OUTPUT);
  
}
void loop() {
  // put your main code here, to run repeatedly:
  slowMove(20);
  delay(1000);
  
}





