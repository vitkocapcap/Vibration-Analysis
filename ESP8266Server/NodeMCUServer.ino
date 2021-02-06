/*
 * @Program: NodeMCUServer 
 * @Authors: Nguyen Hong Lam Giang, Nguyen Phan Bao Viet
 * @Created on 8/2019
 */
 
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "MPU6050mod.h"

//ID and password of local wifi network
const char* ssid     = "Smartec Office";
const char* password = "Eleksmartec@";

//Variables for MPU6050 acceleration measurement
const int dataSize = 550;
double data[dataSize] = {0.00};
double accelZ = 0.00;
int pos =0;
unsigned long timeStart;
MPU6050mod mpu;

//Variables for client receiving 
const byte numChars = 10;
char receivedChars[numChars];
boolean newData = false;
char endMarker = '\n';

struct rst_info *rtc_info = system_get_rst_info(); 

//Create a server instance
WiFiServer server(80);

/*
 * Set up LED state, Serial monitor, MPU6050 and wifi feature
 */
void setup() {
  /**** Initialize LED as an indicator, will blink after finish setting up  ****/
  pinMode(2,OUTPUT);
  digitalWrite(2,LOW);
     
  Serial.begin(9600);   //Begin serial connection for debug

  /**** Wifi sign-in section ****/
  WiFi.mode(WIFI_STA); //Configure to be wifi station

  
  WiFi.begin(ssid, password); //Input ID and password  
  //Connecting wifi 
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }

  /**** Initialize MPU6050  ****/
  mpu.Initialize(0);
  Serial.println(WiFi.localIP());   //Print local IP address of ESP8266
  server.begin(); 
}


/*
 * Main program. The LED will blink for every 500ms indicating that the server is hearing for client request.
 * When a client is accepted, the LED will transit to the solid state. If the client logs out, the program will
 * switch to its previous state, waiting for another client. 
 * Noted that an internet browser can be used to access the server for debugging purpose
 * After that, send the character 'a' in the serial monitor in order for the program to read and send data 
 * The sent data will be displayed on the browser
 */
void loop() {
  /**** Waiting for a client ****/
  waitForClient();
  
  WiFiClient client = server.available();
  /**** Client found ****/
  if (client) {
    Serial.println("Client accepted");
    digitalWrite(2,LOW); //Indicate that client is accepted
    while(client.connected()) {
      //Read data from Client
      if (readData(client)||Serial.available())
      { 
        //Print any data coming from user
        if(newData){
          Serial.print("Receive from Clients: ");
          Serial.println(receivedChars);
        }
        //If the user want to read data from MPU6050
        //Send characters that begin with 'r' or enter 'a' in the serial monitor
        if ((newData == true && receivedChars[0] == 'r')||Serial.read() == 97){
          readMPU6050data();
          Serial.println(pos);
          sendMPUData(client);
        }
        newData = false;  
      }
    yield();
    }
  }
}

/*
 * Send the number of elements first, and then all the element of data array to client
 * @param WiFiClient client
 */
void sendMPUData(WiFiClient client){
  int i = 0;
  //Check if the connection is still persist 
  if(client.connected()){
    Serial.println("Ready to send data");
    //Send the number of collected data
    client.println(pos);
    //Send all the elements in data array
    while(client.connected() && i < pos){
      client.println(data[i]);
      i++;
    }
    Serial.println("Send successfully");
  }
  else Serial.println("Client disconnected");
}

/*
 * Blink LED to while waiting for client request
 */
void waitForClient(){
  unsigned long wait = millis();
  while(millis() - wait < 500){yield();}
  digitalWrite(2,!digitalRead(2));
}

/*
 * Read data from Client
 * @param WiFiClient client
 */
bool readData(WiFiClient client){
  //Check if there is data coming from Client and if the received data is new or not
  while(client.available() && newData == false){
    static byte ndx = 0;
    char rc = client.read();
    //If the end marker ('\n') is read
    if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
            ndx = numChars - 1;
        }
    }
    //Read data and store in the array
    else {
        receivedChars[ndx] = '\0'; // terminate the string
        ndx = 0;
        newData = true;
        return true;
    }
    yield();
  }
  return false;
}

/*
 * Receive MPU6050 data and store in the array
 * Sample rate 1KHz, which means for every 1ms, update data from MPU6050 once.
 * Read and store data from MPU6050 takes approximately 540us
 * make room for calculating time in while loop, which is roughly about 11-20us
 * it is safe to set sample time at around 970 -> 990us to get the sample time of 1ms (1kHz) 
 */
void readMPU6050data(){
  Serial.println("Reading data...");
  memset(data, 0, sizeof(data)); //Reset data
  pos = 0; //Reset position index
  unsigned long int timeRead = 0;
  timeStart = millis();   //Current time
  //Run for 500ms
  while(millis() - timeStart<=500){
    timeRead = micros();     
    accelZ = (double)mpu.getAccelerationZ()/16348;  //Get acceleration measurements of Z-axis 
    data[pos] = accelZ;
    pos++;
    while(micros() - timeRead < 975); //Delay for the rest amount of time
    yield();
  }
  /**** Uncomment for ebug ****/
  /*
  for(int i = 0; i < pos; i++){
    Serial.println(data[i]);
    yield();
  }
  */
}
