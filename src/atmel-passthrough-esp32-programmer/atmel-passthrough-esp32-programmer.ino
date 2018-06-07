/* Modified from https://github.com/AprilBrother/cactus-micro-r2/tree/master/libraries/CactusMicro/examples/esp8266Programmer */

int program_pin = 7;

void setup() {  
  pinMode(program_pin, OUTPUT);
  digitalWrite(program_pin, HIGH);

//  Serial.begin(115200);
//  Serial1.begin(115200);
  Serial.begin(57600);
  Serial1.begin(57600);

  Serial.println("ESP32 programmer ready.");
}

void loop() {  
  // pass data from ESP to host, if any
  while(Serial1.available())
  {
    Serial.write((uint8_t)Serial1.read());
  }

  // pass data from host to ESP, if any
  while(Serial.available())
  {
    Serial1.write((uint8_t)Serial.read());
   
  }
  
}



