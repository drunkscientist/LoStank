/*code designed to control the continous rotation of 2 stepper motors via IR remote
 * endgoal is for an RC tank on treads or whatevs, with ps2 keyboard for testing
 */

#include <IRremote.h>
#include <PS2Keyboard.h>

//keyboard input
const int DataPin = ;
const int IRQpin =  ;

//ir input
int RECV_PIN = ;

//motor pins
const int ldir = ;
const int rdir = ;
const int lstep = ;
const int rstep = ;
const int lfault = ;
const int rfault = ;

//timing and motion variables
unsigned long oldtime = 0;
unsigned long ntime;
unsigned long spd = 25; // milliseconds
bool lmov;
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

 
}

void loop() {
  ntime = millis();
  look();
  if(lmov == TRUE){
   go();
  }
  rst();
}

void look(){
  if (keyboard.available){
    char c = keyboard.read();
    

  }
//  else if //IR recieve instructions to go here
}

void go(){
  if (ntime - oldtime >= spd){
    oldtime = ntime;
    digitalWrite(lstep, HIGH);
    }
}

void rst(){
  digitalWrite(lstep, LOW);
  digitalWrite(rstep, LOW);
}

