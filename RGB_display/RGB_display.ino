/*
   Credit
   https://howtomechatronics.com/tutorials/arduino/arduino-color-sensing-tutorial-tcs230-tcs3200-color-sensor/
   https://dronebotworkshop.com/arduino-color-sense/
*/
#include <WS2812B.h>  // WS2812B โค้ดอ้างอิงจากตัวอย่าง WS2812B > WS2812B_test
#define NUM_LEDS 16

#define S0 PB3
#define S1 PB4
#define S2 PB5
#define S3 PB6
#define sensorOut PB7

#define LDR PA5

WS2812B strip = WS2812B(NUM_LEDS);   // ต่อกับขา MOSI PA7

void setup() {
  Serial.begin(115200);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  // Setting frequency-scaling to 2%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  strip.begin();// Sets up the SPI
  strip.show();// Clears the strip, as by default the strip data is set to all LED's off.
  //  strip.setBrightness(4);

  // ไฟเลี้ยงให้ LDR
  pinMode(PA4, OUTPUT);
  pinMode(PA3, OUTPUT);
  digitalWrite(PA4, LOW);
  digitalWrite(PA3, HIGH);

}

void loop() {

  // อ่านค่าสีจากเซนเซอร์สี
  int red = getRed();
  int green = getGreen();
  int blue = getBlue();

  // อ่านค่าความเข้มแสงจาก LDR
  int light = analogRead(LDR);

  // แสดงสี RGB  อ้างอิงจากตัวอย่าง WS2812B > WS2812B_test
  colorWipe(strip.Color(red, green, blue), 20); // Green

  // แสดงค่า
  Serial.print("Light:"); Serial.print(light);
  Serial.print("  Red:"); Serial.print(red);
  Serial.print("  Green:"); Serial.print(green);
  Serial.print("  Blue:"); Serial.println(blue);

  delay(500);
}

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

void colorWipe(uint32_t c, uint8_t wait)
{
  for (uint16_t i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
