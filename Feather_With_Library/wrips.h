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
    double x();
    double y();
    double z();
    double dev_x();
    double dev_y();
    double dev_z();
    unsigned int ms();
    void event();
    void orientation(sensors_event_t* event);
    void log_orientation(char *file_name);
    void print_orientation();
    void calc_deviation(double x, double y, double z);
    
  private:
    Adafruit_BNO055 _bno;
    double _x;
    double _y;
    double _z;
    double _dev_x;
    double _dev_y;
    double _dev_z;
    unsigned int _ms;
};

#endif

