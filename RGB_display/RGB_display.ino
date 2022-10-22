/*
   Credit
   https://howtomechatronics.com/tutorials/arduino/arduino-color-sensing-tutorial-tcs230-tcs3200-color-sensor/
   https://dronebotworkshop.com/arduino-color-sense/
*/

#define S0 PB3
#define S1 PB4
#define S2 PB5
#define S3 PB6
#define sensorOut PB7

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  // Setting frequency-scaling to 2%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  Serial.begin(115200);
}

void loop() {
  int red = getRed();
  int green = getGreen();
  int blue = getBlue();

  Serial.print("Red:"); Serial.print(red);
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
  frequency = map(frequency, 20, 360, 255, 0);

  if (frequency > 255) frequency = 255;
  else if (frequency < 0) frequency = 0;

  // Printing the value on the serial monitor
  //  Serial.print("R= ");//printing name
  //  Serial.print(frequency);//printing RED color frequency
  //  Serial.print("  ");

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

  // Printing the value on the serial monitor
  //  Serial.print("G= ");//printing name
  //  Serial.print(frequency);//printing RED color frequency
  //  Serial.print("  ");

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
  frequency = map(frequency, 15, 240, 255, 0);

  if (frequency > 255) frequency = 255;
  else if (frequency < 0) frequency = 0;

  // Printing the value on the serial monitor
  //  Serial.print("B= ");//printing name
  //  Serial.print(frequency);//printing RED color frequency
  //  Serial.println("  ");

  return frequency;
}
