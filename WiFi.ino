#include "WiFiEsp.h"
#include "SoftwareSerial.h"

SoftwareSerial esp(3, 2); // RX, TX
char ssid[] = "김현수의 iPhone";            // Your AP SSID
char pass[] = "999999999";               // Your AR Password
int status = WL_IDLE_STATUS;        // Status
unsigned long ulPreTime = 0;
bool flag_stop;
int trig = A4;
int echo = A5;
int dist;

void setup()
{
  pinMode(13,OUTPUT);
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  Serial.begin(9600);
  esp.begin(9600);
  Serial.println("Start get weather forecast");
  WiFi.init(&esp);
  Serial.println("Completed esp init");

  if (WiFi.status() == WL_NO_SHIELD)
  {
    Serial.println("WiFi shield not present");
    while (true);
  }
  // Check WIFI connection
  while (WiFi.status() != WL_CONNECTED)
  {
    status = WiFi.begin(ssid, pass);
  }
}
const String KMA_url = "/wid/queryDFSRSS.jsp?zone=1117053000";
const char* wSERVER = "www.kma.go.kr";
#define httpPort 80
int GetWeatherInformWIFI() {
  WiFiEspClient client;
  int i=0;
  String temp;
  String tmp_str;
  int cnt=0;
  int res=1;
  float t=0;
  Serial.println("start connect webserver");
  client.setTimeout(5000);
  client.flush();
  if (client.connect(wSERVER, httpPort)) {
    client.print(String("GET ") + KMA_url + " HTTP/1.1\r\n" +
    "Host: " + wSERVER + "\r\n" + 
    "Connection: close\r\n\r\n");
  }
   delay(1000);
   while(client.available()){    
    yield();
    String line = client.readStringUntil('\n');
   
    i = line.indexOf("</tm>");
    if(i > 0) {
      tmp_str="<tm>";
      temp=line.substring(line.indexOf(tmp_str)+tmp_str.length(),i - 4);
      Serial.print(temp + " ");
    }
    i= line.indexOf("</wfEn>");
    if(i>0){
      tmp_str="<wfEn>";
      temp = line.substring(line.indexOf(tmp_str)+tmp_str.length(),i);
      Serial.println(temp);
      if((temp.equals("Rain") || temp.equals("Snow/Rain") || temp.equals("Clear"))) {
          digitalWrite(trig, LOW);
          delayMicroseconds(2);
                   
          digitalWrite(trig,HIGH);
          delayMicroseconds(10);
                    
          digitalWrite(echo, LOW);
          long duration = pulseIn(echo, HIGH);
                    
          dist = duration / 29 / 2;
          if(dist < 7) {
            digitalWrite(13,HIGH);
          }else {
            digitalWrite(13,LOW);
          }
      } else {
        digitalWrite(13,LOW);
      }
      
     res=0;
     break;
     }
   }
  if(res == 1){ 
    Serial.println("Wifi weather Fail!");
  }else{
    Serial.println("Wifi weather OK!");
  }
  client.stop();
  
  return res;
}
void loop()
{

  unsigned long ulCurTime = millis();
  // period 1 sec
  if (ulCurTime - ulPreTime >= 1000)  {
    ulPreTime = ulCurTime;
    GetWeatherInformWIFI();
  }
}
 
