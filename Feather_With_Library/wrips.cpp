#include "Arduino.h"
#include "wrips.h"
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>


uint16_t BNO055_SAMPLERATE_DELAY_MS = 250;

Wrips::Wrips(Adafruit_BNO055 bno)
{
  _bno = bno;
  _x = -100000;
  _y = -100000;
  _z = -100000;
  _ms = 9999999;
  if (!_bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  delay(1000);
}

void Wrips::event(void)
{
  sensors_event_t orientationData;
  _bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  orientation(&orientationData);
}

void Wrips::orientation(sensors_event_t* event)
{
  if (event->type == SENSOR_TYPE_ACCELEROMETER) {
    _x = event->acceleration.x;
    _y = event->acceleration.y;
    _z = event->acceleration.z;
  }
  else if (event->type == SENSOR_TYPE_ORIENTATION) {
    _x = event->orientation.x;
    _y = event->orientation.y;
    _z = event->orientation.z;
  }
  else if (event->type == SENSOR_TYPE_MAGNETIC_FIELD) {
    _x = event->magnetic.x;
    _y = event->magnetic.y;
    _z = event->magnetic.z;
  }
  else if ((event->type == SENSOR_TYPE_GYROSCOPE) || (event->type == SENSOR_TYPE_ROTATION_VECTOR)) {
    _x = event->gyro.x;
    _y = event->gyro.y;
    _z = event->gyro.z;
  }
  _ms = millis();
}

//note when opening file you must use /test.txt
void Wrips::log_orientation(char *file_name)
{
  File file;
  file = SD.open(file_name, FILE_APPEND);
  if (file) {
    file.println(String(_ms) + ", " + String(_x) + ", " + String(_y) + ", " + String(_z));
    file.close();
  }
  else {
    Serial.println("error opening " + String(file_name));
  }
}

void Wrips::print_orientation(void)
{
  Serial.println(String(_ms) + ", " + String(_x) + ", " + String(_y) + ", " + String(_z));
}





