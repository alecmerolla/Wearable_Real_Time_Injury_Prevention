#include "Arduino.h"
#include "wrips.h"
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define EMG1 A4
#define EMG2 A7
#define EMG3 A2
#define EMG4 A3

uint16_t BNO055_SAMPLERATE_DELAY_MS = 250;

Wrips::Wrips(Adafruit_BNO055 bno, Adafruit_BNO055 bno1)
{
  //init private vars
  _bno = bno;
  _bno1 = bno1;
  _x = -100000;
  _y = -100000;
  _z = -100000;
  _x_save = -100000;
  _y_save = -100000;
  _z_save = -100000;
  _x1 = -1000000;
  _y1 = -1000000;
  _z1 = -1000000;
  _emg1 = -100000;
  _emg2 = -100000;
  _emg3 = -100000;
  _emg4 = -100000;
  _qx = -100000;
  _qy = -100000;
  _qz = -100000;
  _qw = -100000;
  _qx1 = -100000;
  _qy1 = -100000;
  _qz1 = -100000;
  _qw1 = -100000;
  _ms = 9999999;
  _ms_save = 9999999;
  _isAvail = 0;

  // for angle deviation
  _dev_x = _x;
  _dev_y = _y;
  _dev_z = _z;
  _dev_x1 = _x;
  _dev_y1 = _y;
  _dev_z1 = _z;
  _set_dev_x = 15;
  _set_dev_y = 15;
  _set_dev_z = 15;
  _set_dev_x1 = _x;
  _set_dev_y1 = _y;
  _set_dev_z1 = _z;
  _dev_ori_x = _x;
  _dev_ori_y = _y;
  _dev_ori_z = _z;
  _dev_ori_x1 = _x;
  _dev_ori_y1 = _y;
  _dev_ori_z1 = _z;
  _buff = 0;
}

void Wrips::start(void)
{
  while (!_bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected at address 0x28 ... Check your wiring or I2C ADDR!");
    //    while (1);
    delay(100);
  }
  //  if (!_bno1.begin())
  //  {
  //    /* There was a problem detecting the BNO055 ... check your connections */
  //    Serial.print("Ooops, no BNO055 detected at address 0x29 ... Check your wiring or I2C ADDR!");
  //    //    while (1);
  //  }
  delay(1000);
  pinMode(EMG1, INPUT);
  pinMode(EMG2, INPUT);
  pinMode(EMG3, INPUT);
  pinMode(EMG4, INPUT);
}

String Wrips::begin_exercise(void)
{
  exercise_status = 1;
  return "begin";
}

String Wrips::end_exercise(void)
{
  exercise_status = 0;
  return "end";
}

int Wrips::check_exercise(int piez)
{
  if (exercise_status == 2)
  return 1;
  else if (exercise_status == 1 && _dev_x < 15 && _dev_y < 15 && _dev_z < 15)
    {
      if ((millis() - _ms_save) > 4000)
      {
        exercise_status = 2;
        digitalWrite(piez, HIGH);
        return 0;
      }
      else
      {
        return 0;
      }
    }
    else
    {
      _dev_ori_x = _x;
      _dev_ori_y = _y;
      _dev_ori_z = _z;
      _ms_save = millis();
      return 0;
    }
}

// Check that new data is available
int Wrips::isAvail(void)
{
  return _isAvail;
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

double Wrips::q_x(void)
{
  return _qx;
}

double Wrips::q_y(void)
{
  return _qy;
}

double Wrips::q_z(void)
{
  return _qz;
}

double Wrips::q_w(void)
{
  return _qw;
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

double Wrips::x1(void)
{
  return _x1;
}

double Wrips::y1(void)
{
  return _y1;
}

double Wrips::z1(void)
{
  return _z1;
}

double Wrips::q_x1(void)
{
  return _qx1;
}

double Wrips::q_y1(void)
{
  return _qy1;
}

double Wrips::q_z1(void)
{
  return _qz1;
}

double Wrips::q_w1(void)
{
  return _qw1;
}

double Wrips::dev_x1(void)
{
  return _dev_x1;
}

double Wrips::dev_y1(void)
{
  return _dev_y1;
}

double Wrips::dev_z1(void)
{
  return _dev_z1;
}

double Wrips::set_dev_x1(double x)
{
  _set_dev_x1 = x;
  return _set_dev_x1;
}

double Wrips::set_dev_y1(double y)
{
  _set_dev_y1 = y;
  return _set_dev_y1;
}

double Wrips::set_dev_z1(double z)
{
  _set_dev_z1 = z;
  return _set_dev_z1;
}

double Wrips::dev_ori_x1(double x)
{
  _dev_ori_x1 = x;
  return _dev_ori_x1;
}

double Wrips::dev_ori_y1(double y)
{
  _dev_ori_y1 = y;
  return _dev_ori_y1;
}

double Wrips::dev_ori_z1(double z)
{
  _dev_ori_z1 = z;
  return _dev_ori_z1;
}

double Wrips::emg1(void)
{
  return _emg1;
}

double Wrips::emg2(void)
{
  return _emg2;
}

double Wrips::emg3(void)
{
  return _emg3;
}
double Wrips::emg4(void)
{
  return _emg4;
}


void Wrips::event(void)
{
  sensors_event_t orientationData;
  //  sensors_event_t orientationData1;
  _bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  orientation(&orientationData);
  //  delay(10);
  //  _bno1.getEvent(&orientationData1, Adafruit_BNO055::VECTOR_EULER);
  //  orientation1(&orientationData1);
  emg();
  _isAvail = 1;
}

/*
  void Wrips::event(void)
  {
  sensors_event_t event;
  _bno.getEvent(&event);
  imu::Quaternion q = _bno.getQuat();
  q.normalize();
  double test = q.x() * q.y() + q.z() * q.w();
  if (test > 0.499) { // singularity at north pole
    _x = 2 * atan2(q.x(), q.w());
    _y = M_PI / 2;
    _z = 0;
    return;
  }
  if (test < -0.499) { // singularity at south pole
    _x = -2 * atan2(q.x(), q.w());
    _y = - M_PI / 2;
    _z = 0;
    return;
  }
  double sqx = q.x() * q.x();
  double sqy = q.y() * q.y();
  double sqz = q.z() * q.z();
  _x = atan2(2 * q.y() * q.w() - 2 * q.x() * q.z() , 1 - 2 * sqy - 2 * sqz);
  _y = asin(2 * test);
  _z = atan2(2 * q.x() * q.w() - 2 * q.y() * q.z() , 1 - 2 * sqx - 2 * sqz);
  _x = 180 / M_PI * _x;
  _y = 180 / M_PI * _y;
  _z = 180 / M_PI * _z;
  //  float temp = q.x();  q.x() = -q.y();  q.y() = temp;
  //  q.z() = -q.z();
  //  Serial.print("W: = ");
  //  Serial.println(q.w());
  //  imu::Vector<3> euler = q.toEuler();
  //  _x = -180/M_PI * euler.x();
  //  _y = -180/M_PI * euler.y();
  //  _z = 180/M_PI * euler.z();
  //  _ms = millis();
  //_x = to_360(_x);
  //  _y = to_360(_y);
  //  _z = to_360(_z);
  }
*/
/*
  void Wrips::event(void)
  {
  sensors_event_t event;
  _bno.getEvent(&event);
  imu::Quaternion q = _bno.getQuat();
  q.normalize();
  _qx = q.x();
  _qy = q.y();
  _qz = q.z();
  _qw = q.w();
  _ms = millis();
  _isAvail = 1;
  }
*/

/*
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
*/

void Wrips::emg(void)
{
  _emg1 = analogRead(EMG1);
  _emg2 = analogRead(EMG2);
  _emg3 = analogRead(EMG3);
  _emg4 = analogRead(EMG4);
}

void Wrips::orientation(sensors_event_t* event)
{
  //  if (millis() < 20000)
  //  {
  _x = event->orientation.x;
  _y = event->orientation.y;
  _z = event->orientation.z;
  //  }
  //  else
  //  {
  //    _x = correct_orientation(_x, event->orientation.x);
  //    _y = correct_orientation(_y, event->orientation.y);
  //    _z = correct_orientation(_z, event->orientation.z);
  //  }
  _ms = millis();
  _y = to_360(_y);
  _z = to_360(_z);
}

void Wrips::orientation1(sensors_event_t* event)
{
  _x1 = event->orientation.x;
  _y1 = event->orientation.y;
  _z1 = event->orientation.z;
  _y1 = to_360(_y1);
  _z1 = to_360(_z1);
}

double Wrips::correct_orientation(double xyz_old, double xyz)
{
  if (abs(xyz_old - xyz) > 80.0)
  {
    if (abs(xyz_old - xyz) > 150.0)
    {
      return xyz_old;
    }
    else
    {
      return xyz_old;
    }
  }
  else
    return xyz;
}

//format angle range from [-180, 180] to [0, 360]
double Wrips::to_360(double x)
{
  if (x < 0) {
    return 360.0 - abs(x);
  }
  else return x;
}

/*
  //note when opening file you must use /test.txt
  void Wrips::log_orientation(char *file_name)
  {
  if (_buff < buff_size)
  {
    _x_arr[_buff] = _x;
    _y_arr[_buff] = _y;
    _z_arr[_buff] = _z;
    _ms_arr[_buff] = _ms;
    _buff++;
  }
  else
  {
    File file;
    file = SD.open(file_name, FILE_APPEND);
    for (int i = 0; i < buff_size; i++)
    {
      if (file) {
        file.println(String(_ms_arr[i]) + ", " + String(_x_arr[i]) + ", "
                     + String(_y_arr[i]) + ", " + String(_z_arr[i]));
      }
      else {
        Serial.println("error opening " + String(file_name));
      }
    }
    file.close();
    _buff = 0;
  }
  _isAvail = 0;
  }
*/
void Wrips::log_data(char *IMU_FILE, char *EMG_FILE)
{
  if (_buff < buff_size)
  {
    _qx_arr[_buff] = _qx;
    _qy_arr[_buff] = _qy;
    _qz_arr[_buff] = _qz;
    _qw_arr[_buff] = _qw;
    _x_arr[_buff] = _x;
    _y_arr[_buff] = _y;
    _z_arr[_buff] = _z;
    _ms_arr[_buff] = _ms;
    _emg1_arr[_buff] = _emg1;
    _emg2_arr[_buff] = _emg2;
    _emg3_arr[_buff] = _emg3;
    _emg4_arr[_buff] = _emg4;
    //    _buff++;
    Serial.println(String(_ms_arr[_buff]) + ", " + String(_x_arr[_buff]) + ", "
                   + String(_y_arr[_buff]) + ", " + String(_z_arr[_buff]));
    _buff++;
  }
  else
  {
    File file;
    // Log imu data
    file = SD.open(IMU_FILE, FILE_APPEND);
    while (!file)
    {
      file = SD.open(IMU_FILE, FILE_APPEND);
      Serial.println("ERROR IMU FILE");
    }
    for (int i = 0; i < buff_size; i++)
    {
      if (file) {
        //        file.println(String(_ms_arr[i]) + ", " + String(_qx_arr[i]) + ", "
        //                     + String(_qy_arr[i]) + ", " + String(_qz_arr[i]) + ", " + String(_qw_arr[i]));
        file.println(String(_ms_arr[i]) + ", " + String(_x_arr[i]) + ", "
                     + String(_y_arr[i]) + ", " + String(_z_arr[i]));
        Serial.println(String(_ms_arr[i]) + ", " + String(_x_arr[i]) + ", "
                       + String(_y_arr[i]) + ", " + String(_z_arr[i]));

      }
      else {
        Serial.println("error opening " + String(IMU_FILE));
      }
    }
    file.close();
    // Log emg data
    file = SD.open(EMG_FILE, FILE_APPEND);
    while (!file)
    {
      file = SD.open(EMG_FILE, FILE_APPEND);
      Serial.println("ERROR EMG FILE");
    }
    for (int i = 0; i < buff_size; i++)
    {
      if (file) {
        file.println(String(_ms_arr[i]) + ", " + String(_emg1_arr[i]) + ", "
                     + String(_emg2_arr[i]) + ", " + String(_emg3_arr[i]) + ", " + String(_emg4_arr[i]));
      }
      else {
        Serial.println("error opening " + String(EMG_FILE));
      }
    }
    file.close();
    _buff = 0;
  }
  _isAvail = 0;
}

void Wrips::print_orientation(void)
{
  Serial.println(String(_ms) + ", " + String(_x) + ", " + String(_y) + ", " + String(_z));
  //  Serial.println(String(_ms) + ", " + String(_qx) + ", "
  //                 + String(_qy) + ", " + String(_qz) + ", " + String(_qw));
  //  Serial.println(String(_ms) + ", " + String(_x1) + ", " + String(_y1) + ", " + String(_z1));
  Serial.println(String(_ms) + ", " + String(_emg1) + ", "
                 + String(_emg2) + ", " + String(_emg3) + ", " + String(_emg4));
}

//calculate the min deviation away from angle
//returns positive value
void Wrips::calc_deviation(void)
{
  double distance;
  distance = abs(_x - _dev_ori_x);
  _dev_x = min(distance, 360.0 - distance);
  distance = abs(_y - _dev_ori_y);
  _dev_y = min(distance, 360.0 - distance);
  distance = abs(_z - _dev_ori_z);
  _dev_z = min(distance, 360.0 - distance);
}

//check if deviation is greater than threshold
char Wrips::isBeep(char x, int piez)
{
  if (x == 'x' || x == 'X') {
    if (_dev_x >= _set_dev_x && _set_dev_x >= 0) {
      Serial.println("Deviation detected on X-axis");
      digitalWrite(piez, HIGH);
      return 'x';
    }
    else digitalWrite(piez, LOW);
  }
  else if (x == 'y' || x == 'Y') {
    if (_dev_y >= _set_dev_y && _set_dev_y >= 0) {
      Serial.println("Deviation detected on Y-axis");
      digitalWrite(piez, HIGH);
      return 'y';
    }
    else digitalWrite(piez, LOW);
  }
  else if (x == 'z' || x == 'Z') {
    if (_dev_z >= _set_dev_z && _set_dev_z >= 0) {
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


