#include "MPU6050mod.h"

MPU6050mod::MPU6050mod(){}

void MPU6050mod::Initialize(uint8_t rate_div) {
	Wire.begin();                  //begin the wire comunication 
	Wire.beginTransmission(address);  //begin, Send the slave adress (in this case 68)              
	Wire.write(0x6B);             //make the reset (place a 0 into the 6B register)
	Wire.write(0x00);
	Wire.endTransmission(true);   //end the transmission

	Wire.beginTransmission(address); 
	Wire.write(0x1C);             //We want to write to the ACCEL_CONFIG register
	Wire.write(0x00);             //00000000 (+/- 2g full scale range)
	Wire.endTransmission(true); 

	Wire.beginTransmission(address); //Set sample rate for MPU6050 to 1000kHz
	Wire.write(0x19);             
	Wire.write(rate_div);		      //1000Hz/(1+div)
	Wire.endTransmission(true);  
}

int16_t MPU6050mod::getAccelerationX(){
	Wire.beginTransmission(address);     //begin, Send the slave adress (in this case 68) 
  Wire.write(0x3B);                 //Ask for the 0x3F register- correspond to AcZ
  Wire.endTransmission(false);      //keep the transmission and next
  Wire.requestFrom(address,(size_t)2,true);    //ask for next 2 registers starting withj the 3B  
  return (int16_t)(Wire.read()<<8|Wire.read());

}

int16_t MPU6050mod::getAccelerationY(){
	Wire.beginTransmission(address);     //begin, Send the slave adress (in this case 68) 
  Wire.write(0x3D);                 //Ask for the 0x3F register- correspond to AcZ
  Wire.endTransmission(false);      //keep the transmission and next
  Wire.requestFrom(address,(size_t)2,true);    //ask for next 2 registers starting withj the 3B  
  return (int16_t)(Wire.read()<<8|Wire.read());

}

int16_t MPU6050mod::getAccelerationZ(){
	Wire.beginTransmission(address);     //begin, Send the slave adress (in this case 68) 
  Wire.write(0x3F);                 //Ask for the 0x3F register- correspond to AcZ
  Wire.endTransmission(false);      //keep the transmission and next
  Wire.requestFrom(address,(size_t)2,true);    //ask for next 2 registers starting withj the 3B  
  return (int16_t)(Wire.read()<<8|Wire.read());
}
