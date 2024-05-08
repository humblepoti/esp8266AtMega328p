#include <ArduinoJson.h>
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


#define STATIC_IP_ADDRESS  192,168,86,246 // Change to the IP Address of your choice
#define SUBNET_MASK        255,255,255,0 // Change to the subnet of your network
#define GATEWAY_IP_ADDRESS 192,168,86,1 // Change to the gateway of your newtwork

char ssid[] = "YOUR WIFI SSID";
char password[] = "WIFI PASSWORD";
int soilmoisture;
int temperature;
int humidity;



ESP8266WebServer server(80);
 
// Serving Soild Moisture
void getSoilMoist() {
  int soilmoisture = getSoilMoisture();
    server.send(200, "text/json", "{\"moisture\": \""+String(soilmoisture)+"\"}");
}

// Serving Temperature
void getTemp() {
  int temperature = getTemper();
    server.send(200, "text/json", "{\"temperature\": \""+String(temperature)+"\"}");
}

//Service Humidty
void getHumid() {
    int humidity = getHumidit();
    server.send(200, "text/json", "{\"humidity\": \""+String(humidity)+"\"}");
  }

// Serving all objects
void getall() {
    getAll();
    server.send(200, "text/json", "{\"humidity\": \""+String(humidity)+"\", \"temperature\": \""+String(temperature)+"\", \"moisture\": \""+String(soilmoisture)+"\"}");
  }
 
// Define routing
void restServerRouting() {
    server.on("/", HTTP_GET, []() {
        server.send(200, F("text/html"),
            F("Welcome to the REST Web Server"));
    });
    server.on(F("/sensors"), HTTP_GET, getall);    
    server.on(F("/temp"), HTTP_GET, getTemp);
    server.on(F("/humidity"), HTTP_GET, getHumid);    
    server.on(F("/soilmoisture"), HTTP_GET, getSoilMoist);
}
 
// Manage not found URL
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

 void setWebServer(){
  
  // Set server routing
  restServerRouting();
  // Set not found response
  server.onNotFound(handleNotFound);
  // Start server
  server.begin();
  Serial.println("HTTP server started");
  }


void setWifi(){

  IPAddress local_ip(STATIC_IP_ADDRESS);
  IPAddress subnet(SUBNET_MASK);
  IPAddress gateway(GATEWAY_IP_ADDRESS);


 // Configures static IP address
  if (!WiFi.config(local_ip, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
   if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
  
  }

int getTemper(){
  bool StringReady;
  String json;
  while (Serial.available()){ // listen for serial data from the Arduino Atmega328
         json=Serial.readString();
         StringReady = true;
         }
     
         if (StringReady){
          StaticJsonBuffer<200> jsonBuffer;  // preallocated memory to store the JsonObject max 200 bytes
          JsonObject& root = jsonBuffer.parseObject(json); // turning the string into JSON data
       
            if(!root.success()) {
              return temperature;
            }
       
         temperature = root["temperature"]; //
         root.remove("temperature");// remove the temp field from the JSON root object so I can reuse the root object to send back data
         // if the serial console is open it will print: 22:40:32.989 -> 21oC : temperature recieved from Arduino at ESP

         // SEND DATA BACK TO THE ARDUINO
         //delay (5000); // delay the send back by 5 seconds
//         int moistureSendToArduino = soilmoisture ;
         return temperature;

   }
  }





int getHumidit(){
  bool StringReady;
  String json;
  while (Serial.available()){ // listen for serial data from the Arduino Atmega328
         json=Serial.readString();
         StringReady = true;
         }
     
         if (StringReady){
          StaticJsonBuffer<200> jsonBuffer;  // preallocated memory to store the JsonObject max 200 bytes
          JsonObject& root = jsonBuffer.parseObject(json); // turning the string into JSON data
       
            if(!root.success()) {
              return humidity;
            }
       
         humidity = root["humidity"]; //
         root.remove("humidity");// remove the temp field from the JSON root object so I can reuse the root object to send back data
         return humidity;

   }
  }



void getAll(){
  bool StringReady;
  String json;
  while (Serial.available()){ // listen for serial data from the Arduino Atmega328
         json = Serial.readString();
         StringReady = true;
         }
     
         if (StringReady){
          StaticJsonBuffer<200> jsonBuffer;  // preallocated memory to store the JsonObject max 200 bytes
          JsonObject& parsed = jsonBuffer.parseObject(json); // turning the string into JSON data
          int retorno[3];
          soilmoisture = parsed["moisture"];
          temperature = parsed["temperature"];
          humidity = parsed["humidity"];
//            if(!root.success()) {
//              return humidity;
//            }
       
//         humidity = root["humidity"]; //
//         root.remove("humidity");// remove the temp field from the JSON root object so I can reuse the root object to send back data
//         return retorno;

   }
  }

int getSoilMoisture(){
  bool StringReady;
  String json;
  

   //ACCEPT DATA FROM THE ARDUINO
         while (Serial.available()){ // listen for serial data from the Arduino Atmega328
         json=Serial.readString();
         StringReady = true;
         }
     
         if (StringReady){
          StaticJsonBuffer<200> jsonBuffer;  // preallocated memory to store the JsonObject max 200 bytes
          JsonObject& root = jsonBuffer.parseObject(json); // turning the string into JSON data
       
            if(!root.success()) {
              //Serial.println("parseObject() failed");
              return soilmoisture;
            }
       
         soilmoisture = root["moisture"]; 
         root.remove("moisture");
         return soilmoisture;
   
   }
  }

void setup() {
  Serial.begin(57600);

  setWifi();
  setWebServer();
 
 
  }

void loop()
{

   server.handleClient();

}
