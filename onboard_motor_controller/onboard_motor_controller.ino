/*

Andrew Albano
Doan Tran Dong Son
12/21/2022

Onboard Motor Controller Testing

Control 1 BLDC Motor with built in driver. 
- Red wire = VCC
- Black wire = GND
- Blue wire = PWM
- White wire = DIRECTION (0 = CW, 1 = CCW)
- Yellow wire = FG (Hall Signal pulse outputs, 9 pulse/cycle)

*/

// LIBRARIES
#include<EasyButton.h>
#include <SoftwareSerial.h>

// MOTOR DEFINITIONS
#define MOTOR_PWM_L   2
#define MOTOR_PWM_R   3
#define DIR_PIN_L     22
#define DIR_PIN_R     24
// POTENTIOMETER DEFINITIONS
#define POT           A0

int zero_upper = 600;
int zero_lower = 400;


// BLUETOOTH DEFINITIONS
#define rxPin 50
#define txPin 48
SoftwareSerial HM10(rxPin, txPin); //RX pin = 16,  TX pin = 17
//char appData;
char inData;
//String inData = "";


// BUTTON DEFINITIONS AND SETUP
#define BUTTON_PIN26 26
uint8_t debounce = 0;  
bool pullup = false;
bool invert = true;
EasyButton dir_button(BUTTON_PIN26, debounce, pullup, invert);   // direction button


// modes and speeds
int mode = 1; // START IN MODE1
int drive_state = 0;
int speed = 0;

//Joystick Control
unsigned int l_yaxis;
unsigned int r_yaxis;
int left_axis;
int right_axis;
int left_motor_speed;
int right_motor_speed;

// FUNCTIONS
// general looks for joystick or button mode
/*

void bluetooth_general() {
  HM10.listen();  // listen to the HM10 port

  if(HM10.available() > 0) {   // if HM10 sends something then read
    inData = HM10.read();
    inData = String(appData);  // save the data in string format
    //Serial.write(appData);
  }
}//end bluetooth button mode receiving function
*/

// function that is called while in button mode
void bluetooth_buttonMode() {
  HM10.listen();  // listen to the HM10 port

  if(HM10.available() > 0) {   // if HM10 sends something then read
    inData = HM10.read();
    //inData = String(appData);  // save the data in string format
  }
}//end bluetooth button mode receiving function



// need a way to identify whether the signal is meant for the mode or not, maybe send the mode each time and read it first.
/*
// Bluetooth function called when in joystick mode to read 2 values
void bluetooth_JoystickMode() {
  HM10.listen();  // listen to the HM10 port

  while(HM10.available() >= 2) {   // if 2 bytes are in the buffer, read both
    l_yaxis = HM10.read(); 
    r_yaxis = HM10.read();
  }
}   // end joystick mode 
*/

void forward(){
  digitalWrite(DIR_PIN_L,LOW);        // spin left wheel ccw
  digitalWrite(DIR_PIN_R,HIGH);       // spin right wheel cw
  analogWrite(MOTOR_PWM_L,speed);
  analogWrite(MOTOR_PWM_R,speed);
}
void reverse(){
  digitalWrite(DIR_PIN_L,HIGH);       // spin left wheel cw
  digitalWrite(DIR_PIN_R,LOW);        // spin right wheel ccw
  analogWrite(MOTOR_PWM_L,speed);
  analogWrite(MOTOR_PWM_R,speed);
}

void clockwise(){
  digitalWrite(DIR_PIN_L,LOW);        // spin left wheel ccw
  digitalWrite(DIR_PIN_R,LOW);        // spin right wheel ccw
  analogWrite(MOTOR_PWM_L,speed);
  analogWrite(MOTOR_PWM_R,speed);
}

void counterclockwise(){
  digitalWrite(DIR_PIN_L,HIGH);        // spin left wheel cw
  digitalWrite(DIR_PIN_R,HIGH);        // spin right wheel cw
  analogWrite(MOTOR_PWM_L,speed);
  analogWrite(MOTOR_PWM_R,speed);
}

void off(){
  //speed=0;
  //digitalWrite(MOTOR_PWM_L,LOW);
  //digitalWrite(MOTOR_PWM_R,LOW);
  //digitalWrite(DIR_PIN_L,HIGH);        // spin left wheel cw
  //digitalWrite(DIR_PIN_R,HIGH);        // spin right wheel cw
  analogWrite(MOTOR_PWM_L,0);
  analogWrite(MOTOR_PWM_R,0);
  
}


void setup() {
  // SET BAUD RATE FOR SERIAL COMMUNICATION
  Serial.begin(9600);     // Baud rate for computer serial communication
  HM10.begin(9600);       // Baud rate for bluetooth serial communication-9600 default

  
  Serial.println("BLDC motor test");
  HM10.println("Onboard controller connected at 9600 baud rate");

  TCCR3B = TCCR3B & B11111000 | B00000001;   // for PWM frequency of 31372.55 Hz
  pinMode(3,OUTPUT);
  
  // MOTOR PIN INITIALIZATION
  pinMode(MOTOR_PWM_L, OUTPUT);
  pinMode(DIR_PIN_L, OUTPUT);
  pinMode(MOTOR_PWM_R, OUTPUT);
  pinMode(DIR_PIN_R, OUTPUT);

  // POTENTIOMETER PIN INITIALIZATION
  pinMode(POT, INPUT);

  // BUTTON PIN INITIALIZATION
  dir_button.begin();
  bool dir_state = false;
  mode=1;
}

/*
mode 1 = button mode
mode 2 = joystick mode
*/
void loop() {
  
  // READ POTENTIOMETER TO GET SPEED
  //speed=analogRead(POT);            // Read the pot
  //speed=map(speed,0,1023,0,255);    // Scale the output range
  //Speed=200; // max Speed
 
  //bool dir_state;
  // SET MOTOR DIRECTION
  // BLUETOOTH METHOD

  // BUTTON MODE
  if(mode == 1){
    digitalWrite(42, LOW);  
    speed=analogRead(POT);            // Read the pot
    
    //speed = 900;
    speed=map(speed,0,1023,0,255);    // Scale the output range
    // DETERMINE MOTOR STATE
    if(HM10.available() > 0) {   // if HM10 receives a state update, read it
      inData = HM10.read();
      // INTERPRET INPUT AND PREPARE OUTPUT STATE
      switch (inData) {
        case 'f':
          HM10.println("FORWARD");
          drive_state = 1;
          break;
          
        case 'b':
          HM10.println("REVERSE");
          drive_state = 2;
          break;
      
        case 'r':
          HM10.println("CW");
          drive_state = 3;   // rotate clockwise 
          break;
        
        case 'l':
          HM10.println("CCW");
          drive_state = 4;   // rotate counter clockwise 
          break;
        
        case 'u':
          HM10.println("SOLAR UP");
          drive_state = 5;   // expand solar panel 
          break;
          
        case 'd':
          HM10.println("SOLAR DOWN");
          drive_state = 6;   // retract solar panel
          break;

        case 't':
          HM10.println("TURBINE UP");
          drive_state = 7;   // Expand turbine          
          // turbine up
          break;
              
        case 'c':
          HM10.println("TURBINE DOWN");
          drive_state = 8;   // rotate counter clockwise 
          break;
          
        case 'o':
          HM10.println("MOTORS OFF");
          drive_state = 0;
          break;
        case '1':
          mode = 1;
          break;
        case 'z':
          mode = 2;
          HM10.println("MOTORS OFF");
          HM10.println("JOYSTICK MODE");
          drive_state = 0;
          break;
        default: break;
      }//end switch case statements
    } //end if hm10 available

    // ACTIVATE MOTOR STATE
    if(drive_state == 1){
      forward();
    }
    else if(drive_state == 2){
      reverse();
    }
    else if(drive_state == 3){
      clockwise();
    }
    else if(drive_state == 4){  
      counterclockwise();
    }
    else if(drive_state == 0){
      off();
    }
    else if(drive_state == 5){ // add code solar up
      off();     
    }
     else if(drive_state == 6){ // add code solar down
      off();     
    }
     else if(drive_state == 7){ // add code for turbines up
      off();     
    }
     else if(drive_state == 8){ // add code for turbines down
      off();     
    }
    
  }//end if mode = 1

  else if(mode == 2){
    // DEFAULT VALUE WHEN JOYSTICK IS CENTERED
    l_yaxis = 510/4;
    r_yaxis = 510/4;
  digitalWrite(42, HIGH);  
    // READ THE INCOMING DATA FROM THE JOYSTICK AAND BLUETOOTH
    if(HM10.available() >= 2){
      l_yaxis = HM10.read();
      delay(10);
      r_yaxis = HM10.read();
    }

    // CONVERT BACK TO 0-1023 RANGE FROM 0-255 RANGE
    left_axis = l_yaxis*4;
    right_axis = r_yaxis*4;
    
    // LEFT MOTOR SPEED AND DIRECTION CONTROLS
    if(left_axis > zero_upper){                  // left joystick is moved up
      digitalWrite(DIR_PIN_L,LOW);        // spin left wheel ccw to move forward
      left_motor_speed = map(left_axis, zero_upper, 1023, 0, 255);
      analogWrite(MOTOR_PWM_L,left_motor_speed);
    }
    else if(left_axis < zero_lower){             // left Joystick is moved down
      digitalWrite(DIR_PIN_L,HIGH);        // spin left wheel ccw to move forward
      left_motor_speed = map(left_axis, zero_lower, 0, 0, 255);
      
    }
    
    // RIGHT MOTOR SPEED AND DIRECTION CONTROLS
    if(right_axis > zero_upper){                  // right joystick is moved up // 550
      digitalWrite(DIR_PIN_R,HIGH);        // spin left wheel cw to move forward
      right_motor_speed = map(right_axis, zero_upper, 1023, 0, 255); 
    }
    else if(right_axis < zero_lower){             // left Joystick is moved down //470
      digitalWrite(DIR_PIN_R,LOW);        // spin left wheel ccw to move forward
      left_motor_speed = map(left_axis, zero_lower, 0, 0, 255);
    }
int zero_upper = 600;
int zero_lower = 400;
    // OUTPUT THE SPEED
    analogWrite(MOTOR_PWM_L,left_motor_speed);
    analogWrite(MOTOR_PWM_R,right_motor_speed);


} // end if mode 2
}// end loop


/*
if(inData=='f'){
        //drive_state =1;
        HM10.println("f");
        drive_state=1;
      }       
      else if(inData=='b'){
        //drive_state =1;
        HM10.println("reverse");
        drive_state=2;
      }     
            else if(inData=='o'){
        //drive_state =1;
        HM10.println("off");
        drive_state=0;
      }   
      */



/*
  }
  if (inData == "2"){

    if(mode == 2  && inData == "2"){//if mode 2 is already on, deactivate it
      
        mode=0; //turns off the mode and goes to button mode
        HM10.println("MODE 2 DEACTIVATED");
      }//end if statement for deactivating mode 2

      else{
          mode = 2;
          HM10.println("Reverse");
      }
  }

  if(inData=="f"){
    if(mode == 1 && inData == "f"){//if mode 1 is already on, deactivate it
      
        mode=0; //turns off the mode and goes to button mode
        HM10.println("MODE 1 DEACTIVATED");
      }//end if statement for deactivating mode 2

      else{
          mode = 1;
          HM10.println("Forward");
      }
  }
  
  if(inData =="b"){
    if(mode == 3 && inData == "b"){//if mode 1 is already on, deactivate it
      
        mode=0; //turns off the mode and goes to button mode
        HM10.println("Button MODE 3 ACTIVATED");
      }//end if statement for deactivating mode 2

      else{
          mode = 3;
          HM10.println("Button MODE 3 ACTIVATED");
      }
    }   

  if(mode!=0){
    if (mode == 1)
    {
      digitalWrite(DIR_PIN_L,HIGH); // SET CW
    }
    else if(mode == 2)
    {
      digitalWrite(DIR_PIN_L,LOW);  //SET CCW
    }
    else if(mode == 3){
      // PHYSICAL METHOD
      dir_button.read();                          
      bool dir_state = dir_button.isPressed();
      if(dir_state){
        digitalWrite(DIR_PIN_L,LOW); // SET CCW
      }
      else{
        digitalWrite(DIR_PIN_L,HIGH);  //SET CW
      }
    }

    analogWrite(MOTOR_PWM_L, speed);
    }
  else{
    digitalWrite(MOTOR_PWM_L, LOW);
  }
  */
