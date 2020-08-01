#include <ESP8266WiFi.h>
#include"dht.h"


#define LIGHT1 16 
#define LIGHT2 5
#define FAN1 13
#define FAN2 14

int sensor=3;


String readString;
dht DHT;

IPAddress ip(192,168,43,150);
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255, 0);
IPAddress DNS(192,168,43,1);
WiFiClient client;

const char* ssid ="bro";
const char* password ="cheppanu";
boolean Light1=HIGH,Light2=HIGH,Fan1=HIGH,Fan2=HIGH;
String str[2]={" ","checked"};
int Temp;

WiFiServer server(80);
  
void setup() 
{
Serial.begin(115200);
delay(10);
  pinMode(LIGHT1, OUTPUT);
  digitalWrite(LIGHT1,HIGH);
   pinMode(LIGHT2, OUTPUT);
  digitalWrite(LIGHT2,HIGH);
     pinMode(FAN1, OUTPUT);
  digitalWrite(FAN1,HIGH);
  pinMode(FAN2, OUTPUT);
  digitalWrite(FAN2,HIGH);

  Serial.println();
  Serial.println();
  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet, DNS);
  delay(100); 
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  while (WiFi.waitForConnectResult() != WL_CONNECTED) 
  {
    Serial.println("Fail connecting");
    delay(5000);
    ESP.restart();
  }
   Serial.print("   OK  ");
  Serial.print("Module IP: ");
  Serial.println(WiFi.localIP());
  
  server.begin();
  Serial.println("Server started");
 }


void loop()
{

 int chk=DHT.read11(sensor);
  WiFiClient client = server.available();
  if (client) 
  {
    while (client.connected()) 
    {   
      if (client.available()) 
      {
        char c = client.read();
        if (readString.length() < 100) 
        {
          readString += c;
         }
        if (c == '\n') {                           
        Serial.println(readString);
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println("Refresh: 5");
        client.println();
        client.println("<!DOCTYPE HTML>");
        client.println("<html>");
        client.println("<head>");
        client.println("<meta charset=\"utf-8\">");
        client.println("<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">");
        client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
        client.println("<script src=\"https://code.jquery.com/jquery-2.1.3.min.js\"></script>");
        client.println("<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css\">");
        client.println("</head><div class=\"container\">");
        
        client.println("<h1>SMART HOME AUTOMATION HUB</h1><br>");
        client.println("<div class=\"row\">");
        client.println("<div class=\"col-sm-4\">");
        client.println("<h2>Temperature</h2>");
        client.println("<input type= \"text\" value= "+String(DHT.temperature,DEC)+" class= \"form-control\">"); 
         client.println("<h2>Humidity</h2>");
        client.println("<input type= \"text\" value= "+String(DHT.humidity,DEC)+" class= \"form-control\">");        
        client.println("</div >");
        client.println("</div >");
                
        client.println("<h2>LIGHT1</h2>");
        client.println("<div class=\"row\">");
        if(Light1==HIGH)
        client.println("<div class=\"col-md-2\"><a href=\"?pin=LIGHT1ON\" class=\"btn btn-block btn-lg btn-success\" role=\"button\">LIGHT1 ON</a></div>");
        else
        client.println("<div class=\"col-md-2\"><a href=\"?pin=LIGHT1OFF\" class=\"btn btn-block btn-lg btn-danger\" role=\"button\">LIGHT1 OFF</a></div>");
        client.println("</div>");
        client.println("<h2>LIGHT2</h2>");
        client.println("<div class=\"row\">");
        if(Light2==HIGH)
        client.println("<div class=\"col-md-2\"><a href=\"?pin=LIGHT2ON\" class=\"btn btn-block btn-lg btn-success\" role=\"button\">LIGHT2 ON</a></div>");
        else
        client.println("<div class=\"col-md-2\"><a href=\"?pin=LIGHT2OFF\" class=\"btn btn-block btn-lg btn-danger\" role=\"button\">LIGHT2 OFF</a></div>");
        client.println("</div>");
        client.println("<h2>FAN1</h2>");
        client.println("<div class=\"row\">");
        if(Fan1==HIGH)
        client.println("<div class=\"col-md-2\"><a href=\"?pin=FAN1ON\" class=\"btn btn-block btn-lg btn-success\" role=\"button\">FAN1 ON</a></div>");
        else
        client.println("<div class=\"col-md-2\"><a href=\"?pin=FAN1OFF\" class=\"btn btn-block btn-lg btn-danger\" role=\"button\">FAN1 OFF</a></div>");
        client.println("</div>");
         client.println("<h2>FAN2</h2>");
        client.println("<div class=\"row\">");
        if(Fan2==HIGH)
        client.println("<div class=\"col-md-2\"><a href=\"?pin=FAN2ON\" class=\"btn btn-block btn-lg btn-success\" role=\"button\">FAN2 ON</a></div>");
        else
        client.println("<div class=\"col-md-2\"><a href=\"?pin=FAN2OFF\" class=\"btn btn-block btn-lg btn-danger\" role=\"button\">FAN2 OFF</a></div>");
        client.println("<br><br></div>");
        client.println("</html>");
        delay(1);
        client.stop();
         if (readString.indexOf("?pin=LIGHT1ON") >0)
         {
               Light1=LOW;
               digitalWrite(LIGHT1, LOW);
               
           }
            else if (readString.indexOf("?pin=LIGHT1OFF") >0)
            {  Light1=HIGH;
               digitalWrite(LIGHT1, HIGH);
               
               
           }
           if (readString.indexOf("?pin=LIGHT2ON") >0){
            Light2=LOW;
               digitalWrite(LIGHT2, LOW);
           }
            else if (readString.indexOf("?pin=LIGHT2OFF") >0){
              Light2=HIGH;
               digitalWrite(LIGHT2, HIGH);
               
           }
            
           if (readString.indexOf("?pin=FAN1ON") >0){
            Fan1=LOW;
               digitalWrite(FAN1, LOW);
           }
            else if (readString.indexOf("?pin=FAN1OFF") >0){
              Fan1=HIGH;
               digitalWrite(FAN1, HIGH);
               
           }
            if (readString.indexOf("?pin=FAN2ON") >0)
            {
                 Fan2=LOW;
                digitalWrite(FAN2, LOW);
            }
           else if (readString.indexOf("?pin=FAN2OFF") >0){
            Fan2=HIGH;
               digitalWrite(FAN2, HIGH);
              
           }
            readString="";  
           
         }
       }
    }
 }
}
