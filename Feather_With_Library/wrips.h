#ifndef Wrips_h
#define Wrips_h

#include "Arduino.h"
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

class Wrips
{
  public:
    Wrips(Adafruit_BNO055 bno);
    void event();
    void orientation(sensors_event_t* event);
    void log_orientation(char *file_name);
    void print_orientation();
  private:
    Adafruit_BNO055 _bno;
    double _x;
    double _y;
    double _z;
    unsigned int _ms;
};

#endif

