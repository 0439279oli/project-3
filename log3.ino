#include <SD.h>

File myFile;
const int CSPin = 10;

// starts empty
String file_contents = "";
// the buffer
char moves[64];

const int In1 = 4;
const int In2 = 0;
const int In3 = 2;
const int In4 = 1;
const int ENA = 5;  
const int ENB = 3;


void setup()

{
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(In3, OUTPUT);
  pinMode(In4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  pinMode(CSPin, OUTPUT);

  if (!SD.begin(CSPin)) {

// not working
  digitalWrite(LED_BUILTIN, HIGH);

    return;

  }
     
// open the file for reading
  myFile = SD.open("project");

  if (myFile) {

    // read from the file until there's nothing else in it
    while (myFile.available()) {
      // Read a single character 
      char c = myFile.read();
      // Add it to the string
      file_contents += c;  
    }
    // close the file:
    myFile.close();

  } else {

// file doesn't open
digitalWrite(LED_BUILTIN, HIGH);
  }
  delay(2000);
}
void loop(){

  file_contents.toCharArray(moves, file_contents.length());

  for (int i =0; i < file_contents.length(); i++){

   //Serial.println(moves[i]);
 switch(moves[i]){
   case 'L':
     left();
		break;
   case 'R':
     right();
		break;
   case 'F':
     forward();
    break;
   case 'B':
     back();
    break;
 }
}
}

void left()
{
  analogWrite(ENA, 98);
  analogWrite(ENB, 70);

  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
  digitalWrite(In3, LOW);
  digitalWrite(In4, HIGH);
  delay(600);
  stop_motors();
}

void right()
{
  analogWrite(ENA, 98);
  analogWrite(ENB, 70);

  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);
  digitalWrite(In3, LOW);
  digitalWrite(In4, LOW);
  delay(600);
  stop_motors();
}

void forward()
{
  analogWrite(ENA, 98);
  analogWrite(ENB, 70);

  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);
  digitalWrite(In3, LOW);
  digitalWrite(In4, HIGH);
  delay(600);
  stop_motors();
}

void back()
{
  analogWrite(ENA, 98);
  analogWrite(ENB, 70);

  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);
  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);
  delay(600);
  stop_motors();
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
