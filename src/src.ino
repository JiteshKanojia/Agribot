#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
 
const char* ssid = "TP-LINK_469A"; //Wifi SSID
const char* password = "30052314"; //Wifi Password

//TODO: implement encoder based movement
const int leftForward = 14; //yellow motor wire
const int leftReverse = 12; //red motor wire
const int rightForward = 5; //blue motor wire
const int rightReverse = 4; //green motor wire
const int rightWheelEncoder = 9; //right wheel encoder input
const int leftWheelEncoder = 10; //left wheel encoder input
const int leftPump = 13;   //Left motorpump output
const int rightPump = 15;  //Right motorpump output

volatile unsigned long int leftTickCount;
volatile unsigned long int rightTickCount;

ESP8266WebServer server(80);

////ISR for Wheel Encoders

IRAM_ATTR void detectLeftTick(){
  leftTickCount++;
  //Serial.println(leftTickCount);
  }

IRAM_ATTR void detectRightTick(){
  rightTickCount++;
  //Serial.println(rightTickCount);
  }  
///////////////////////////////////////
void moveForwardPrecise(int numberOfTicks){
  unsigned long int ticksToMoveLeft = leftTickCount + numberOfTicks;
  unsigned long int ticksToMoveRight = leftTickCount + numberOfTicks;
  
  digitalWrite(leftForward,HIGH);
  digitalWrite(rightForward,HIGH);
  while(true){
      if(leftTickCount >= ticksToMoveLeft && rightTickCount >= ticksToMoveRight){
        digitalWrite(leftForward,LOW);
        digitalWrite(rightForward,LOW);
        return;    
        }        
    }

  }
  
void moveReversePrecise(int numberOfTicks){
  unsigned long int ticksToMoveLeft = leftTickCount + numberOfTicks;
  unsigned long int ticksToMoveRight = leftTickCount + numberOfTicks;
  
  digitalWrite(leftReverse,HIGH);
  digitalWrite(rightReverse,HIGH);
  while(true){
      if(leftTickCount >= ticksToMoveLeft && rightTickCount >= ticksToMoveRight){
        digitalWrite(leftReverse,LOW);
        digitalWrite(rightReverse,LOW);
        return;    
        }        
    }

  }  

void turnRightPrecise(int numberOfTicks){
  unsigned long int ticksToMoveLeft = leftTickCount + numberOfTicks;
  
  digitalWrite(rightReverse,HIGH);
  digitalWrite(leftForward,HIGH);
  while(true){
      if(leftTickCount >= ticksToMoveLeft){
        digitalWrite(rightReverse,LOW);
        digitalWrite(leftForward,LOW);
        return;    
        }        
    }
  }
  
void turnLeftPrecise(int numberOfTicks){
  unsigned long int ticksToMoveRight = rightTickCount + numberOfTicks;
  
  digitalWrite(leftReverse,HIGH);
  digitalWrite(rightForward,HIGH);
  while(true){
      if(rightTickCount >= ticksToMoveRight){
        digitalWrite(leftReverse,LOW);
        digitalWrite(rightForward,LOW);
        return;    
        }        
    }
  }
void activateLeftPump(){
  const long interval = 1000;
  unsigned long previousMillis = 0;
  while(true){
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= interval){      
      previousMillis = currentMillis;
      digitalWrite(leftPump, HIGH);
    }else{
      digitalWrite(leftPump, LOW);    
      return;
      }  
    }
  }
      
void activateRightPump(){
  const long interval = 1000;
  unsigned long previousMillis = 0;
  while(true){
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= interval){      
      previousMillis = currentMillis;
      digitalWrite(rightPump, HIGH);
    }else{
      digitalWrite(rightPump, LOW);    
      return;
      }  
    }
  }    

void activateBothPump(){
  const long interval = 1000;
  unsigned long previousMillis = 0;
  while(true){
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= interval){      
      previousMillis = currentMillis;
      digitalWrite(leftPump, HIGH);
      digitalWrite(rightPump, HIGH);
    }else{
      digitalWrite(leftPump, LOW);
      digitalWrite(rightPump, LOW);    
      return;
      }  
    }
  }    
    
  
void handleNotFound(){
  server.send(404,"text/plain","Command not found");
  }
  
void setup() {
  pinMode(leftForward, OUTPUT);
  pinMode(leftReverse, OUTPUT);
  pinMode(rightForward, OUTPUT);
  pinMode(rightReverse, OUTPUT);
  pinMode(rightWheelEncoder,INPUT);
  pinMode(leftWheelEncoder,INPUT);
  pinMode(leftPump, OUTPUT);
  pinMode(rightPump, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(leftWheelEncoder),detectLeftTick,FALLING);
  attachInterrupt(digitalPinToInterrupt(rightWheelEncoder),detectRightTick,FALLING);
  //Initialize serial debug output
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
    server.send(200, "text/html", "OK");
    moveForwardPrecise(11);
  });

  server.on("/reverse", [](){
    server.send(200, "text/html", "OK");
    moveReversePrecise(15);
  });
  
  server.on("/left", [](){
    server.send(200, "text/html", "OK");
    turnLeftPrecise(18);
  });
  
  server.on("/right", [](){
    server.send(200, "text/html", "OK");
    turnRightPrecise(18);
  });

  server.on("/sprayleft", [](){
    server.send(200, "text/html", "OK");
    activateLeftPump();
  });
  
  server.on("/sprayright", [](){
    server.send(200, "text/html", "OK");
    activateRightPump();
  });

  server.on("/sprayboth", [](){
    server.send(200, "text/html", "OK");
    activateBothPump();
  });
  
  server.begin();
  Serial.println("Web server started!");
}

void loop(){
  server.handleClient();
  MDNS.update();
  //Serial.println("OK");
  }
