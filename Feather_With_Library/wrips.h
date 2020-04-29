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
    Wrips(Adafruit_BNO055 bno, Adafruit_BNO055 bno1);
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
    double x1();
    double y1();
    double z1();
    double q_x1();
    double q_y1();
    double q_z1();
    double q_w1();
    double dev_x1();
    double dev_y1();
    double dev_z1();
    double emg1();
    double emg2();
    double emg3();
    double emg4();
    void emg();
    double set_dev_x(double x);
    double set_dev_y(double y);
    double set_dev_z(double z);
    double dev_ori_x(double x);
    double dev_ori_y(double y);
    double dev_ori_z(double z);
    double set_dev_x1(double x);
    double set_dev_y1(double y);
    double set_dev_z1(double z);
    double dev_ori_x1(double x);
    double dev_ori_y1(double y);
    double dev_ori_z1(double z);
    String begin_exercise(void);
    String end_exercise(void);
    int check_exercise(int piez);
    double to_360(double x);
    int exercise_status = 0;
    int isAvail();
    unsigned int ms();
    char isBeep(char x, int piez);
    void event();
    double correct_orientation(double xyz_old, double xyz);
    void orientation(sensors_event_t* event);
    void orientation1(sensors_event_t* event);
    void log_data(char *IMU_FILE, char *EMG_FILE);
    void print_orientation();
    void calc_deviation();
    void button_set_dev();

  private:
    Adafruit_BNO055 _bno;
    Adafruit_BNO055 _bno1;
    double _x;
    double _y;
    double _z;
    double _qx;
    double _qy;
    double _qz;
    double _qw;
    double _x1;
    double _y1;
    double _z1;
    double _qx1;
    double _qy1;
    double _qz1;
    double _qw1;
    double _emg1;
    double _emg2;
    double _emg3;
    double _emg4;
    unsigned int _buff;
    double _x_save;
    double _y_save;
    double _z_save;
    int _ms_save;
    double _x_arr[buff_size];
    double _y_arr[buff_size];
    double _z_arr[buff_size];
    double _qx_arr[buff_size];
    double _qy_arr[buff_size];
    double _qz_arr[buff_size];
    double _qw_arr[buff_size];
    double _emg1_arr[buff_size];
    double _emg2_arr[buff_size];
    double _emg3_arr[buff_size];
    double _emg4_arr[buff_size];
    int _isAvail;
    unsigned int _ms_arr[buff_size];
    //calculated deviation
    double _dev_x;
    double _dev_y;
    double _dev_z;
    double _dev_x1;
    double _dev_y1;
    double _dev_z1;
    //allowable deviation
    double _set_dev_x;
    double _set_dev_y;
    double _set_dev_z;
    double _set_dev_x1;
    double _set_dev_y1;
    double _set_dev_z1;
    //angle to deviate from
    double _dev_ori_x;
    double _dev_ori_y;
    double _dev_ori_z;
    double _dev_ori_x1;
    double _dev_ori_y1;
    double _dev_ori_z1;
    unsigned int _ms;
};

#endif

