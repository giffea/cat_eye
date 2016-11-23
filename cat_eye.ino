// Cat Eye Project 
#include <Wire.h>
#include <SeeedOLED.h>
#define gas_sensorPin A0
#define pingPin 4
#define buzzer 7
//Motion detection
/************************ UltraSound Pin Connection Setup Start ********************************/
//  The circuit:
//  * +V connection of the PING))) attached to +5V
//  * GND connection of the PING))) attached to ground
//  * Trigger connection of the PING))) attached to digital pin 5.
//  * Output  connection of the PING))) attached to digital pin 4.
/************************ UltraSound Pin Connection Setup End ********************************/
/************************ Gas Connection Setup Start ********************************/
// Plug Gas-detector in the A0
/************************ Gas Connection Setup End ********************************/
/************************ Speaker Connection Setup Start ********************************/
// Plug one speaker in the digital pin 7
// Plug another speaker in the digital pin 8

/************************ Speaker Connection Setup End ********************************/

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  
  
}
// set initial value
int turn_on_camera_flag = 0;
int turn_on_speaker_flag = 0;
int Gas_Sensor = 0;
int user_manual_mode = 0;
long ultrasound_duration =0, ultrasound_distance_inches=0, ultrasound_distance_cm=0;
int ii=0, bee_length=0, star_length=0, toneNo=0;
int bee_duration=0,star_duration=0;
const int toneTable[7][5]={
    { 66, 131, 262, 523, 1046},  // C Do
    { 74, 147, 294, 587, 1175},  // D Re
    { 83, 165, 330, 659, 1318},  // E Mi
    { 88, 175, 349, 698, 1397},  // F Fa
    { 98, 196, 392, 784, 1568},  // G So
    {110, 220, 440, 880, 1760},  // A La
    {124, 247, 494, 988, 1976}   // B Si
};
char toneName[]="CDEFGAB";
char beeTone[]="GEEFDDCDEFGGGGEEFDDCEGGEDDDDDEFEEEEEFGGEEFDDCEGGC"; 
char starTone[]="CCGGAAGFFEEDDCGGFFEEDGGFFEEDCCGGAAGFFEEDDC";
int beeBeat[]={
    1,1,2, 1,1,2, 1,1,1,1,1,1,2,
    1,1,2, 1,1,2, 1,1,1,1,4,
    1,1,1,1,1,1,2, 1,1,1,1,1,1,2,
    1,1,2, 1,1,2, 1,1,1,1,4
};
int starBeat[]={
    1,1,1,1,1,1,2, 1,1,1,1,1,1,2,
    1,1,1,1,1,1,2, 1,1,1,1,1,1,2,
    1,1,1,1,1,1,2, 1,1,1,1,1,1,2
};
void loop() {
  /******************* UltraSound operation  Start ************************/ 
  // UltraSound operation
  // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  
  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  ultrasound_duration = pulseIn(pingPin, HIGH);
  // convert the time into a distance
  // ultrasound_distance_inches = microsecondsToInches(ultrasound_duration);
  //Serial.print(ultrasound_distance_inches);
  //Serial.print("ultrasound_distance_inches, ");
  ultrasound_distance_cm = microsecondsToCentimeters(ultrasound_duration);
  /*
  Serial.print(ultrasound_distance_cm);
  Serial.print("ultrasound_distance_cm");
  Serial.println();
  delay(100);
  */
  /******************** UltraSound operation End **********************/ 
  /******************** Gas detection operation Start ********************/ 
  pinMode(gas_sensorPin, INPUT);
  Gas_Sensor = analogRead(gas_sensorPin);
  //Check Gas in port monitor
  //Serial.print("Gas_Sensor: ");
  //Serial.print(Gas_Sensor);
  //Serial.print(" \n");
  /******************** Gas detection operation End ********************/ 
  /******************** User manual operation Start ********************/ 
  //receive MCS signal and want to cateye to turn on the camera
  //user_manual_mode = 1;
  /******************** User manual operation End ********************/ 
  /******************** Speaker operation Start ********************/ 
  //pinMode(buzzer,OUTPUT);
  //noTone(buzzer);
  /******************** Speaker operation End ********************/ 
  /******************** Status Check Start *********************************/
  // status check on port monitor
  // UltraSound 
  Serial.print(ultrasound_distance_cm);
  Serial.print(" ultrasound_distance_cm \n");
  Serial.print("turn_on_camera_flag = ");
  Serial.print(turn_on_camera_flag);
  Serial.print(" \n");
  // Gas
  Serial.print("Gas_Sensor: ");
  Serial.print( Gas_Sensor);
  Serial.print(" \n");
  /******************** Status Check End *********************************/
  /******************** Cateye Stage 1 Start *******************************/
  // Motion detected or Gas detected or User confirmation
  if ((ultrasound_distance_cm > 10) && (ultrasound_distance_cm < 50) && (ultrasound_distance_cm != 0)) { // motion detected 
    bee_speaker();
    turn_on_camera_flag = 1;
  } else if ((ultrasound_distance_cm <= 10) && (ultrasound_distance_cm != 0)) { // too closed
    phone_speaker();
    turn_on_camera_flag = 1;
  } else if (Gas_Sensor > 300) { // gas detected
    urgent_speaker();
    turn_on_camera_flag = 1;
  } else if (user_manual_mode == 1) { // user manual mode 
    star_speaker();
    turn_on_camera_flag = 1;
  }
  if (turn_on_camera_flag == 1) {
    // camera operation

    // SD card operation

    // MCS operation
  }
  /******************** Cateye Stage 1 End *********************************/
  /******************** Cateye Stage 2 Start *******************************/ 
  // Motion not detected or Gas not detected or User not confirmation
  /******************** Cateye Stage 2 End *********************************/
  if (turn_on_camera_flag == 1) { // camera work done and return to initial stage
    turn_on_camera_flag = 0;
  }
  
  
}
int getTone(char symbol) {
    int toneNo = 0;
    for ( int ii=0; ii<7; ii++ ) {
        if ( toneName[ii]==symbol ) {
            toneNo = ii;
            break;
        }
    }
    return toneNo;
}
long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

void star_speaker() {
    // star
    star_length = sizeof(starTone)-1; 
    for ( ii=0; ii<star_length; ii++ ) {
        toneNo = getTone(starTone[ii]);
        star_duration = starBeat[ii]*333;
        tone(buzzer,toneTable[toneNo][3]);
        delay(star_duration);
        noTone(buzzer);
    }
    noTone(buzzer);
    //delay(20);
}
void bee_speaker() {
    // bee
    bee_length = sizeof(beeTone)-1; 
    for ( ii=0; ii<bee_length; ii++ ) {
        toneNo = getTone(beeTone[ii]);
        bee_duration = beeBeat[ii]*333;
        tone(buzzer,toneTable[toneNo][3]);
        delay(bee_duration);
        noTone(buzzer);
    }
    //delay(20);
}
void urgent_speaker() {
    // simulate phone call
    for ( int ii=0; ii<100; ii++ ) {
        tone(buzzer,1000);
        delay(50);
        tone(buzzer,500);
        delay(50);
    }
    noTone(buzzer);
    //delay(20);
}
void phone_speaker() {
    // simulate phone call
    for ( int ii=0; ii<10; ii++ ) {
        tone(buzzer,1000);
        delay(50);
        tone(buzzer,500);
        delay(50);
    }
    noTone(buzzer);
    //delay(20);
}
