#include <SoftwareSerial.h>

SoftwareSerial mySerial(D4, D3); // RX, TX

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);
}

void loop() {
  // if there's any serial available, read it:
  while (mySerial.available() > 0) {
    // look for the next valid integer in the incoming serial stream:
    int light = mySerial.parseInt();
    
    int red = mySerial.parseInt();
    // do it again:
    int green = mySerial.parseInt();
    // do it again:
    int blue = mySerial.parseInt();

    // look for the newline. That's the end of your sentence:
    if (mySerial.read() == '\n') {
      // print the three numbers in one string as hexadecimal:
      Serial.print("Light : ");
      Serial.print(light, DEC);
      Serial.print("RGB : ");
      Serial.print(red, DEC);
      Serial.print(" ");
      Serial.print(green, DEC);
      Serial.print(" ");
      Serial.println(blue, DEC);
    }
  }
}
