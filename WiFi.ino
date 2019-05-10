#include <ESP8266WiFi.h>
#include <time.h>

const char* ssid = "김현수의 iPhone";  // AP SSID
const char* password = "999999999"; // AP password

const int httpPort = 80;

const String KMA_url = "/wid/queryDFSRSS.jsp?zone=4511358000";

const char* SERVER = "www.kma.go.kr";


void setup(void) {
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

}

void loop() {
  WiFiClient client;
  String a[3];
  int i = 0;
  String temp;
  String wfEn;
  String reh;
  String tmp_str;

  if (client.connect(SERVER, httpPort)) {

    client.print(String("GET ") + KMA_url + " HTTP/1.1\r\n" +
                 "Host: " + SERVER + "\r\n" +
                 "Connection: close\r\n\r\n");

    delay(10);
    while (client.available()) {
      String line = client.readStringUntil('\n');

      i = line.indexOf("</temp>");

      if (i > 0) {
        tmp_str = "<temp>";
        temp = line.substring(line.indexOf(tmp_str) + tmp_str.length(), i);
        Serial.println(temp);

      }

      i = line.indexOf("</wfEn>");

      if (i > 0) {
        tmp_str = "<wfEn>";
        wfEn = line.substring(line.indexOf(tmp_str) + tmp_str.length(), i);
        Serial.println(wfEn);
      }

      i = line.indexOf("</reh>");

      if (i > 0) {
        tmp_str = "<reh>";
        reh = line.substring(line.indexOf(tmp_str) + tmp_str.length(), i);
        Serial.println(reh);
        break;
      }
    }
  }

  tft.drawString("Temperature:" + temp, 10, 50, GFXFF); // Print the string name of the font
  tft.drawString("Humidity :" + reh,  10, 80, GFXFF); // Print the string name of the font
  tft.drawString("Cloudy:" + wfEn, 10, 110, GFXFF); // Print the string name of the font

  delay(1000);
}
