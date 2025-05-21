#include <BluetoothSerial.h> 
BluetoothSerial ESP_BT;

#define r_pwm 33
#define l_pwm 32
#define RF 13
#define LF 26
#define RB 12
#define LB 27

int incoming;
int speed = 150;

void stopC(){
  digitalWrite(RF, LOW);
  digitalWrite(LB, LOW);
  digitalWrite(LF, LOW);
  digitalWrite(RB, LOW);                     
}

void setup(){
  Serial.begin(9600);
  ESP_BT.begin("roboS");

  pinMode(RF, OUTPUT);
  pinMode(LF, OUTPUT);
  pinMode(LB, OUTPUT);
  pinMode(RB, OUTPUT);
  digitalWrite(RF, LOW);
  digitalWrite(LB, LOW);
  digitalWrite(LF, LOW);
  digitalWrite(RB, LOW);    
}

void loop(){
  if(ESP_BT.available()){
    incoming = ESP_BT.read();

    if(incoming == 'S'){  // stop all
      stopC(); 
    }
    if(incoming == 'B'){ // back
      digitalWrite(RF,LOW);
      digitalWrite(LB,HIGH);
      digitalWrite(LF,LOW);
      digitalWrite(RB,HIGH);
      analogWrite(l_pwm,speed);
      analogWrite(r_pwm,speed);                    
    }
    if(incoming == 'F'){ // forward
      digitalWrite(RF, HIGH);
      digitalWrite(LB, LOW);
      digitalWrite(LF, HIGH);
      digitalWrite(RB, LOW);  
      analogWrite(l_pwm, speed);
      analogWrite(r_pwm, speed);  
    }
    if (incoming == 'H'){ // back left
      digitalWrite(RF, HIGH);
      digitalWrite(LB, HIGH);
      digitalWrite(LF, LOW);
      digitalWrite(RB, LOW);  
      analogWrite(l_pwm, speed);
      analogWrite(r_pwm, speed);                      
    }
    if(incoming == 'L'){ // left
      digitalWrite(RF, HIGH);
      digitalWrite(LB, LOW);
      digitalWrite(LF, HIGH);
      digitalWrite(RB, LOW);  
      analogWrite(l_pwm, 150);
      analogWrite(r_pwm, 255);                      
    }
    if(incoming == 'G'){  // forward left
      digitalWrite(RF,HIGH);
      digitalWrite(LB,LOW);
      digitalWrite(LF,HIGH);
      digitalWrite(RB,LOW);  
      analogWrite(l_pwm,150);
      analogWrite(r_pwm,255);                      
    }
    if(incoming == 'J'){ // back right
      digitalWrite(RF,LOW);
      digitalWrite(LB,LOW);
      digitalWrite(LF,HIGH);
      digitalWrite(RB,HIGH);    
      analogWrite(l_pwm,speed);
      analogWrite(r_pwm,speed);                    
    }
    if(incoming == 'R'){ // right
      digitalWrite(RF,HIGH);
      digitalWrite(LB,LOW);
      digitalWrite(LF,HIGH);
      digitalWrite(RB,LOW);    
      analogWrite(l_pwm,255);
      analogWrite(r_pwm,150);                    
    }
    if (incoming == 'I'){ // forward right
      digitalWrite(RF,HIGH);
      digitalWrite(LB,LOW);
      digitalWrite(LF,HIGH);
      digitalWrite(RB,LOW);    
      analogWrite(l_pwm,255);
      analogWrite(r_pwm,190);                    
    }          
    if(incoming == '1'){
      speed = 150;
    }
    if(incoming == '2'){
      speed = 160;
    }
    if(incoming == '3'){
      speed = 170;
    }
    if(incoming == '4'){
      speed = 180;
    }
    if(incoming == '5'){
      speed = 190;
    }
    if(incoming == '6'){
      speed = 200;
    }
    if(incoming == '7'){
      speed = 210;
    }
    if(incoming == '8'){
      speed = 220;
    }
    if(incoming == '9'){
      speed = 235;
    }
    if(incoming == 'q'){
      speed = 255;
    }    
  }
}
