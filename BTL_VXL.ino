#include "WiFi.h"
#include <FirebaseESP32.h>

#define RELAY1 27 
#define RELAY2 13 
#define TOUCH1 14 
#define TOUCH2 26
#define FIREBASE_HOST "https://fir-e15b0.firebaseio.com/"
#define FIREBASE_AUTH "orWcGoszOfgSzhQq3YmjeIqf7hbUqsA8Wl1IPHXd"

FirebaseData firebaseData;
FirebaseJson json;
String relayFirebase1, relayFirebase2, path;  

void setup() {
  Serial.begin(115200);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(TOUCH1, INPUT);
  pinMode(TOUCH2, INPUT);
  WiFi.mode(WIFI_AP_STA);
  WiFi.beginSmartConfig();
  Serial.println("Waiting for SmartConfig.");
  while (!WiFi.smartConfigDone()) {
     vTaskDelay(1000 / portTICK_PERIOD_MS);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("SmartConfig received.");
  Serial.println("Waiting for WiFi");
  while (WiFi.status() != WL_CONNECTED) {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
    Serial.print(".");
  }
  Serial.println("WiFi Connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.SSID());
  Serial.println(WiFi.psk());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
}
void sendFeedbackToFirebase1(String dataForRelay1){
  Firebase.setString(firebaseData, path + "/State/Machine1", dataForRelay1);
  }
void sendFeedbackToFirebase2(String dataForRelay1){
  Firebase.setString(firebaseData, path + "/State/Machine2", dataForRelay1);
  }  
void handleData(String data1, String data2){
  if(data1 == "ON"){
    digitalWrite(RELAY1, HIGH);
    }
  if(data1 == "OFF"){
    digitalWrite(RELAY1, LOW);
    }
  if(data2 == "ON"){
    digitalWrite(RELAY2, HIGH);
    }
  if(data2 == "OFF"){
    digitalWrite(RELAY2, LOW);
    }
  }
void process(String data1, String data2){
  int stateTouch1 = digitalRead(TOUCH1);
  int stateTouch2 = digitalRead(TOUCH2);
  if(data1 == "ON" && stateTouch1 == HIGH){
    digitalWrite(RELAY1, LOW);
    sendFeedbackToFirebase1("OFF");
    }
  if(data1 == "OFF" && stateTouch1 == HIGH){
    digitalWrite(RELAY1, HIGH);
    sendFeedbackToFirebase1("ON");
    }
  if(data2 == "ON" && stateTouch2 == HIGH){
    digitalWrite(RELAY2, LOW);
    sendFeedbackToFirebase2("OFF");
    }
  if(data2 == "OFF" && stateTouch2 == HIGH){
    digitalWrite(RELAY2, HIGH);
    sendFeedbackToFirebase2("ON");
    }
  } 
  
void loop() {
  Firebase.getString(firebaseData, path + "/State/Machine1", relayFirebase1);
  Firebase.getString(firebaseData, path + "/State/Machine2", relayFirebase2);
  Serial.println(relayFirebase1);
  Serial.println(relayFirebase2);
  handleData(relayFirebase1, relayFirebase2);
  process(relayFirebase1, relayFirebase2);
}
