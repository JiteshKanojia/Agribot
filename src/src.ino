#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
 
const char* ssid = "*****"; //Wifi SSID
const char* password = "******"; //Wifi Password

//TODO: implement encoder based movement
const int leftForward = 14; //yellow motor wire
const int leftReverse = 12; //red motor wire
const int rightForward = 5; //blue motor wire
const int rightReverse = 4; //green motor wire

ESP8266WebServer server(80);

void moveForward(){
  digitalWrite(leftForward,HIGH);
  digitalWrite(rightForward,HIGH);
  delay(500);
  digitalWrite(leftForward,LOW);
  digitalWrite(rightForward,LOW);
  }
  
void moveReverse(){
   digitalWrite(leftReverse,HIGH);
   digitalWrite(rightReverse,HIGH);
   delay(500);
   digitalWrite(leftReverse,LOW);
   digitalWrite(rightReverse,LOW);
  }
  
void moveLeft(){
  digitalWrite(leftReverse,HIGH);
  digitalWrite(rightForward,HIGH);
  delay(1000);
  digitalWrite(leftReverse,LOW);
  digitalWrite(rightForward,LOW);
  }  

void moveRight(){
  digitalWrite(rightReverse,HIGH);
  digitalWrite(leftForward,HIGH);
  delay(1000);
  digitalWrite(rightReverse,LOW);
  digitalWrite(leftForward,LOW);
  }  

void handleNotFound(){
  server.send(404,"text/plain","Command not found");
  }
  
void setup() {
  // put your setup code here, to run once:
  pinMode(leftForward, OUTPUT);
  pinMode(leftReverse, OUTPUT);
  pinMode(rightForward, OUTPUT);
  pinMode(rightReverse, OUTPUT);
  Serial.begin(115200);  
  //Connect to WIFI
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("..");
  }
  Serial.println(WiFi.localIP());

  if (MDNS.begin("agribot")) {
    Serial.println("MDNS responder started");
  }
  
  server.onNotFound(handleNotFound);
  
  server.on("/", [](){
    server.send(200, "text/html", "All Systems Nominal :D");
  });
  
  server.on("/forward", [](){
    server.send(200, "text/html", "Moving Forward");
    moveForward();
  });

  server.on("/reverse", [](){
    server.send(200, "text/html", "Moving Reverse");
    moveReverse();
  });
  
  server.on("/left", [](){
    server.send(200, "text/html", "Turning Left");
    moveLeft();
  });
  server.on("/right", [](){
    server.send(200, "text/html", "Turning Right");
    moveRight();
  });
  
  server.begin();
  Serial.println("Web server started!");
}

void loop(){
  server.handleClient();
  MDNS.update();
  }
