#include <ArduinoJson.h>
#include <LWiFi.h>

const char* ssid     = "Jennifer iphone";           // insert your SSID
const char* pass     = "aaaa4444";   // insert your password
int keyIndex = 0;  

WiFiClient client;

const int buttonPin = 6;  
const int ledPin =  7;  
int buttonState = 0; 

const char* server1 = "opendata.epa.gov.tw";  
const char* resource1 = "/ws/Data/ATM00625/?$format=json"; 
const char* server2 = "maker.ifttt.com"; 
const char* resource2 = "/trigger/lineha/with/key/ooMaPx9iVAHa5ljcb602K6jKp7kLodSsrKqxIJ3BPDS?value1=";
char json[12000]; 

void setup() {  
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println();
}

void loop() {
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    digitalWrite(ledPin, HIGH);
    Serial.println("Starting connection to PM2.5 server...");
    // connect to server  
    while(!client.connect(server1, 80)){
      // wait for connecting server...
    }
  
    //Send request to server
    client.print("GET ");
    client.print(resource1);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(server1);
    client.println("Connection: close");
    client.println();
    delay(100);
    client.setTimeout(10000);
    while(!client.find("\r\n\r\n")){
    // wait for finishing header stream reading ...
    }
    client.readBytes(json, 12000);
    DynamicJsonBuffer jsonBuffer;
    JsonArray& rootArray = jsonBuffer.parseArray(json);
    JsonObject& guTingSite = rootArray[62];
    const char* site0 = guTingSite["Site"];
    double pm250 = guTingSite["PM25"];
    client.stop();
    delay(2000);
    Serial.println("Starting connection to IFTTT server...");
    while(!client.connect(server2,80)){
    }
    client.print("GET https://");
    client.print(server2);
    client.print(resource2);
    client.println(pm250);
    delay(1000);
    client.stop();
  } else {
    digitalWrite(ledPin, LOW);
  }
}


