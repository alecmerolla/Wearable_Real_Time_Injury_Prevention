//PIN definitions
#define SD_cs 5
#define piezo 32
#define button 15
#define devX 15.0
#define devY 15.0
#define devY 15.0

#include "wrips.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Wrips wrips(Adafruit_BNO055(55, 0x28));
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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

void display_init(void)
{
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  delay(100);
}

void oled_update(void)
{
  char str[256];
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.setTextSize(1);
  sprintf(str, "WRIPS ECE-1  %d\n", wrips.ms());
  display.println(F(str));
  sprintf(str, "X:%.1lf\nY:%.1lf\nZ:%.1lf\ndevX:%.1lf\ndevY:%.1lf\ndevZ:%.1lf\n",
          wrips.x(), wrips.y(), wrips.z(), wrips.dev_x(), wrips.dev_y(), wrips.dev_z());
  display.println(F(str));
  display.display();
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

  //  display_init();

  Serial.println("initialization done.");

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(piezo, OUTPUT);
}

void loop() {
  //  Wrips wrips(Adafruit_BNO055(55, 0x28));
  wrips.start();
  wrips.set_dev_x(devX);
  char file_name[256];
  display_init();

  while (1) {
    wrips.event();

    sprintf(file_name, "/testlog.txt");
    digitalWrite(LED_BUILTIN, HIGH);
    wrips.log_orientation(file_name);
    digitalWrite(LED_BUILTIN, LOW);
    wrips.print_orientation();
    wrips.calc_deviation();
    oled_update();

    if (button_trigged == true)
    {
      wrips.button_set_dev();
      digitalWrite(piezo, HIGH);
      button_trigged = false;
    }
    wrips.isBeep('x', piezo);

    Serial.println("Deviation: " + String(wrips.dev_x()) + ", " + String(wrips.dev_y()) + ", " + String(wrips.dev_z()));
    delay(100);
  }
}
