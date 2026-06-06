#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASS";

const char* mqtt_server = "192.168.1.10";

WiFiClient espClient;
PubSubClient client(espClient);

int motorFL = 13;
int motorFR = 12;
int motorBL = 14;
int motorBR = 27;

void callback(char* topic, byte* payload, unsigned int length) {
  String cmd = "";

  for (int i = 0; i < length; i++) {
    cmd += (char)payload[i];
  }

  Serial.println(cmd);

  if (cmd == "STOP") {
    analogWrite(motorFL, 0);
    analogWrite(motorFR, 0);
    analogWrite(motorBL, 0);
    analogWrite(motorBR, 0);
  }

  if (cmd == "FORWARD") {
    analogWrite(motorFL, 700);
    analogWrite(motorFR, 700);
    analogWrite(motorBL, 800);
    analogWrite(motorBR, 800);
  }
}

void reconnect() {
  while (!client.connected()) {
    client.connect("AI_DRONE");
    client.subscribe("drone/control");
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(motorFL, OUTPUT);
  pinMode(motorFR, OUTPUT);
  pinMode(motorBL, OUTPUT);
  pinMode(motorBR, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();
}
