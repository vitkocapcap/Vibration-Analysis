/*
 * @Program:  MPU6050 Library
 * @Authors: Nguyen Hong Lam Giang, Nguyen Phan Bao Viet
 * @Created on 8/2019 
 */
 
#include <Wire.h>

const uint8_t address = 0x68;

class MPU6050mod
{
  public:
    MPU6050mod();
	  void Initialize(uint8_t rate_div);
	  int16_t getAccelerationX();
    int16_t getAccelerationY();
    int16_t getAccelerationZ();
};
