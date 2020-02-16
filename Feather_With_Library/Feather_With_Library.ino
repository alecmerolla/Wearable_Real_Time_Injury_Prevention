#include "wrips.h"

void setup() {
  // put your setup code here, to run once:
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");
  SD.begin(5);
  if (!SD.begin(5)) {
    Serial.println("initialization failed!");
    while(!SD.begin(5))
    {
      SD.begin(5);
      Serial.println("Trying SD card again...");
      delay(200);
    }
  }
  
  Serial.println("initialization done.");

  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {
  Wrips wrips(Adafruit_BNO055(55, 0x28));
  
  char file_name[256];

  while (1) {
    digitalWrite(LED_BUILTIN, HIGH);
    wrips.event();

    sprintf(file_name, "/testlog.txt");
    wrips.log_orientation(file_name);
    wrips.print_orientation();
    wrips.calc_deviation(100,100,100);
    Serial.println("Deviation: " + String(wrips.dev_x()) + ", " + String(wrips.dev_y()) + ", " + String(wrips.dev_z()));
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
  }
}
