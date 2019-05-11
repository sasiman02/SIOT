//*******************************************************************************
// Project : ESP8266 ESP-01 in Advenced set(Get Weather)
// Board : Arduino Uno 
// By : Kit Plus
//*******************************************************************************/
#include "WiFiEsp.h"
#include "SoftwareSerial.h"
SoftwareSerial esp(3, 2); // RX, TX
char ssid[] = "김현수의 iPhone";            // Your AP SSID
char pass[] = "999999999";               // Your AR Password
int status = WL_IDLE_STATUS;        // Status
unsigned long ulPreTime = 0;
bool flag_stop;

void setup()
{
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
  pinMode(13,OUTPUT);
}
String preTime="00000000";
const String KMA_url = "/wid/queryDFSRSS.jsp?zone=1117053000";
const char* wSERVER = "www.kma.go.kr";
#define httpPort 80
int GetWeatherInformWIFI() {
  WiFiEspClient client;
  int i=0;
  String temp;
  String nowTime;
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
   
    // search forecast weather string
    i = line.indexOf("</tm>");
    if(i > 0) {
      tmp_str="<tm>";
      nowTime=line.substring(line.indexOf(tmp_str)+tmp_str.length(),i - 4);
      Serial.println(nowTime);
    }

//      preTime = nowTime;
      i= line.indexOf("</wfEn>");
      if(i>0){
        tmp_str="<wfEn>";
        temp = line.substring(line.indexOf(tmp_str)+tmp_str.length(),i);
        Serial.println(temp);
        if(!preTime.equals(nowTime) && (temp.equals("Rain") || temp.equals("Snow/Rain")|| temp.equals("Clear"))) {
            preTime = nowTime;
            Serial.println("Buzzer On");
            analogWrite(13,255);
            delay(100);
            analogWrite(13,0);
            Serial.println("Buzzer Off");
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
  if (ulCurTime - ulPreTime >= 1000)
  {
    ulPreTime = ulCurTime;
    GetWeatherInformWIFI();
  }
}
 
