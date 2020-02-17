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
  //init private vars
  _bno = bno;
  _x = -100000;
  _y = -100000;
  _z = -100000;
  _ms = 9999999;

  // for angle deviation
  _dev_x = _x;
  _dev_y = _y;
  _dev_z = _z;
  _set_dev_x = _x;
  _set_dev_y = _y;
  _set_dev_z = _z;
  _dev_ori_x = _x;
  _dev_ori_y = _y;
  _dev_ori_z = _z;

  if (!_bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  delay(1000);
}

//functions to return private var
double Wrips::x(void)
{
  return _x;
}

double Wrips::y(void)
{
  return _y;
}

double Wrips::z(void)
{
  return _z;
}

unsigned int Wrips::ms(void)
{
  return _ms;
}

double Wrips::dev_x(void)
{
  return _dev_x;
}

double Wrips::dev_y(void)
{
  return _dev_y;
}

double Wrips::dev_z(void)
{
  return _dev_z;
}

double Wrips::set_dev_x(double x)
{
  _set_dev_x = x;
  return _set_dev_x;
}

double Wrips::set_dev_y(double y)
{
  _set_dev_y = y;
  return _set_dev_y;
}

double Wrips::set_dev_z(double z)
{
  _set_dev_z = z;
  return _set_dev_z;
}

double Wrips::dev_ori_x(double x)
{
  _dev_ori_x = x;
  return _dev_ori_x;
}

double Wrips::dev_ori_y(double y)
{
  _dev_ori_y = y;
  return _dev_ori_y;
}

double Wrips::dev_ori_z(double z)
{
  _dev_ori_z = z;
  return _dev_ori_z;
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
  _y = to_360(_y);
  _z = to_360(_z);
}

//format angle range from [-180, 180] to [0, 360]
double Wrips::to_360(double x)
{
  if (x < 0) {
    return abs(x) + 180.0;
  }
  else return x;
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

//MUST FIX
//calculate the smallest deviation away from angle
//returns positive value
void Wrips::calc_deviation(void)
{
  if (_dev_ori_x >= _x)_dev_x = abs(_dev_ori_x - _x);
  else _dev_x = abs(_x - _dev_ori_x);
  if (_dev_ori_y >= _y)_dev_y = abs(_dev_ori_y - _y);
  else _dev_y = abs(_y - _dev_ori_y);
  if (_dev_ori_z >= _z)_dev_z = abs(_dev_ori_z - _z);
  else _dev_z = abs(_z - _dev_ori_z);
}

//check if deviation is greater than threshold
char Wrips::isBeep(char x, int piez)
{
  if (x == 'x' || x == 'X') {
    if (_dev_x > _set_dev_x && _set_dev_x >= 0) {
      Serial.println("Deviation detected on X-axis");
      digitalWrite(piez, HIGH);
      return 'x';
    }
    else digitalWrite(piez, LOW);
  }
  else if (x == 'y' || x == 'Y') {
    if (_dev_y > _set_dev_y && _set_dev_y >= 0) {
      Serial.println("Deviation detected on Y-axis");
      digitalWrite(piez, HIGH);
      return 'y';
    }
    else digitalWrite(piez, LOW);
  }
  else if (x == 'z' || x == 'Z') {
    if (_dev_z > _set_dev_z && _set_dev_z >= 0) {
      Serial.println("Deviation detected on Z-axis");
      digitalWrite(piez, HIGH);
      return 'z';
    }
    else digitalWrite(piez, LOW);
  }
  else digitalWrite(piez, LOW);
}

//set the deviation with current angles
void Wrips::button_set_dev(void)
{
  _dev_ori_x = _x;
  _dev_ori_y = _y;
  _dev_ori_z = _z;
  Serial.println("Deviation origins set [x,y,z]: " + String(_dev_ori_x) + ", "
                 + String(_dev_ori_y) + ", " + String(_dev_ori_z) + ", ");
}




