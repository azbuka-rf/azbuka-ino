#include "WiFi.h"
#include "chernozem.h"

#define stpEnable 1

#define xStp 2
#define xDir 3
#define xEnd 4

#define yStp 5
#define yDir 6
#define yEnd 7

const char* ssid = "vika";
const char* password = "korovin228";

const char* host = "192.168.43.185";
const uint16_t port = 12343;

WiFiClient tcp;
chernozem ser(&Serial);

void serLog(String msg) {
  ser << "[azbuka] Log: " << msg << "\n";
}

void sendTCP(String msg) {
  
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

  pinMode(stpEnable, OUTPUT);
  digitalWrite(stpEnable, LOW);
  serLog("Steppers initialized");

  // ==== TCP ==== 
  while(!tcp.connect(host, port)) {
    serLog("Connecting to server...");
    delay(100);
  }
  serLog("Connected to server");
}
bool zzz = 0;
void loop() {
  if(WiFi.status() != WL_CONNECTED || !tcp.connected()) {
    setup();
  }

  if(!zzz) {
    tcp.println();
    zzz = 1;
  }

  while (tcp.available()) {
    char c = tcp.read();
    Serial.println(c);
    zzz = 0;
  }
  // if(Serial.available()) {
  //   String cmd; ser >> cmd;
  //   if(cmd == "send") {
  //     int x, y, t; ser >> x >> y >> t;
  //     sendTCP(String("M ") + x + " " + y + " " + t);
  //   }
  // }
  
}






































