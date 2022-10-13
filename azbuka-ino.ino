#include <WiFi.h>
#include <Adafruit_PCF8574.h>
#include "chernozem.h"
#include "chernozemTCP.h"
#include "azbuka.h"

#define Delay 150

#define half 17

#define stpEn 1

#define xStp 2
#define xDir 3

#define yStp 5
#define yDir 6

#define xEnd 4
#define yEnd 7

#define mgEn 8

struct cord {
  int x,y;
};
const int xSq[] = {};
const int ySq[] = {};

int X, Y;

const char* ssid = "vika";
const char* password = "korovin228";

const char* host = "192.168.43.185";
const uint16_t port = 1234;

WiFiClient _tcp;
chernozem ser(&Serial);
chernozemTCP tcp(&_tcp);

Adafruit_PCF8574 pcfX;
Adafruit_PCF8574 pcfY;

void serLog(String msg) {
  ser << "[azbuka] Log: " << msg << "\n";
}

void goHome() {
  digitalWrite(yDir, LOW);
  digitalWrite(xDir, HIGH);
  while(digitalRead(xEnd)) {
    digitalWrite(yStp, HIGH);
    digitalWrite(xStp, HIGH);
    digitalWrite(yStp, LOW);
    digitalWrite(xStp, LOW);
    delayMicroseconds(Delay);
  }
  X = 0;

  digitalWrite(yDir, HIGH);
  digitalWrite(xDir, HIGH);
  while(digitalRead(yEnd)) {
    digitalWrite(yStp, HIGH);
    digitalWrite(xStp, HIGH);
    digitalWrite(yStp, LOW);
    digitalWrite(xStp, LOW);
    delayMicroseconds(Delay);
  }
  Y = 0;
}

void movePerX(int mm) {
  int steps = mm * 200 * 16 / 2 / 20;
  if(steps < 0) {
    steps = -steps;
    digitalWrite(yDir, LOW);
    digitalWrite(xDir, HIGH);
  } else {
    digitalWrite(yDir, HIGH);
    digitalWrite(xDir, LOW);
  }
  while(steps--) {
    digitalWrite(yStp, HIGH);
    digitalWrite(xStp, HIGH);
    digitalWrite(yStp, LOW);
    digitalWrite(xStp, LOW);
    delayMicroseconds(Delay);
  }
  X += mm;
}

void movePerY(int mm) {
  int steps = mm * 200 * 16 / 2 / 20;
  if(steps < 0) {
    steps = -steps;
    digitalWrite(yDir, HIGH);
    digitalWrite(xDir, HIGH);
  } else {
    digitalWrite(yDir, LOW);
    digitalWrite(xDir, LOW);
  }
  while(steps--) {
    digitalWrite(yStp, HIGH);
    digitalWrite(xStp, HIGH);
    digitalWrite(yStp, LOW);
    digitalWrite(xStp, LOW);
    delayMicroseconds(Delay);
  }
  Y += mm; 
}

void goTo(int x, int y) {
  movePerX(x-X);
  movePerY(y-Y);
}

void goTo(cord A) {
  goTo(A.x, A.y);  
}

void setup() {
  // ==== Serial ==== 
  Serial.begin(115200);
  serLog("Serial initialized");

  // ==== WiFi ==== 
  WiFi.hostname("Azbuka S/N 1");
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    serLog("Connecting to WiFi...");
    delay(500);
  }
  serLog("Connected to WiFi");
  serLog("Log: Local IP: " + String(WiFi.localIP()));
  serLog("WiFi initialized");

  // ==== Steppers ==== 
  pinMode(xStp, OUTPUT);
  pinMode(xDir, OUTPUT);
  pinMode(xEnd, INPUT_PULLUP);

  pinMode(yStp, OUTPUT);
  pinMode(yDir, OUTPUT);
  pinMode(yEnd, INPUT_PULLUP);

  pinMode(stpEn, OUTPUT);
  digitalWrite(stpEn, LOW);
  serLog("Steppers initialized");
  goHome();

  // ==== Keyboard ====
  if(!pcfX.begin(0x20, &Wire)) {
    Serial.println("Couldn't find pcfX");
    while(1);
  }
  if(!pcfY.begin(0x21, &Wire)) {
    Serial.println("Couldn't find pcfY");
    while(1);
  }

  for(int i = 0; i < 8; ++i) {
    pcfX.pinMode(i, INPUT_PULLUP);    
  }
  for(int i = 0; i < 8; ++i) {
    pcfY.pinMode(i, OUTPUT);
  }

  // ==== TCP ==== 
  while(!_tcp.connect(host, port)) {
    serLog("Connecting to server...");
    delay(100);
  }
  serLog("Connected to server");
}

void loop() {
  if(WiFi.status() != WL_CONNECTED || !_tcp.connected())
    setup();

  while(tcp.available()) {
    String cmd; tcp >> cmd;
    if(cmd == "move") {
      cord s,e; tcp >> s.x >> s.y >> e.x >> e.y;
      goTo(s)
      if(e.x < s.x) {
        stepX(-17);
      } else if(s.x < e.x) {
        stepX(17);
      }

      if(s.y < e.y) {
        goTo(s.x,e.y-17);
      } else if(e.y < s.y) {
        goTo(s.x, e.y+17);
      }

      goTo(e.x,Y);
      goTo(e);
    }
  }
  if(ser.available()) {
    String cmd; ser >> cmd;
    if(cmd == "send") {
      int t, x, y; ser >> t >> x >> y;
      tcp << String("M ") + t + " " + x + " " + y + "\n";
    }
  }
}






































