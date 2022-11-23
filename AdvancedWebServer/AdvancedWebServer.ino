/*
โค้ดต้นฉบับจาก Example > ESP8266WebServer > AdvancedWebServer
อ้างอิงโค้ด Web Server https://iotkiddie.com/blog/esp8266-webserver/
อ้างอิงโค้ด Serail STM->ESP https://iotkiddie.com/blog/serial-uart-esp-stm/

การต่อระหว่างบอร์ด esp กับ stm
PA3 > D3
PA2 > D4
GND > GND

การอัพโหลดบอร์ด esp8266
https://medium.com/@pechpijitthapudom/%E0%B8%81%E0%B8%B2%E0%B8%A3%E0%B8%95%E0%B8%B4%E0%B8%94%E0%B8%95%E0%B8%B1%E0%B9%89%E0%B8%87-esp8266-arduino-core-%E0%B9%80%E0%B8%9E%E0%B8%B7%E0%B9%88%E0%B8%AD%E0%B9%83%E0%B8%8A%E0%B9%89%E0%B8%87%E0%B8%B2%E0%B8%99%E0%B8%81%E0%B8%B1%E0%B8%9A-arduino-ide-7ad468e969e6
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SoftwareSerial.h>

// ตั้งค่า softwareSerial ที่พิน RX,TX = D4,D3
SoftwareSerial stmSerial(D4, D3); // RX, TX

// ตั้งค่า wifi ที่ต้องการเชื่อมต่อ
#ifndef STASSID
#define STASSID "ชื่อไวไฟ"
#define STAPSK "รหัสไวไฟ"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

// ตัวแปรที่ต้องรับค่าจาก STM
int light, red, green, blue;

// สร้างเว็บเซอเวอร์ที่พอร์ต 80 พอร์ตมาตรฐานของหน้าเว็บ
ESP8266WebServer server(80);

// ส่งค่า HTML หน้าเว็บกลับไปยัง Web Browser
void handleRoot()
{
/*
อธิบาย HTML

รีเฟรชหน้าเอง (จากตัวอย่าง AdvancedWebServer)
meta http-equiv='refresh' content='10'            =>    ให้หน้าเว็บรีเฟรชเองทุก 10 วินาทีเพื่ออัพเดทค่า

css ปรับแต่งหน้าเว็บ (จากตัวอย่าง AdvancedWebServer)
background-color: #cccccc;                        =>    สีพื้นหลัง
font-family: Arial, Helvetica, Sans-Serif;        =>    ฟอร์น
Color: #000088; text-align: center;               =>    สีตัวอักษร

รูปภาพ svg
ที่มารูป https://fontawesome.com/icons/lightbulb?s=solid&f=classic
ปรับแต่งสี http://www.svgbasics.com/colour.html
ปรับขนาดรูป https://css-tricks.com/scale-svg/
*/


  String payload; // สร้างตัวแปรสตริงเก็บโค้ด html

  // กำหนดค่า html และรวมกับค่าจากเซนเซอร์ โดยให้รีเฟรชอัตโนมัติทุก 10 วินาที  กำหนดเวลาที meta http-equiv='refresh' content='10'
  payload = "<html>\
              <head>\
                <meta http-equiv='refresh' content='10'/>\ 
                <title>ESP8266 Color RGB</title>\
                <style>\
                  body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; text-align: center; }\
                </style>\
              </head>\
              <body>\
                <h1>Color RGB</h1>\
                <p>Light value : " + String(light) + "</p>\
                <p>RGB value : (" + String(red) + ", " + String(green) + ", " + String(blue) + ")</p>\
                <p><svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 384 512' fill = 'rgb(" + String(red) + ", " + String(green) + ", " + String(blue) + ")' style='height: 50%;'><path d='M272 384c9.6-31.9 29.5-59.1 49.2-86.2l0 0c5.2-7.1 10.4-14.2 15.4-21.4c19.8-28.5 31.4-63 31.4-100.3C368 78.8 289.2 0 192 0S16 78.8 16 176c0 37.3 11.6 71.9 31.4 100.3c5 7.2 10.2 14.3 15.4 21.4l0 0c19.8 27.1 39.7 54.4 49.2 86.2H272zM192 512c44.2 0 80-35.8 80-80V416H112v16c0 44.2 35.8 80 80 80zM112 176c0 8.8-7.2 16-16 16s-16-7.2-16-16c0-61.9 50.1-112 112-112c8.8 0 16 7.2 16 16s-7.2 16-16 16c-44.2 0-80 35.8-80 80z' />\
                </svg></p>\
              </body>\
            </html>";

  // ส่งค่าตัวแปร  payload กับไปยัง  Web Browser
  server.send(200, "text/html", payload);
}

// ส่งค่า HTML หน้าเว็บกลับไปยัง Web Browser กรณีไม่มี url ที่เรียกหา
void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}

void setup(void)
{

  Serial.begin(115200);

  // ตั้งค่า softwareSerial ที่ความเร็ว 9600
  stmSerial.begin(9600);

  // เชื่อมต่อไวไฟ
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); // แสดงค่า ip address

  if (MDNS.begin("esp8266"))
  {
    Serial.println("MDNS responder started");
  }

  // ส่งค่า HTML หน้าเว็บกลับไปยัง Web Browser กรณีต่างๆ
  server.on("/", handleRoot);        // กรณีเรียกหน้าแรก
  server.onNotFound(handleNotFound); // กรณีเรียกหน้าอื่นๆ
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void)
{
  server.handleClient();
  MDNS.update();

  // ถ้าได้รับค่ามาจาก STM ผ่าน serial uart
  while (stmSerial.available() > 0)
  {

    // รับค่าต่างๆที่ได้รับจากบอร์ด STM เก็บไว้ในตัวแปร
    light = stmSerial.parseInt();
    red = stmSerial.parseInt();
    green = stmSerial.parseInt();
    blue = stmSerial.parseInt();

    // ถ้าจบบรรทัดแล้ว(ตัว \n คือจบบรรทัด) ให้แสดงค่าที่ได้รับผ่าน Serial monitor
    if (stmSerial.read() == '\n')
    {
      Serial.print("Light : ");
      Serial.print(light);
      Serial.print(" RGB : ");
      Serial.print(red);
      Serial.print(" ");
      Serial.print(green);
      Serial.print(" ");
      Serial.println(blue);
    }
  }
}
