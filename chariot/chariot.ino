int mg = A6;
int md = A7;
int vitesse = 255;

void setup() {
  // put your setup code here, to run once:
  
}

void loop() {
  // put your main code here, to run repeatedly:
  straight();
}

void straight(){
  analogWrite(mg, vitesse);
  analogWrite(md, vitesse);
}

