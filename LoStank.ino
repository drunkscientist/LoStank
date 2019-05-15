/*code designed to control the continous rotation of 2 stepper motors via IR remote
 * endgoal is for an RC tank on treads or whatevs, with ps2 keyboard for testing
 */

#include <IRremote.h>
#include <PS2Keyboard.h>

//keyboard input
const int DataPin = 17;
const int IRQpin =  16;

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
unsigned long spd = 25; // milliseconds
bool lmov = 0;
//bool rmov; //might reimpliment individual toggles


IRrecv irrecv(RECV_PIN);
decode_results results;
PS2Keyboard keyboard;

void setup() {
 delay(1000);
 keyboard.begin(DataPin, IRQpin);//start the keyboard
 irrecv.enableIRIn(); // Start the IR receiver
 
 pinMode(ldir,OUTPUT);
 pinMode(rdir,OUTPUT);
 pinMode(lstep,OUTPUT);
 pinMode(rstep,OUTPUT);
 pinMode(lfault,OUTPUT);
 pinMode(rfault,OUTPUT);

 digitalWrite(lfault, LOW); //default steppers to off
 digitalWrite(rfault, LOW);
 digitalWrite(ldir, LOW);
 digitalWrite(rdir, LOW); //direction assignments are kindof arbitrary

 
}

void loop() {
  ntime = millis();     //update current time
  look();               //check for user input
  if(lmov == 1){     //check motion flag
   go();
  }
  rst();                //reset step pins
}

void look(){
  if (keyboard.available){
    char c = keyboard.read();//read keyboard input
    
    //q and e toggle fault pins for left and right motor
    if (c == 'q'){
      digitalWrite(lfault, !digitalRead(lfault));
    }
    if (c == 'e'){
      digitalWrite(rfault, !digitalRead(rfault));
    }

    //z and c toggle the direction pins for left and right motors
    if (c == 'z'){
      digitalWrite(ldir, !digitalRead(ldir));
    }
    if (c == 'c'){
      digitalWrite(rdir, !digitalRead(rdir));
    }

    if (c == 'w'){  //stop/go
      lmov = !lmov;
    }

    

  }
//  else if //IR recieve instructions to go here
}

void go(){
  if (ntime - oldtime >= spd){
    oldtime = ntime;
    digitalWrite(gstep, HIGH);
    }
}

void rst(){
  //reset all step pins to low
  digitalWrite(lstep, LOW);
  digitalWrite(rstep, LOW);
  digitalWrite(gstep, LOW);
}
