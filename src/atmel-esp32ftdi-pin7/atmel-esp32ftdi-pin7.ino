// make sure the target board is sparkfun pro micro, 3.3v

#define PROGPIN 7

void setup() {
  pinMode(PROGPIN, OUTPUT);
  digitalWrite(PROGPIN, LOW);       // set it this way to program the ESP32 via FTDI port
  //digitalWrite(PROGPIN, HIGH);      // set it this way to program the ESP32 via the atmel

  Serial.begin(115200);
}

void loop() {
  int state = digitalRead(PROGPIN);
  Serial.print("State: ");
  if (state){
    Serial.println("HIGH: Program ESP32 via atmel");
   }
  else {
    Serial.println("LOW: Program ESP32 via FTDI port");
  }  
  delay(2000);
  
}
