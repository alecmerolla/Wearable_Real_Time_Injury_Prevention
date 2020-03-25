//PIN definitions
#define SD_cs 5
#define piezo 32
#define button 15
#define devX 15.0
#define devY 15.0
#define devY 15.0

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "FS.h"
#include "wrips.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1
#define DELAY 100

// network credentials
const char* ssid = "wavelengths";
const char* password = "mikemona100";

bool button_trigged = false;

void TaskProcessSensor( void *pvParameters);
void TaskProcessLog( void *pvParameters);
void TaskProcessWebserver(void *pvParameters);
void IRAM_ATTR button_press(void);
void display_init(void);
void oled_update(void);
void button_set_dev(void);
String get_data(void);

// AsyncWebServer object set on port 80, common http port
AsyncWebServer server(80);
// Static IP address
IPAddress local_IP(192, 168, 0, 225);
// Gateway IP address
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 0, 0);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Wrips wrips(Adafruit_BNO055(55, 0x28));
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  //Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect.
  }

  attachInterrupt(digitalPinToInterrupt(button), button_press, FALLING);
  pinMode(LED_BUILTIN, OUTPUT);

  //  display_init();
  wrips.start();
  wrips.set_dev_x(devX);

  xTaskCreatePinnedToCore(
    TaskProcessSensor
    ,  "TaskProcessSensor"
    ,  4096
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL
    ,  0 ); //Arduino core to run on, be careful with core 0 for ESP32 firmware protocols
  /*
  xTaskCreatePinnedToCore(
    TaskProcessLog
    ,  "TaskProcessLog"
    ,  4096
    ,  NULL
    ,  1
    ,  NULL
    ,  1);
  */
  xTaskCreatePinnedToCore(
    TaskProcessWebserver
    ,  "TaskProcessWebserver"
    ,  16384
    ,  NULL
    ,  1
    ,  NULL
    ,  1);

  Serial.println("initialization done.");
}

void loop() {
  //  Wrips wrips(Adafruit_BNO055(55, 0x28));
  delay(99999999);
}

void TaskProcessWebserver(void *pvParameters)
{
  // Initialize SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html");
  });
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", get_data().c_str());
  });

  // Start server
  server.begin();
  while (1)
  {
  }
}

void TaskProcessSensor(void *pvParameters)
{
  (void) pvParameters;
  display_init();
  while (1) {
    wrips.event();
    wrips.print_orientation();
    wrips.calc_deviation();
    oled_update();
    wrips.isBeep('x', piezo);
    button_set_dev();
    Serial.println("Deviation: " + String(wrips.dev_x()) + ", " + String(wrips.dev_y()) + ", " + String(wrips.dev_z()));
    vTaskDelay(DELAY);
  }
}

void TaskProcessLog(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
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
  while (1) {
    if (wrips.isAvail()) {
      char file_name[256];
      sprintf(file_name, "/testlog.txt");
      digitalWrite(LED_BUILTIN, HIGH);
      wrips.log_orientation(file_name);
      vTaskDelay(500);
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}

//Interrupt function
//IRAM_ATTR runs in ram instead of flash for faster access
void IRAM_ATTR button_press(void)
{
  if (button_trigged == false)
  {
    button_trigged = true;
  }
}

String get_data(void) {
  return String(wrips.x()) + ", " + String(wrips.y()) + ", "
         + String(wrips.z());
}

void button_set_dev(void)
{
  if (button_trigged == true)
  {
    wrips.button_set_dev();
    digitalWrite(piezo, HIGH);
    button_trigged = false;
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
