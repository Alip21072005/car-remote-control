#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// === Pin Motor & Aksesoris ===
#define r_pwm 33
#define l_pwm 32
#define RF 13
#define LF 26
#define RB 12
#define LB 27
#define BUZZER 25
#define FRONT_LIGHT 14
#define REAR_LIGHT 15

// === OLED Display ===
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// === Konfigurasi WiFi ===
const char* ssid = "RoboCar_AP";
const char* password = "12345678";
WebServer server(80);
int speed = 150;

// === Variabel Animasi Mata ===
int ref_eye_height = 24;
int ref_eye_width = 24;
int ref_space_between_eye = 20;
int ref_corner_radius = 6;
int left_eye_height = ref_eye_height;
int left_eye_width = ref_eye_width;
int left_eye_x = SCREEN_WIDTH / 2 - ref_eye_width - ref_space_between_eye / 2;
int left_eye_y = SCREEN_HEIGHT / 2;
int right_eye_x = SCREEN_WIDTH / 2 + ref_eye_width + ref_space_between_eye / 2;
int right_eye_y = SCREEN_HEIGHT / 2;
int right_eye_height = ref_eye_height;
int right_eye_width = ref_eye_width;

// === Fungsi Animasi Mata ===
void draw_eyes(bool update = true) {
  display.clearDisplay();
  int x = left_eye_x - left_eye_width / 2;
  int y = left_eye_y - left_eye_height / 2;
  display.fillRoundRect(x, y, left_eye_width, left_eye_height, ref_corner_radius, SSD1306_WHITE);
  x = right_eye_x - right_eye_width / 2;
  y = right_eye_y - right_eye_height / 2;
  display.fillRoundRect(x, y, right_eye_width, right_eye_height, ref_corner_radius, SSD1306_WHITE);
  if (update) display.display();
}

void center_eyes(bool update = true) {
  left_eye_x = SCREEN_WIDTH / 2 - ref_eye_width - ref_space_between_eye / 2;
  right_eye_x = SCREEN_WIDTH / 2 + ref_eye_width + ref_space_between_eye / 2;
  left_eye_y = right_eye_y = SCREEN_HEIGHT / 2;
  draw_eyes(update);
}

void blink(int blink_speed = 6) {
  for (int i = 0; i < 3; i++) {
    left_eye_height -= blink_speed;
    right_eye_height -= blink_speed;
    draw_eyes();
    delay(20);
  }
  for (int i = 0; i < 3; i++) {
    left_eye_height += blink_speed;
    right_eye_height += blink_speed;
    draw_eyes();
    delay(20);
  }
}

void happy_eye() {
  center_eyes(false);
  display.fillCircle(left_eye_x, left_eye_y + 6, 4, SSD1306_BLACK);
  display.fillCircle(right_eye_x, right_eye_y + 6, 4, SSD1306_BLACK);
  display.display();
}

// === Fungsi Motor ===
void stopC() {
  digitalWrite(RF, LOW); digitalWrite(LF, LOW);
  digitalWrite(RB, LOW); digitalWrite(LB, LOW);
  analogWrite(l_pwm, 0); analogWrite(r_pwm, 0);
  center_eyes();
}

// === Web Server ===
void setupServer() {
  server.on("/", []() {
    server.send(200, "text/html",
      "<h2>RoboCar Controller</h2>"
      "<button onclick=\"fetch('/forward')\">Maju</button>"
      "<button onclick=\"fetch('/backward')\">Mundur</button><br><br>"
      "<button onclick=\"fetch('/left')\">Kiri</button>"
      "<button onclick=\"fetch('/right')\">Kanan</button><br><br>"
      "<button onclick=\"fetch('/stop')\">Stop</button><br><br>"
      "<button onclick=\"fetch('/buzzer/on')\">Buzzer ON</button>"
      "<button onclick=\"fetch('/buzzer/off')\">Buzzer OFF</button><br><br>"
      "<button onclick=\"fetch('/light/front/on')\">Lampu Depan ON</button>"
      "<button onclick=\"fetch('/light/front/off')\">Lampu Depan OFF</button><br><br>"
      "<button onclick=\"fetch('/light/rear/on')\">Lampu Belakang ON</button>"
      "<button onclick=\"fetch('/light/rear/off')\">Lampu Belakang OFF</button>");
  });

  server.on("/forward", []() {
    digitalWrite(RF, HIGH); digitalWrite(LF, HIGH);
    digitalWrite(RB, LOW); digitalWrite(LB, LOW);
    analogWrite(r_pwm, speed); analogWrite(l_pwm, speed);
    happy_eye();
    server.send(200, "text/plain", "Forward");
  });

  server.on("/backward", []() {
    digitalWrite(RF, LOW); digitalWrite(LF, LOW);
    digitalWrite(RB, HIGH); digitalWrite(LB, HIGH);
    analogWrite(r_pwm, speed); analogWrite(l_pwm, speed);
    blink();
    server.send(200, "text/plain", "Backward");
  });

  server.on("/left", []() {
    digitalWrite(RF, HIGH); digitalWrite(LF, HIGH);
    digitalWrite(RB, LOW); digitalWrite(LB, LOW);
    analogWrite(r_pwm, 255); analogWrite(l_pwm, 150);
    left_eye_x -= 5; right_eye_x -= 5;
    draw_eyes();
    server.send(200, "text/plain", "Left");
  });

  server.on("/right", []() {
    digitalWrite(RF, HIGH); digitalWrite(LF, HIGH);
    digitalWrite(RB, LOW); digitalWrite(LB, LOW);
    analogWrite(r_pwm, 150); analogWrite(l_pwm, 255);
    left_eye_x += 5; right_eye_x += 5;
    draw_eyes();
    server.send(200, "text/plain", "Right");
  });

  server.on("/stop", []() {
    stopC();
    server.send(200, "text/plain", "Stopped");
  });

  server.begin();
}

// === Setup Utama ===
void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("OLED failed!");
    while (1);
  }
  display.clearDisplay();
  center_eyes();

  pinMode(RF, OUTPUT); pinMode(LF, OUTPUT);
  pinMode(RB, OUTPUT); pinMode(LB, OUTPUT);
  pinMode(r_pwm, OUTPUT); pinMode(l_pwm, OUTPUT);
  pinMode(BUZZER, OUTPUT); pinMode(FRONT_LIGHT, OUTPUT); pinMode(REAR_LIGHT, OUTPUT);
  stopC();

  WiFi.softAP(ssid, password);
  Serial.println("AP IP: " + WiFi.softAPIP().toString());
  setupServer();
}

void loop() {
  server.handleClient();
}
