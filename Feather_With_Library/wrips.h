#ifndef Wrips_h
#define Wrips_h

#include "Arduino.h"
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define buff_size 150

class Wrips
{
  public:
    Wrips(Adafruit_BNO055 bno);
    void start();
    double x();
    double y();
    double z();
    double q_x();
    double q_y();
    double q_z();
    double q_w();
    double dev_x();
    double dev_y();
    double dev_z();
    double set_dev_x(double x);
    double set_dev_y(double y);
    double set_dev_z(double z);
    double dev_ori_x(double x);
    double dev_ori_y(double y);
    double dev_ori_z(double z);
    double to_360(double x);
    int isAvail();
    unsigned int ms();
    char isBeep(char x, int piez);
    void event();
    void orientation(sensors_event_t* event);
    void log_orientation(char *file_name);
    void print_orientation();
    void calc_deviation();
    void button_set_dev();

  private:
    Adafruit_BNO055 _bno;
    double _x;
    double _y;
    double _z;
    double _qx;
    double _qy;
    double _qz;
    double _qw;
    unsigned int _buff;
    double _x_arr[buff_size];
    double _y_arr[buff_size];
    double _z_arr[buff_size];
    double _qx_arr[buff_size];
    double _qy_arr[buff_size];
    double _qz_arr[buff_size];
    double _qw_arr[buff_size];
    int _isAvail;
    unsigned int _ms_arr[buff_size];
    //calculated deviation
    double _dev_x;
    double _dev_y;
    double _dev_z;
    //allowable deviation
    double _set_dev_x;
    double _set_dev_y;
    double _set_dev_z;
    //angle to deviate from
    double _dev_ori_x;
    double _dev_ori_y;
    double _dev_ori_z;
    unsigned int _ms;
};

#endif

