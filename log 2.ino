#include <QTRSensors.h>

QTRSensors line_sensor;

const uint8_t SensorCount = 4;
uint16_t sensor_readings[SensorCount];

const int In1 = 4;
const int In2 = 0;
const int In3 = 2;
const int In4 = 1;
const int ENA = 5;  
const int ENB = 3;

#define S0 A0
#define S1 A1
#define S2 A2
#define S3 A3
#define sensorOut A4

// set variables to hold integers
int BlackG = 0;
int BlackR = 0;
int BlackB = 0;

int WhiteG = 0;
int WhiteR = 0;
int WhiteB = 0;

int Min_WhiteG = 500;
int Min_WhiteR = 500;
int Min_WhiteB = 500;

int Max_BlackG = 0;
int Max_BlackR = 0;
int Max_BlackB = 0;

int redValue;
int greenValue;
int blueValue;

int redPW = 0;
int greenPW = 0;
int bluePW = 0;

const int pingPin = A5;

void setup() {
 
  Serial.begin(9600);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(sensorOut, INPUT);
 
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);

  //Put the black slip infront of the colour sensor
  delay(5000);

  //Turn LED on indicating calibration for black is commensing
  digitalWrite(LED_BUILTIN, HIGH);

  //Get Black vals
  for (int i = 0; i < 28; i++)
  {
  BlackR = getRedPW();
  if(BlackR > Max_BlackR){
    Max_BlackR = BlackR;
  }
 delay(200);
 
  BlackG = getGreenPW();
  if(BlackG > Max_BlackG){
    Max_BlackG = BlackG;
  }
  delay(200);
 
  BlackB = getBluePW();
  if(BlackB > Max_BlackB){
    Max_BlackB = BlackB;
  }
  delay(200);
  }
  //Turn off LED indicating calibration for black is finished
  digitalWrite(LED_BUILTIN, LOW);

  //Put the white slip infront of the colour sensor
  delay(5000);

  //Turn LED on indicating calibration for white is commensing
  digitalWrite(LED_BUILTIN, HIGH);

//Get White vals
  for (int i = 0; i < 28; i++)
  {
  WhiteR = getRedPW();
  if(WhiteR < Min_WhiteR){
    Min_WhiteR = WhiteR;
  }
  delay(200);
 
  WhiteG = getGreenPW();
  if(WhiteG < Min_WhiteG){
    Min_WhiteG = WhiteG;
  }
  delay(200);
 
  WhiteB = getBluePW();
  if(WhiteB < Min_WhiteB){
    Min_WhiteB = WhiteB;
  }
  delay(200);
  }
  //Turn off LED indicating calibration for white is finished
  digitalWrite(LED_BUILTIN, LOW);

delay(2000);

  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(In3, OUTPUT);
  pinMode(In4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // configure the sensors
  line_sensor.setTypeRC();
  line_sensor.setSensorPins((const uint8_t[]){6, 7, 8, 9}, SensorCount);

  delay(2000);
  pinMode(LED_BUILTIN, OUTPUT);
 
//Turn on arduinos led indicating that the calibration is starting
  digitalWrite(LED_BUILTIN, HIGH);

//calibrates 400 times, which takes roughly 10 seconds
  for (uint16_t i = 0; i < 400; i++)
  {
    line_sensor.calibrate();
  }
//Turn off arduino led indicating that calibration is finished
  digitalWrite(LED_BUILTIN, LOW);

}
 
void loop() {

   // Read Red value
  redPW = getRedPW();
  // Map to value from 0-255
  redValue = map(redPW, Min_WhiteR,Max_BlackR,255,0);
  // Delay to stabilize sensor
  delay(200);
 
  // Read Green value
  greenPW = getGreenPW();
  // Map to value from 0-255
  greenValue = map(greenPW, Min_WhiteG,Max_BlackG,255,0);
  // Delay to stabilize sensor
  delay(200);
 
  // Read Blue value
  bluePW = getBluePW();
  // Map to value from 0-255
  blueValue = map(bluePW, Min_WhiteB,Max_BlackB,255,0);
  // Delay to stabilize sensor
  delay(200);

  //read calibrated sensor values then determine the line position
  uint16_t position = line_sensor.readLineBlack(sensor_readings);
 
  //if sensor senses white or black
  if((redValue > 200 && greenValue > 200 && blueValue > 200) || (redValue < 100 && greenValue < 100 && blueValue < 100)){


   if(position > 2000){
   
    little_right();
  }

  else if(position < 1000){

    little_left();
  }
  else{go_straight();}
}

else if(greenValue > redValue && greenValue > blueValue){

stop_motors();
  // A high pules is emitted for 5 microseconds
  // Gives a short low pulse prior to get a clean high response

  // Assign datatype long to both cm and duration
  long duration, cm;

  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // Change the pin mode, as we will now take in values from this pin
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
  cm = microsecondsToCentimetres(duration);

if(cm < 30)
{
  go_straight();
}
  }
}
 
// Function to read Red Pulse Widths
int getRedPW() {
 
  // Set sensor to read Red only
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;
}
 
// Function to read Green Pulse Widths
int getGreenPW() {
 
  // Set sensor to read Green only
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;
}
 
// Function to read Blue Pulse Widths
int getBluePW() {
 
  // Set sensor to read Blue only
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;
}

void stop_motors()
{
  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
  digitalWrite(In3, LOW);
  digitalWrite(In4, LOW);
}

void little_left()
{
  analogWrite(ENA, 98);
  analogWrite(ENB, 70);

  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
  digitalWrite(In3, LOW);
  digitalWrite(In4, HIGH);
  delay(60);
  stop_motors();
}

void little_right()
{
  analogWrite(ENA, 98);
  analogWrite(ENB, 70);

  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);
  digitalWrite(In3, LOW);
  digitalWrite(In4, LOW);
  delay(60);
  stop_motors();
}

void go_straight()
{
  analogWrite(ENA, 98);
  analogWrite(ENB, 70);

  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);
  digitalWrite(In3, LOW);
  digitalWrite(In4, HIGH);
}

long microsecondsToCentimetres(long microseconds)
{
  return microseconds / 29 / 2;
}

