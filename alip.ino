#include <BluetoothSerial.h>
BluetoothSerial ESP_BT;

#define r_pwm 33
#define l_pwm 32
#define RF 13
#define LF 26
#define RB 12
#define LB 27

#define BUZZER 25
#define FRONT_LIGHT 14  // Hindari pin input-only seperti 34
#define REAR_LIGHT 15

int incoming;
int speed = 150;

unsigned long lastCommandTime = 0;
const unsigned long timeout = 1000; // 1 detik
bool isMoving = false;

void stopC() {
  digitalWrite(RF, LOW);
  digitalWrite(LF, LOW);
  digitalWrite(RB, LOW);
  digitalWrite(LB, LOW);
  analogWrite(l_pwm, 0);
  analogWrite(r_pwm, 0);
}

void setup() {
  Serial.begin(9600);
  ESP_BT.begin("roboS");

  pinMode(RF, OUTPUT);
  pinMode(LF, OUTPUT);
  pinMode(LB, OUTPUT);
  pinMode(RB, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(FRONT_LIGHT, OUTPUT);
  pinMode(REAR_LIGHT, OUTPUT);

  stopC();
  digitalWrite(BUZZER, LOW);
  digitalWrite(FRONT_LIGHT, LOW);
  digitalWrite(REAR_LIGHT, LOW);
}

void loop() {
  if (ESP_BT.available()) {
    incoming = ESP_BT.read();
    lastCommandTime = millis(); // Reset timer
    isMoving = true;

    switch (incoming) {
      case 'S': stopC(); isMoving = false; break;
      case 'F':
        digitalWrite(RF, HIGH); digitalWrite(LB, LOW);
        digitalWrite(LF, HIGH); digitalWrite(RB, LOW);
        analogWrite(l_pwm, speed);
        analogWrite(r_pwm, speed);
        break;
      case 'B':
        digitalWrite(RF, LOW); digitalWrite(LB, HIGH);
        digitalWrite(LF, LOW); digitalWrite(RB, HIGH);
        analogWrite(l_pwm, speed);
        analogWrite(r_pwm, speed);
        break;
      case 'L':
        digitalWrite(RF, HIGH); digitalWrite(LB, LOW);
        digitalWrite(LF, HIGH); digitalWrite(RB, LOW);
        analogWrite(l_pwm, 150);
        analogWrite(r_pwm, 255);
        break;
      case 'R':
        digitalWrite(RF, HIGH); digitalWrite(LB, LOW);
        digitalWrite(LF, HIGH); digitalWrite(RB, LOW);
        analogWrite(l_pwm, 255);
        analogWrite(r_pwm, 150);
        break;
      case 'G': // Forward Left
        digitalWrite(RF, HIGH); digitalWrite(LB, LOW);
        digitalWrite(LF, HIGH); digitalWrite(RB, LOW);
        analogWrite(l_pwm, 150);
        analogWrite(r_pwm, 255);
        break;
      case 'I': // Forward Right
        digitalWrite(RF, HIGH); digitalWrite(LB, LOW);
        digitalWrite(LF, HIGH); digitalWrite(RB, LOW);
        analogWrite(l_pwm, 255);
        analogWrite(r_pwm, 190);
        break;
      case 'H': // Back Left
        digitalWrite(RF, HIGH); digitalWrite(LB, HIGH);
        digitalWrite(LF, LOW); digitalWrite(RB, LOW);
        analogWrite(l_pwm, speed);
        analogWrite(r_pwm, speed);
        break;
      case 'J': // Back Right
        digitalWrite(RF, LOW); digitalWrite(LB, LOW);
        digitalWrite(LF, HIGH); digitalWrite(RB, HIGH);
        analogWrite(l_pwm, speed);
        analogWrite(r_pwm, speed);
        break;
      case 'W': digitalWrite(FRONT_LIGHT, HIGH); break;
      case 'w': digitalWrite(FRONT_LIGHT, LOW); break;
      case 'U': digitalWrite(REAR_LIGHT, HIGH); break;
      case 'u': digitalWrite(REAR_LIGHT, LOW); break;
      case 'V': digitalWrite(BUZZER, HIGH); break;
      case 'v': digitalWrite(BUZZER, LOW); break;
      case '1': speed = 150; break;
      case '2': speed = 160; break;
      case '3': speed = 170; break;
      case '4': speed = 180; break;
      case '5': speed = 190; break;
      case '6': speed = 200; break;
      case '7': speed = 210; break;
      case '8': speed = 220; break;
      case '9': speed = 235; break;
      case 'q': speed = 255; break;
    }
  }

  // Auto-stop jika tidak ada perintah selama 1 detik
  if (isMoving && (millis() - lastCommandTime > timeout)) {
    stopC();
    isMoving = false;
  }
}
