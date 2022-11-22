/*
   Credit
   https://howtomechatronics.com/tutorials/arduino/arduino-color-sensing-tutorial-tcs230-tcs3200-color-sensor/
   https://dronebotworkshop.com/arduino-color-sense/
*/
#include <WS2812B.h>  // WS2812B โค้ดอ้างอิงจากตัวอย่าง WS2812B > WS2812B_test
#define NUM_LEDS 16

// พินสำหรับเซนเซอร์สี RGB Colour Sensor (TCS230/TCS3200)
#define S0 PB3
#define S1 PB4
#define S2 PB5
#define S3 PB6
#define sensorOut PB7

// พินสำหรับเซนเซอร์แสง LDR
#define LDR PA5

// หลอดไฟ rgb
WS2812B strip = WS2812B(NUM_LEDS);   // ต่อกับขา MOSI PA7

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600); // Serial2 on rx,tx = PA3,PA2

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  // ตั้งค่าเซนเซอร์สี RGB Setting frequency-scaling to 2%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  // เริ่มทำงานหลอดไฟ RGB
  strip.begin();// Sets up the SPI
  strip.show();// Clears the strip, as by default the strip data is set to all LED's off.
  //  strip.setBrightness(4);

}

void loop() {

  // อ่านค่าสีจากเซนเซอร์สี เป็น digital sensor
  // เซนเซอร์สี RGB Colour Sensor (TCS230/TCS3200)
  int red = getRed();     // อ่านค่าสีแดง
  int green = getGreen(); // อ่านค่าสีเขียว
  int blue = getBlue();   // อ่านค่าสีฟ้า

  // อ่านค่าความเข้มแสงจาก LDR เป็น analog sensor มีค่าตั้งแต่ 0-4095
  int light = analogRead(LDR);

  // แสดงสีที่หลอดไฟ RGB  อ้างอิงจากตัวอย่าง WS2812B > WS2812B_test
  colorWipe(strip.Color(red, green, blue), 20); // แสดงสีที่ต้องการ

  // แสดงค่าจากเซนเซอร์
  Serial.print("Light:"); Serial.print(light);
  Serial.print("  Red:"); Serial.print(red);
  Serial.print("  Green:"); Serial.print(green);
  Serial.print("  Blue:"); Serial.print(blue);
  Serial.print("\n");

  // อ้างอิงตัวอย่าง https://iotkiddie.com/blog/serial-uart-esp-stm/
  // ส่งค่าไปยัง ESP โดยใช้ UART2
  Serial2.print("Light:"); Serial2.print(light);
  Serial2.print("  Red:"); Serial2.print(red);
  Serial2.print("  Green:"); Serial2.print(green);
  Serial2.print("  Blue:"); Serial2.print(blue);
  Serial2.print("\n");
  
  delay(1000);
}


// อ้างอิงจากตัวอย่าง
int getRed() {
  int frequency;
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  //Remaping the value of the frequency to the RGB Model of 0 to 255
  frequency = map(frequency, 20, 340, 255, 0);

  if (frequency > 255) frequency = 255;
  else if (frequency < 0) frequency = 0;

  return frequency;
}

int getGreen() {
  int frequency;
  // Setting Green filtered photodiodes to be read
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  //Remaping the value of the frequency to the RGB Model of 0 to 255
  frequency = map(frequency, 30, 335, 255, 0);

  if (frequency > 255) frequency = 255;
  else if (frequency < 0) frequency = 0;

  return frequency;
}

int getBlue() {
  int frequency;
  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  //Remaping the value of the frequency to the RGB Model of 0 to 255
  frequency = map(frequency, 15, 230, 255, 0);

  if (frequency > 255) frequency = 255;
  else if (frequency < 0) frequency = 0;

  return frequency;
}

// อ้างอิงจาก WS2812B > WS2812B_test
void colorWipe(uint32_t c, uint8_t wait)
{
  for (uint16_t i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
