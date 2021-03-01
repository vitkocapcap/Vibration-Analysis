# Vibration-Analysis

The repository includes ESP8266 server and Matlab script.
The Java Client can be found from project [Data Acquisition](https://github.com/vitkocapcap/DataAcquisition)

###Updated
All files are located here

## ESP8266Server
This small project intended to read the acceleration caused by the vibration of machine in order to determine the machine health. The measurements will be performed by MPU6050, using I2C communication, with the sample rate of 1kHz. A NodeMCU ESP8266 will also be used as a server to read and transmit the acquired data whenever there is a client request. 
Noted that, in order to maintain roughly the sample rate of 1kHz the sensor will only send the acceleration measurements of the z-axis. Also, since the server will only send raw data in time domain, a simple client application to receive data and an addition analysis tool like Matlab are necessary.  

## Java Client Application
This small Java program is used to request and receive data from the server hosted by NodeMCU ESP8266. All the received data will be written to a .txt file which will be named by the time it receives data. Noted that a user should first determine the IP Address of server in order for the application to connect. 

## Matlab Script
A Matlab script to read files and carry out FFT algorithm to determine frequency components of the signal is also provided. 
