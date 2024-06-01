#include "HX711.h"
#include "Servo.h"

const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;
HX711 scale;

const int trigPin = 9;  
const int echoPin = 10; 
float duration, distance;

Servo servo1;
Servo servo2;

void setup() {

  Serial.begin(115200);
  // setupWeight();
  setupVolume();
  setupServo();
}

void loop() {
  // checkWeight();
  checkVolume();

}



void setupWeight(){

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read());      // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));   // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
            // by the SCALE parameter (not set yet)

  scale.set_scale(-210126);                 

  scale.tare();               // reset the scale to 0

  Serial.println("After setting up the scale:");

  Serial.print("read: \t\t");
  Serial.println(scale.read());                 // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
            // by the SCALE parameter set with set_scale

}

void setupVolume(){
  pinMode(trigPin, OUTPUT);  
	pinMode(echoPin, INPUT);  
}

void setupServo(){
  servo1.attach(7);
  servo2.attach(6);

  servo1.write(85);
  servo2.write(87);
}


void checkWeight(){
  Serial.print("one reading:\t");
  Serial.print(scale.get_units(), 1);
  Serial.print("\t| average:\t");
  Serial.println(scale.get_units(10), 5);

  if(scale.get_units() > 6){
    responseOfFull();
  }
  delay(500);
}

void checkVolume(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
  Serial.print("Distance: ");
  Serial.println(distance);

  if(distance < 8){ // 8 centimeters
    responseOfFull();
      
  }
  
  delay(100);
}

void pickUpWaste(){
  // position to ground
  //bwelo onti
  for (int pos2servo = 85; pos2servo > 45; pos2servo--) {
    servo2.write(pos2servo);
    delay(15); 
  }
  for (int pos1servo = 85; pos1servo < 173; pos1servo++){
    servo1.write(pos1servo);
    delay(15); 
  }
  delay(1000);

  // scoop
  
  for (int pos2servo = 85; pos2servo < 173; pos2servo++){
    servo2.write(pos2servo);
    delay(15); 
  }
  delay(1000);

  // ahon
  int pos2servo = 173;
  for (int pos1servo = 173; pos1servo > 55; pos1servo--){
    servo1.write(pos1servo);
    if(pos2servo>45){
      servo2.write(pos2servo);
      pos2servo--;
    }
    delay(15); 
  }
  delay(1000);

  // tapon
  for (int pos2servo = 55; pos2servo < 153; pos2servo++){
    servo2.write(pos2servo);
    delay(15); 
  }
  delay(1000);

  // back to position
  for (int pos1servo = 63; pos1servo < 85; pos1servo++){
    servo1.write(pos1servo);
    delay(15); 
  }
  for (int pos2servo = 153; pos2servo > 86; pos2servo--){
    servo2.write(pos2servo);
    delay(15); 
  }
}

void responseOfFull(){
   Serial.println("trash bin is full");
}

