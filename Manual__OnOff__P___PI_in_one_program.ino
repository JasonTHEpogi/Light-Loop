// Jason Ferrer - final project
/*
 This program will demonstrate how to control a light loop in four different ways. 
 The first control is called Manual control where the operator has to press the pushbuttons repeatedly to control the C.O..
 There is also the ON/OFF control which is an automatic control; meaning that the operator can control the loop away 
 from the controller by simply putting a SP.
 The third control is known as the P control which is an automatic control; however, it has few dissadvantages 
 such as it produces error in the system.
 Finally, the last control that we're going to be showing is the PI control; it is an automatic control as well in which
 the error outputted in P control will be eliminated when adding integral.
 */
 
#include <LiquidCrystal.h>  //include the lcd library.
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  //enable these pins on the Arduino UNO board.

int PbUP = 7;  //assign PbUP to pin 7.
int PbDOWN = 8;  //assign pbDOWN to pin 8.
int Pbchange = 10;  //assign Pbchange to pin 10.
int PbHysUP = 13;  //assign PbHysUP to pin 13.
int PbHysDOWN = 6; //assign PbHysUP to pin 6.


int UPstate = 0;   //declare a variable to read the state of PbUP.
int DOWNstate = 0; //declare a variable to read the state of PbDOWN.
int PbCValue = 0;  //declare a variable to read the state of Pbchange.
int PbHysADD = 0;  //declare a variable to read the state of PbHysUP .
int PbHysSUB = 0;  //declare a variable to read the state of PbHysDOWN .

int count;  //declare a variable to store the count.
int page;  //declare a variable to keep track of the page.

int LDR = A0;  //assign ldr to pin A0.
int led = 9;  //assign the LED to pin PWM 9.
int CO = 0;  //declare a variable to read the CO.
int PV = 0; //read the LDR and store its value here.

int Sp = 0;  //declare a variable for SP.
int Hys = 0;  //declare a variable fo hysteresis.
int co = 0;  //for ON/OFF CO.

float pCO = 0.0;  //declare a  variable for pController output.
float SSE = 0.0;  //declare a variable for the steady state error.
float pGain = 0.10;  //declare a variable for pGain.

float iGain = 0.10;  //declare a variable for iGain.
int Ti = 0;  //declare a variable for integralAction.
int piCO =0;// declare a variable for piController output.

unsigned long previousMillis = 0; //assign a variable for previousMillis.
const long interval = 100;  //declare a variable for interval and pre-enter a value too.
unsigned long currentMillis;  //declare a variable to read the millis function.

void setup() {
  //put your setup code here, to run once:
Serial.begin(9600);  //set the serial communication at 9600 baudrate.
lcd.begin(16, 2);  //enable the lcd screen, 16 columns, 2 rows.
  
pinMode(PbUP, INPUT); //set PbUP as digital input.
pinMode(PbDOWN, INPUT);  //set PbDOWN as digital input.
pinMode(Pbchange, INPUT);  //set the third pushbutton(Pbchange) as digital input.
pinMode(led, OUTPUT);  //set the LED as digital output.
pinMode(PbHysADD, INPUT);  //set the 4th Pb(PgBysADD) as digital input.
pinMode(PbHysSUB, INPUT);  //set the 5th Pb(PgBysSUB) as digital input.
}


void loop() {
  // put your main code here, to run repeatedly:
PV = analogRead(LDR); //read the analog value coming from the LDR.
UPstate = digitalRead(PbUP);  //read the state of PbUP(pushbutton 1).
DOWNstate = digitalRead(PbDOWN); //read the state of PbDOWN(pushbutton 2).
PbCValue = digitalRead(Pbchange); //read the state of Pbchange(pushbutton 3 to change the page on the LCD screen).
currentMillis = millis();  //store the time at which the Arduino has booted up.
PbHysADD = digitalRead(PbHysUP); //read the state of PbHysADD(pushbutton 4).
PbHysSUB = digitalRead(PbHysDOWN); //read the state of PbHysSUB(pushbutton 5).
  
//This section is to increase or decrease the CO in Manual control; also,to control the SP in ON/OFF, P & PI controls, using 2 PusButtons.
  
  if (UPstate == HIGH)  //if PbUp is pressed, do the following:
 {
   if (CO > 1023 ) // if CO is greater than 1023, store the value to CO.
  {
    CO = CO;  //store the value of CO to the same variable to not exceed the max value.
  }
  
   else  //do the condition below.
    CO = count++;  // increment by 1 and store it in the variable "CO".
    analogWrite(led, CO);  //turn on the led based on the value stored in CO.
 }
  
 
  if (DOWNstate == HIGH) //if PbDOWN is pressed, run the code below.
  {
    if (CO < 1)  //if CO is less than 1, store that value back in CO.
    { 
     CO = CO;  //store the value in CO.
    }

    else  //decrement the count by 1.
     CO = count--;  //subract 1 from value stored in the variable "count".
     analogWrite(led, CO);  //turn on the LED based on the value stored in CO.
   }  

//Hysteresis add and subtract instructions

 if (PbHysADD == HIGH) //if PbHysADD is pressed, do the following:
   { if (Hys > 100 || pGain > 100)  //if Hys or pGain is greater than 100, move 100 to the variable Hys or pgain.
      Hys, pGain, iGain = 100;  //move 100 to this variable.
     else
      Hys++; //else increment by one.
      pGain+= 0.01; //add by 0.01.
      delay(200);  //delay for 0.2 seconds.
   }
 if (PbHysSUB == HIGH) //if PbHyssub is pressed, do the following:
  { if (Hys < 0 || pGain< 0) //if Hys or pGain is less than 0, move 0 to the variable Hys or pgain.
     Hys = 0; //move 0 to this variable.
    else
     Hys--; //decrement by one.
     pGain-= 0.01; // decrement by 0.01.
     delay(200); //delay for 0.2 seconds.
  }

   
// Setting the home screen on the LCD screen and printing "Select Mode".
  if (page == 0)  //if the stored value in "page" is 0, do the following.
  {  
   lcd.setCursor(0,0);  //print the word below on the first row, first column on the LCD screen.
   lcd.print("Select MODE");  //print "Select MODE"
  }


// reading the state of the third pusbutton to change the screen pages on the LCD.
 if (PbCValue == HIGH)  // store the state of Pb3 in the variable "PbCValue" to be used to call each function.
{
 { 
  page++;  //if PbCValue is TRUE, increment by one and store in the variable "page".
  delay(200);  //delay for 0.2 seconds.
  CO = 0;  //reset the value of CO or SP to zero when pb3 is pressed.
  Hys = 0; //reset the Hysteresis to zero.
 }
  
 
 if (page == 1)  //if PbChange(pushbutton 3) is pressed once, the variable "page" will display the phrase
                 //"Manual Mode" on the screen.  
 {
  page = 1;  //if the above statement is TRUE, move 1 to variable "page" to stay on Manual Mode screen.
  lcd.clear();  //clear the lcd screen.
  lcd.setCursor(0,0);  // print the phrase below to upper left in the first row.
  lcd.print("Manual Mode"); //print "Manual Mode" on the lcd screen.
 }

  if (page == 2)  //if PbChange(pushbutton 3)is pressed twice, display the word "ON/OFF" on the lcd screen.
 {
  page = 2;  //move 2 to variable "page" to remain on that screen.
  lcd.clear();  //clear the lcd screen.
  lcd.setCursor(0,0);  //display the phrase below in the first row, first column.
  lcd.print("ON/OFF"); //print "ON/OFF" on the screen.
 }  
 
 
 if (page == 3)  //if PbChange(pushbutton 3) is pressed three times, display the word "P" on the lcd screen.
 {
  page = 3;  //move 3 to variable "page" to remain on that screen.
  lcd.clear();  //clear the screen.
  lcd.setCursor(0,0);  //display the phrase below in the first row, first column.
  lcd.print("P MODE");  //print the word "P" on the screen.
 }  
        
   if (page == 4) //if PbChange(pushbutton 3) is pressed four times, display the word "PI" on the lcd screen.
 {
  page = 4; //move 4 to variable "page" to remain on that screen
  lcd.clear(); //clear the screen.
  lcd.setCursor(0,0);  //display the phrase below in the first row, first column.
  lcd.print("PI MODE");  //print the word "PI" on the screen.
 }  

  
  if (page == 5)//if PbChange(pushbutton 3) is pressed 5 times, return to the main screen.
 { 
  page = 0; //move 0 to variable "page" to remain on that screen.
  lcd.clear(); //clear the lcd screen.
  analogWrite(led, 0); //reset the led to 0 or OFF.
 }
  
 }

  
  //Manual control section:
  else if (page == 1) //if "page" is equal to 1, run the code below.
  {
   Serial.print("CO:"); //print the word "CO:" in the serial monitor.
   Serial.print(CO);  //display the value of CO.
   Serial.print(" \t");  //create a space.
   Serial.print("PV:");  //print "PV:" in the serial monitor.
   Serial.print(PV);  //display the value of PV.
   Serial.println();  //print these vertically. 
  
  lcd.setCursor(0,1);  //display the variable on the second row, first column.
  lcd.print("PV:");  //print the word "PV: ".
  lcd.print(PV);     //print the value of PV.
  if (PV < 100);  //delete extra character if PV is less than 100.
  {lcd.print(" ");}
  
  lcd.setCursor(7,1);  //display the variable on the second row and on the 7th column.
  lcd.print("CO:");  //print "CO: " on the screen.
  lcd.print(CO);  //print the value of CO.
  if (CO < 100);  //delete extra character if CO is less than 100.
  {lcd.print(" ");}
  }
  
  
  //ONOFF control section:
  else if (page == 2)  // if page is equal to 2, execute the code below.
  {
 
 //The CO here is now the SP since I'm using CO in the Manaul Mode to control the loop, I'm just
 //going to leave the name as is.
  if (CO/*SP*/  - Hys > PV) //subtract the SP to Hysteresis and compare the result to PV.
  { co = 255;  //move 255 to co variable.
   analogWrite(led, co); } //if PV is less than SP - HYS, turn ON the actuator(led).
    
  else if (CO/*SP*/  + Hys < PV) //add the SP and Hysteresis, compare the result to PV.
   { co = 0; //move 0 to co variable.
   analogWrite(led, co); } //if PV is greater than SP + Hys, turn off the actuator(led).

   Serial.print("CO:"); //print the word "SP:" in the serial monitor.
   Serial.print(CO/*SP*/);  //display the value of SP.
   Serial.print(" \t");  //create a space.
   Serial.print("PV:");  //print "PV:" in the serial monitor.
   Serial.print(PV);  //display the value of PV.
   Serial.print(" \t");  //create a space.
   Serial.print("Hys:");  //print "Hys:" in the serial monitor.
   Serial.print(Hys);  //display the value of Hys.
   Serial.println();  //print these vertically. 
   
  lcd.setCursor(0,0); //print the word "ON/OFF" on the screen, first row, first column.
  lcd.print("ON/OFF");  //print "ON/OFF".
  lcd.setCursor(9,0);  //print the word "PV:" on the screen, second row, 9th column.
  lcd.print("PV:");  //print "PV: "
  lcd.print(PV); //display the value of PV.
  if (PV < 100);  //delete extra character if CO is less than 100.
  {lcd.print(" ");}
   
  lcd.setCursor(9,1);  //print the word "SP: " on the screen, first row, 9th column.
  lcd.print("SP:"); //print the word "SP: ".
  lcd.print(CO/*SP*/ ); //print the value of SP, use the pbUP and pbDOWN to control the SP.
  if (CO < 100);  //delete extra character if CO is less than 100.
  {lcd.print(" ");}
  
  lcd.setCursor(2,1); //print the word "Hys" on the screen, second row, first column.
  lcd.print("Hys:"); //print the word "Hys:".
  lcd.print(Hys);  //display the value of Hys.
  if (Hys < 100);  //delete extra character if Hys is less than 100.
  {lcd.print(" ");}
  }

  
  //P control section:
    else if (page == 3)  //if page is equal to 3, execute the code below.
 { 

  SSE = CO/*SP*/ - PV;  //to find the error, subtract the SP to PV, the CO here is the SP; I'm using it in Manaul control
                        //that's why I can't rename it.
  pCO = pGain * SSE; //the controller output will be the result of (pGain x Error).
  
     if (pCO > 255)  //create a cap to not go over 255.
  { pCO = 255; }  //if the controller output is more than 255, move 255 to pCO(controller output).

  if (pCO < 0)  //minimum limit to not go below zero.
  { pCO = 0; } //if pCO is less than zero, move 0 to pCO.
  analogWrite(led, pCO);  //control the intinsity of the led based upon the value od pCO.
  
  Serial.print("SP:");  //print the SP in the serial monitor.
  Serial.print(CO /*SP*/);  //print the value of SP, the CO here is the SP, I couldn't change the name since I use it in Manual.
  Serial.print(" \t"); //creat space.
  Serial.print("PV:");  //print "PV:"
  Serial.print(PV);  //print the value of PV.
  Serial.print(" \t");  //create a space.
  Serial.print("pCO: ");  //print "pCO:" in the serial monitor.
  Serial.print(pCO);  //display the value of pCO.
  Serial.println();  //print these variable vertically.
    
  lcd.setCursor(9,0);  //print the word "PV:" on the lcd screen, first row, 8th column.
  lcd.print("PV:");  //print "PV:".
  lcd.print(PV);  //display the value of PV.
  if (PV < 100);  //delete extra character if CO is less than 100.
  {lcd.print("       ");}
  
  lcd.setCursor(9,1);  //print the word "SP:" on the lcd screen, second row, 8th column.
  lcd.print("SP:");  //print "SP:".
  lcd.print(CO /*SP*/); //display the value of SP.
  if (CO < 100);  //delete extra character if CO is less than 100.
  {lcd.print("      ");}
   
  lcd.setCursor(0,1);  //print the word "pG:" on the lcd screen, second row, first column.
  lcd.print("pG:");  //print "pG: ", this is the pGAIN.
  lcd.print(pGain);  //display the value of pGain.  
  if (pGain < 100);  //delete extra character if pGain is less than 100.
   {lcd.print(" ");}
 }

 
  //PI section:   
  else if (page == 4) //if "page" is equal to 4, run the code below.
  {
    
  //Using 2 pushbuttons to change the SP; the variable "CO" here is the SP, but different name.
  if (UPstate == HIGH)  //if PbUP(pushbutton to increment) is pressed, the code below will be executed.
  {
  if (CO /*SP*/> 1023 ) // if SP is greater than 1023, store the value to CO.
  { CO /*SP*/= CO/*SP*/; }  //store the value of SP to the same variable to not exceed the max limit.
    
  else   //else add 1 to the variable count.
  CO /*SP*/ = count++; //increment by one and store it in the variable "CO" to keep track of the times the PB was pressed.
 }
 
  if (DOWNstate == HIGH)  //if PbDOWN(pushbutton to decrement the count) is pressed, the code below is executed.
  {
   if (CO /*SP*/ < 1)  //if SP is less than 1, store the value to CO.
   { CO /*SP*/= CO/*SP*/; } //store the value of CO back to CO to create a cap.

   else  //else subtract 1 from the variable "count".
  CO /*SP*/ = count--;  // decrement by one and store it in CO.
   } 
  
  if (currentMillis - previousMillis >= interval) //subtract currentMillis and previousMillisi, then compare it to the interval.
  {
  previousMillis = currentMillis;  //store the new value of currentMillis in the variable "previousMillis" for the next execution.
  SSE = CO/*SP*/ - PV;  //find the steady state error by subracting SP to PV.
  Ti = Ti + SSE;  //add the integralAction(Ti) to the SSE.
  piCO = (pGain * SSE) + (iGain * Ti); //to find the CO, do the following calculation: (pGain x SSE) + (iGain x Ti).
  
//creating a cap to limit the controller output.
  if (piCO > 255)  //if piCO is greater than 254, do the following.
  { piCO = 255; } //store the value that is greater than 254 in piCO.

  if (piCO < 0)  //if piCO is less than 1, do the following.
  { piCO = 0; }  //store the value to piCO to create a cap.
  
   analogWrite(led, piCO);  //use the value from the controller output to control the brightness of the LED.
  }
   Serial.print("SP:"); //print the word "SP:" in the serial monitor.
   Serial.print(CO/*SP*/);  //display the value of SP.
   Serial.print(" \t");  //create a space.
   Serial.print("PV:");  //print "PV:" in the serial monitor.
   Serial.print(PV);  //display the value of PV.
   Serial.print(" \t");  //create a space.
   Serial.print("piCO: ");  //print "piCO" in the serial monitor.
   Serial.print(piCO);  //display the value of piCO.
   Serial.print(" \t");  //create a space.
   Serial.print("SSE: ");  //print "SSE:" in the serial monitor.
   Serial.print(SSE);  //display the value of SSE.
   Serial.println(); //print these vertically.

   lcd.setCursor(9,0);  //print the word "PV:" on the first row, 8th column on the screen.
   lcd.print("PV:");  //print "PV: ".
   lcd.print(PV);  //display the value of PV on the lcd screen.
   if (PV < 100);  //delete extra character if PV is less than 100.
   {lcd.print(" ");}
   
   lcd.setCursor(9,1);  //print "SP: " on the second row, 8th column on the lcd screen.
   lcd.print("SP:");  //print "SP:".
   lcd.print(CO/*SP*/);  //display the value of SP.
   if (CO /*SP*/< 100);  //delete extra character if SP is less than 100.
   {lcd.print("           ");}

   lcd.setCursor(0,1);  //print "iG: " on the second row, first column.
   lcd.print("iG:");  //print "iG: "
   lcd.print(iGain);// display the value of "iGain".
   if (iGain < 100);  //delete extra character if iGain is less than 100.
   {lcd.print(" ");}
}
  
}
