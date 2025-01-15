/*
  Andrew Albano
  Created:  12/31/2022
  Updated:  3/31/2023
  
  Description:
  Handheld controller with four buttons used to control the drive state of the ASME Harvest the Sun and Wind remote operated vehicle
  
  Controller:       Arduino Nano
  Driver Required:  CH340
  Processor:        ATmega328P (Old Bootloader)
 
  Buttons:
  - 4 buttons external pull up resistor, external debounce. R1= 47 kohms, R2 = 1 Kohms, C = 0.1 uF
  - Button check hierarchy, Forward, Reverse, Right, Left
  - only one button state can be activated at a time

  HM10 Module:
  - UART Protocol
  - Voltage divider on RX pin to drop from 5 V to 3.3 V
  - Use 1 Kohm and 2.2 Kohm resistor
  - Use HM10 program for AT Commands
  - HM10 Module Name:   NU Handheld
  - Configuration:      Master

  Power Supply: 9 V battery to Vin pin
  
  Pins
  D2  -   Forward Button
  D3  -   Rotate CW Button
  D9  -   Rotate CCW Button
  D10 -   Backwards Button
  D11 -   Connected to RX on HM10 Module
  D12 -   Connected to TX on HM10 Module   
  Vin -   Positive terminal of Battery
  Gnd -   Negative terminal of Battery
  5V  -   Positive Rail
  GND -   Negative Rail

  For Debugging:
  1.  Take HM1O off the board and connect to Another Arduino UNO or NANO. Use the same voltage divider circuit
  2.  Connect RX on HM10 to pin 9
  3.  Connect TX on HM10 to pin 8
  4.  Upload HM1O AT Program
  5.  Follow HM10 program to convert to slave role
  6.  Connect to Ipad Program "BLE Terminal" to monitor output of program for debugging 
  7.  When  finished convert back to Master role

Alternatively:
Connect to serial monitor to see inputs and outputs
use LED indicator lights
    
  Future Improvements:
  - Use interrupts instead of looping
  - 2 modes: Mode 1 is buttons, Mode 2 is joystick
  - Update speed via potentiometer on handheld controller instead of onboard controller
*/

// LIBRARIES
#include <EasyButton.h>
#include <SoftwareSerial.h>


// INPUT PIN DEFINITIONS
#define BUTTON_PIN2 2 
#define BUTTON_PIN3 3 
#define BUTTON_PIN9 9
#define BUTTON_PIN10 10

// BLUETOOTH PIN DEFINITIONS
#define rxPin 12           // CONNECTED TO TX ON HM10 MODULE  // ORANGE WIRE
#define txPin 11            // CONNECTED TO RX ON HM10 MODULE // YELLOW WIRE
SoftwareSerial HM10(rxPin, txPin); 


// BUTTON SETUP
uint8_t debounce = 35;
bool pullup = false;
bool invert = true;
EasyButton fwd_button(BUTTON_PIN2, debounce, pullup, invert);     // forward button
EasyButton rev_button(BUTTON_PIN10, debounce, pullup, invert);    // reverse button
EasyButton cw_button(BUTTON_PIN3, debounce, pullup, invert);      // turn clockwise button
EasyButton ccw_button(BUTTON_PIN9, debounce, pullup, invert);     // turn counter clockwise button

// MODE STATE VARIABLES
int mode        = 0;            // Initialize the current state mode
int mode_prev   = 0;            // Initialize the previous state mode
int drive_state = 0;            // Initialize the drive state mode
int drive_state_prev = 0;       // Initialize the previous drive state


void setup() {
  HM10.write("AT+CON94A9A83B0D65");  // CONNECT TO MAC ADDRESS OF SLAVE DE
  // SET BAUD RATE FOR SERIAL COMMUNICATION VIA HM10
    HM10.begin(9600);       // Baud rate for bluetooth serial communication
  
  //HM10.write("AT+ROLE1"); // CONFIGURE AS MASTER DEVICE
  //HM10.write("AT+CON94A9A83B0D65");  // CONNECT OT MAC ADDRESS OF SLAVE DEVICE
  
  // BUTTON INITIALIZATION
  fwd_button.begin();
  rev_button.begin();
  cw_button.begin();
  ccw_button.begin();
 
  // BUTTON STATE INITIALIZATION
  bool fwd_state  =   false;
  bool cw_state   =   false;
  bool ccw_state  =   false;
  bool rev_state  =   false;

  HM10.println("Handheld controller connected at 9600 baud rate");    // Used to test connection while debugging via ipad terminal
}

void loop() {

    // READ INPUT PINS
    fwd_button.read();
    rev_button.read();
    cw_button.read();
    ccw_button.read();
    
    // STORE STATE OF PINS
    bool fwd_state  =   fwd_button.isPressed();
    bool rev_state  =   rev_button.isPressed();
    bool cw_state   =   cw_button.isPressed();
    bool ccw_state  =   ccw_button.isPressed();
  
    // SET OUTPUT
    if(fwd_state) {                           
      drive_state = 1;                                           
      if (drive_state != drive_state_prev){       //DETERMINE IF NEW INPUT MODE. IF NEW INPUT MODE IS DETECTED, UPDATE THE STATE
        HM10.println("f"); 
      }
    }    
    else if(rev_state) {                           
      drive_state = 2;                                
      if (drive_state != drive_state_prev){       //DETERMINE IF NEW INPUT MODE.  IF NEW INPUT MODE IS DETECTED, UPDATE THE STATE
        HM10.println("b"); 
      }
    }
    else if(cw_state) {                           
      drive_state = 3;                                   
      if (drive_state != drive_state_prev){       //DETERMINE IF NEW INPUT MODE.  IF NEW INPUT MODE IS DETECTED, UPDATE THE STATE
        HM10.println("r"); 
      }
    }
    else if(ccw_state) {                           
      drive_state = 4;                                 
      if (drive_state != drive_state_prev){       //DETERMINE IF NEW INPUT MODE
        HM10.println("l"); 
      }
    }
    else{
      drive_state = 0;                
      if (drive_state != drive_state_prev){       //DETERMINE IF NEW INPUT MODE
        HM10.println("o");
      }
    }  
    drive_state_prev = drive_state;               // update the state of the previous call 
}
    