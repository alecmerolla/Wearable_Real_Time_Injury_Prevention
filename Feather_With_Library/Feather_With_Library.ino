//PIN definitions
#define SD_cs 5
#define piezo 32
#define button 15
#define devX 10.0
#define devY 10.0
#define devY 10.0

#include "wrips.h"

bool button_trigged = false;

//Interrupt function
//IRAM_ATTR runs in ram instead of flash for faster access
void IRAM_ATTR button_press(void)
{
  if (button_trigged == false)
  {
    button_trigged = true;
  }
}


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  //Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect.
  }

  Serial.print("Initializing SD card...");
  SD.begin(SD_cs);
  if (!SD.begin(SD_cs)) {
    Serial.println("initialization failed!");
    while (!SD.begin(SD_cs))
    {
      SD.begin(SD_cs);
      Serial.println("Trying SD card again...");
      delay(200);
    }
  }

  attachInterrupt(digitalPinToInterrupt(button), button_press, FALLING);

  Serial.println("initialization done.");

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(piezo, OUTPUT);
}

void loop() {
  Wrips wrips(Adafruit_BNO055(55, 0x28));
  wrips.set_dev_x(devX);
  char file_name[256];

  while (1) {
//    digitalWrite(LED_BUILTIN, HIGH);
    wrips.event();

    sprintf(file_name, "/testlog.txt");
    digitalWrite(LED_BUILTIN, HIGH);
    wrips.log_orientation(file_name);
    digitalWrite(LED_BUILTIN, LOW);
    wrips.print_orientation();
    wrips.calc_deviation();
    /*
      if (wrips.dev_x() > 20)
      {
      Serial.println("BEEP BEEP");
      digitalWrite(piezo, HIGH);
      }
      else
      {
      digitalWrite(piezo, LOW);
      }

      if (button_trigged == true)
      {
      digitalWrite(piezo, HIGH);
      button_trigged = false;
      Serial.println("BUTTON!!!");
      }
      else
      digitalWrite(piezo, LOW);
    */
    if (button_trigged == true)
    {
      wrips.button_set_dev();
      digitalWrite(piezo, HIGH);
      button_trigged = false;
    }
    wrips.isBeep('x', piezo);

    Serial.println("Deviation: " + String(wrips.dev_x()) + ", " + String(wrips.dev_y()) + ", " + String(wrips.dev_z()));
//    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
}
