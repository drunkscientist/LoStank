/*code designed to control the continous rotation of 2 stepper motors via IR remote
 * endgoal is for an RC tank on treads or whatevs, with ps2 keyboard for testing
 */

#include <IRremote.h>
#include <PS2Keyboard.h>

//keyboard input
const int DataPin = 19; //these need to be interrupt capable or something...
const int IRQpin =  18;

//ir input
int RECV_PIN = 19;

//motor pins
const int ldir = 50;
const int rdir = 51;
const int lstep = 24;
const int rstep = 25;
const int gstep = 22; //this pin goes to BOTH step pins through diodes
const int lfault = 52;
const int rfault = 53;

//timing and motion variables
unsigned long oldtime = 0;
unsigned long ntime;
unsigned long spd = 250; // milliseconds
bool lmov = false;
//bool rmov; //might reimpliment individual toggles

//leds for debugging
const int rled = 13; // motion indication led
const int lled = 3; // error indication
const int rmled = 6; //right motor led indication
const int lmled = 5; // left motor led indication
const int gled = 4; // ostensably step led.


IRrecv irrecv(RECV_PIN);
decode_results results;
PS2Keyboard keyboard;

void setup() {
 Serial.println("setup begin");
 delay(1000);
 Serial.begin(9600);
 keyboard.begin(DataPin, IRQpin);//start the keyboard
 irrecv.enableIRIn(); // Start the IR receiver
 
 pinMode(ldir,OUTPUT);
 pinMode(rdir,OUTPUT);
 pinMode(lstep,OUTPUT);
 pinMode(rstep,OUTPUT);
 pinMode(lfault,OUTPUT);
 pinMode(rfault,OUTPUT);

 pinMode(rled,OUTPUT);
 pinMode(lled,OUTPUT);
 pinMode(rmled,OUTPUT);
 pinMode(lmled,OUTPUT);
 pinMode(gled,OUTPUT);

 digitalWrite(lfault, LOW); //default steppers to off
 digitalWrite(rfault, LOW);
 digitalWrite(ldir, LOW);
 digitalWrite(rdir, LOW); //direction assignments are kindof arbitrary

 Serial.println("setup complete");
}

void loop() {
  ntime = millis();     //update current time
  look();               //check for user input
  if(lmov == true){     //check motion flag
   go();
  }
  
  rst();                //reset step pins
}

void look(){
  if (keyboard.available){
    char c = keyboard.read();//read keyboard input

    /*
     * main keyboard keylist:
     * 
     * 1 - toggle left motor fault/sleep pin. enable or disable left side motor
     * 3 - toggle right motor fault/sleep pin. enable or disable right side motor
     * 4 - toggle left motor direction
     * 6 - toggle right motor direction
     * 5 - will trigger a step. if you hold it will continue to move
     * 8 - toggles the motion flag, will cause motion to continue
     * 7 - reduces step delay, increases rotation speed
     * 9 - increase step delay, reduce rotation speed
     */
    
    // toggle fault pins for left and right motor
    if (c == '1'){
      digitalWrite(lfault, !digitalRead(lfault));
      Serial.println("lfault");
    }
    if (c == '3'){
      digitalWrite(rfault, !digitalRead(rfault));
      Serial.println("rfault");
    }

    // toggle the direction pins for left and right motors
    if (c == '4'){
      Serial.println("left direction changer");
      digitalWrite(ldir, !digitalRead(ldir));
      
    }
    if (c == '6'){
      digitalWrite(rdir, !digitalRead(rdir));
      Serial.println("right direction changer");
    }
    if (c=='5'){
      digitalWrite(gstep, HIGH);
      Serial.println("one step");
    }

    if (c == '7'){
      spd = spd - 10;
    }

    if (c == '8'){  //stop/go
      lmov = !lmov;
      Serial.println("STOP... or go, something");
    }

    if (c == '9'){
      spd = spd + 10;
    }

    
    if (c == PS2_UPARROW){
      Serial.println("one rotation");
      for(int x = 0; x < 200; x++){
      digitalWrite(gstep, HIGH);
      digitalWrite(gled, HIGH);
      delay(spd);
      digitalWrite(gstep, LOW);
      digitalWrite(gled, LOW);
      }
    }
    if (c == PS2_DOWNARROW){
      
      digitalWrite(rfault, HIGH);
      digitalWrite(lfault, HIGH);
      Serial.println("motors: ON");
    }
  }
//  else if //IR recieve instructions to go here
}

void go(){   
  Serial.println("looking at clock...");
  if (ntime - oldtime >= spd){
    oldtime = ntime;
    digitalWrite(gstep, HIGH);
    digitalWrite(gled, HIGH);
    Serial.println("taking a step");
    }
}

void rst(){
  //reset all step pins to low
  
  digitalWrite(lstep, LOW);
  digitalWrite(rstep, LOW);
  digitalWrite(gstep, LOW);
  digitalWrite(gled, HIGH);

  digitalWrite(lmled, !digitalRead(lfault)); //update left motor led
  digitalWrite(rmled, !digitalRead(rfault));// update right motor led
  digitalWrite(rled, lmov); //motion indication led
}
