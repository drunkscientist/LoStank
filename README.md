# LoStank
IR remote control of 2 stepper motors, intended for line of sight ardu-tank
//NOTE: for rled, rmled, and lmled, i used one diode off of a 5v RGB led strip with common cathode (+), logic may need to be reversed for normal leds.
currently there are 2 files, the first uses a ps2 keyboard, the goal was to have code for both simultaneously, but lack of interrupt pins means lostankB will use an IR remote. you can use any remote, just use the IRrecievdemo example included in the IRremote library to grab the codes for the buttons you want to use and replace the ones i have.
