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

unsigned long Time_robot_is_lost = 0;
bool Recovery_finished = false;

unsigned long Time_robot_is_lost2 = 0;
bool Recovery_finished2 = false;

void setup()
{
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

void loop()
{
  //read calibrated sensor values then determine the line position
  uint16_t position = line_sensor.readLineBlack(sensor_readings);

//if the robot has gone off the line
  if(position == 3000){

//start the timer and time is equal to zero, because the timer should only start once
    if(Time_robot_is_lost == 0) {
  Time_robot_is_lost = millis();
  Recovery_finished = false;
}   
//if robot has come off the line for more then 1780 miliseconds and if the recovery hasn't finsihed
  if ((millis() - Time_robot_is_lost >= 1780) && (!Recovery_finished)){
    //if it has, correct the robot
    Recovery_for3000();
    Recovery_finished = true;
} 

//if the recovery is finsihed, find the line by turning left
if(Recovery_finished){
little_left();
}

// If robot hasn't been lost for 1.1 seconds then keep trying to find the line
  else{
little_right();
}
}

//if the robot has gone off the line
  if(position == 0){

//start the timer and time is equal to zero, because the timer should only start once
    if(Time_robot_is_lost2 == 0) {
  Time_robot_is_lost2 = millis();
  Recovery_finished2 = false;
}   
//if robot has come off the line for more then 1780 miliseconds and if the recovery hasn't finished
  if ((millis() - Time_robot_is_lost2 >= 1400) && (!Recovery_finished2)){
    //if it has correct the robot
    Recovery_for0();
    Recovery_finished2 = true;
} 

//if the recovery is finsihed
if(Recovery_finished2){
little_right();
}

// If robot hasn't been lost for 1.1 seconds then keep trying to find the line
  else{
little_left();
}
}

//line is detected reset timer
else {
Time_robot_is_lost = 0; 
Time_robot_is_lost2 = 0;
Recovery_finished = false;
Recovery_finished2 = false; 

//find the line like normal
   if(position > 2000){
    
    little_right();
  }

  else if(position < 1000){

    little_left();
  }
  else{go_straight();}
}
}

void stop_motors()
{
  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
  digitalWrite(In3, LOW);
  digitalWrite(In4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
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

void Recovery_for3000()
{
  analogWrite(ENA, 108);
  analogWrite(ENB, 80);

  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);
  digitalWrite(In3, LOW);
  digitalWrite(In4, LOW);
  delay(1400);
}

void Recovery_for0()
{
  analogWrite(ENA, 108);
  analogWrite(ENB, 80);

  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);
  delay(1400);
}
