/*lostank 0.3
 * 
 * rewritten for ir hopefully
 * 
 * code designed to control the continous rotation of 2 stepper motors via IR remote
 * endgoal is for an RC tank on treads or whatevs, with ps2 keyboard for testing
 */

#include <IRremote.h>
//#include <PS2Keyboard.h>

//keyboard input
//const int DataPin = 19; //these need to be interrupt capable or something...
//const int IRQpin =  18;

//ir input
int RECV_PIN = 20;

//motor pins
const int ldir = 50;
const int rdir = 51;
const int lstep = 24;
const int rstep = 25;
const int gstep = 22; //this pin goes to BOTH step pins through diodes
const int lfault = 52;
const int rfault = 53;

const int bump = 12; // bumpstop
bool wall;

//timing and motion variables
unsigned long oldtime = 0;
unsigned long ntime;
unsigned long spd = 250; // milliseconds
bool autogo = false;


//leds for the lighting up
const int rled = 4; // motion indication led
const int lled = 3; // not currently used
const int rmled = 6; //right motor led indication
const int lmled = 5; // left motor led indication
const int gled = 13; // ostensably step led.


IRrecv irrecv(RECV_PIN);
decode_results results;
//PS2Keyboard keyboard;


// ir codes for the action taking
#define autoGo 0xF7609F        //aka blue
#define forwardstep 0xF7E01F   //aka white
#define leftDir 0xF710EF       //aka 1
#define rightDir 0xF750AF      //aka 3
#define leftTog 0xF730CF       //aka 4
#define rightTog 0xF7708F      //aka 6
#define speedUp 0xF700FF       //aka brightness up
#define speedDown 0xF7807F     //aka brightness down


void setup() {
Serial.println("setup begin");
 delay(1000);
 Serial.begin(9600);
 //keyboard.begin(DataPin, IRQpin);//start the keyboard
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

 pinMode(bump, INPUT);

 digitalWrite(lfault, LOW); //default steppers to off
 digitalWrite(rfault, LOW);
 digitalWrite(ldir, LOW);
 digitalWrite(rdir, HIGH); //motors oppose each other, starting opposite actually means they rotate the same way

 
 Serial.println("setup complete");
}

//------------------------------------------------------------

void loop() {
  ntime = millis();     //update current time

   
  look();     //check for user input
  
  if(autogo == true){     //check motion flag
   go();
   } 
  rst();                //reset step pins, update leds
}

//-----------------------------------------------------------

void go(){   
  Serial.println("looking at clock...");
  if (ntime - oldtime >= spd){
    oldtime = ntime;
    digitalWrite(gstep, HIGH);
    digitalWrite(gled, HIGH);
    Serial.println("taking a step");
    }
}

//____________________________________________________________
void look(){
  if (irrecv.decode(&results)){

    Serial.println("ir says");
    unsigned int value = results.value;

    switch(value) {    
      case autoGo:
        autogo = !autogo;
       break;       
      case forwardstep:
        digitalWrite(gstep, HIGH);
       break;
      case leftTog:
        digitalWrite(lfault, !digitalRead(lfault));
        Serial.println("left motor toggle");
       break;
      case rightTog:
        digitalWrite(rfault, !digitalRead(rfault));
        Serial.println("right motor toggle");
       break;
      case leftDir:
        digitalWrite(ldir, !digitalRead(ldir));
        Serial.println("left direction toggle");
       break;
      case rightDir:
        digitalWrite(rdir, !digitalRead(rdir));
        Serial.println("right direction toggle");
       break;
      case speedUp:
        spd = spd + 10;
        Serial.println("speeding up");
       break;
      case speedDown:
        spd = spd - 10;
        Serial.println("slowing down");
       break;     
    }
    irrecv.resume(); // Receive the next value
  }
}

//________________________________________________________________

void rst(){
  //reset all step pins to low
  
  digitalWrite(lstep, LOW);
  digitalWrite(rstep, LOW);
  digitalWrite(gstep, LOW);
  digitalWrite(gled, HIGH);

  digitalWrite(lmled, !digitalRead(lfault)); //update left motor led
  digitalWrite(rmled, !digitalRead(rfault));// update right motor led
  digitalWrite(rled, autogo); //motion indication led

  
}







