/* PI control in separate program.(both work)
 This experiment will demonstrate how the PI control works in a light loop.
 A PI control is a control that is resposible in removing the error outputted in P control.
*/

#include <LiquidCrystal.h>  //include the lcd library.
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  //enable these pins on the Arduino UNO board.

int PbUP = 7;  //assign PbUP to pin 7.
int PbDOWN = 8;  //assign pbDOWN to pin 8
int Pbchange = 10;  //assign Pbchange to pin 10.
int PbADDUP = 13;  //assign Pb4 to pin 13.
int PbSUBDOWN = 6; //assign Pb5to pin 6.

int UPstate = 0;   //declare a variable to read the state of PbUP.
int DOWNstate = 0; //declare a variable to read the state of PbDOWN.
int PbCValue = 0;  //declare a variable to read the state of Pbchange.
int PbADD = 0;  //declare a variable to read the state of PbADDUP.
int PbSUB = 0;  //declare a variable to read the state of PbSUBDOWN.

int count = 0;  //declare a variable to store count.
int page = 0; //declare a variable to keep track of page.

int LDR = A0;  //assign ldr to pin A0.
int led = 9;  //assign the LED to pin PWM 9.
int CO = 0;  //declare a variable to read the CO.
int PV = 0; //read the LDR and store its value here.
int Sp = 0; //variable for SP.

int SSE = 0;  //declare a variable for the steady state error.
float pGain = 0.10;  //declare a variable for pGain.
float iGain = 0.10;  //declare a variable for iGain.
int Ti = 0;  //declare a variable for integralAction.

unsigned long previousMillis = 0; //assign a variable for previousMillis.
const long interval = 100;  //declare a variable for interval and pre-enter a value too.
unsigned long currentMillis;  //declare a variable to read the millis function.


void setup() {
Serial.begin(9600);  //set the serial communication at 9600 baudrate.
lcd.begin(16, 2);  //enable the lcd screen, 16 columns, 2 rows.

pinMode(led, OUTPUT);  //set the LED as a PWM output. 
pinMode(Pbchange, INPUT);  //set the third pushbutton(Pbchange) as digital input.
pinMode(PbUP, INPUT); //set PbUP as digital input.
pinMode(PbDOWN, INPUT);  //set PbDOWN as digital input.
pinMode(PbADD, INPUT);  //set the 4th Pb(PbADD) as digital input.
pinMode(PbSUB, INPUT);  //set the 5th Pb(PbSUB) as digital input.
}

void loop() {
  // put your main code here, to run repeatedly:
UPstate = digitalRead(PbUP);  //read the state of PbUP(pushbutton 1).
DOWNstate = digitalRead(PbDOWN); //read the state of PbDOWN(pushbutton 2).
PbCValue = digitalRead(Pbchange); //read the state of Pbchange(pushbutton 3 to change the page on the LCD screen).
currentMillis = millis();  //store the time at which the Arduino has booted up.
PbADD = digitalRead(PbADDUP); //read the state of PbADD(pushbutton 4).
PbSUB = digitalRead(PbSUBDOWN); //read the state of PbSUB(pushbutton 5).
PV = analogRead(LDR); //read the analog value coming from the LDR.


 if (page == 0)  //if the stored value in "page" is 0, do the following.
  {  

  if (PbADD == HIGH) //if PbADD is pressed, do the following:
   { if (pGain > 100)  //
      pGain = 100;  //move 100 to this variable.
     else
      pGain+= 0.01;//else increment by one.
      delay(200);  //delay for 0.2 seconds.
   }
  if (PbSUB == HIGH) //if PbHyssub is pressed, do the following:
  { if ( pGain < 0 ) //
     pGain= 0.00; //move 0 to this variable.
    else
    pGain-= 0.01;//decrement by 0.01.
    delay(200); //delay for 0.2 seconds.
  }

   lcd.setCursor(0,0);  //print the word below on the first row, first column on the LCD screen.
   lcd.print("PI>>");  //print "PI>>"
   
   lcd.setCursor(0,1);  //print "kP: " on the second row, first column.
   lcd.print("kP:");  //print "kP: "
   lcd.print(pGain);// display the value of "pGain".
   if (pGain < 100);  //delete extra character if pGain is less than 100.
   {lcd.print(" ");}

   lcd.setCursor(9,1);  //print "C.O: " on the second row, first column.
   lcd.print("C.O:");  //print "C.O: "
   lcd.print(CO);// display the value of "CO".
   if (CO < 100);  //delete extra character if CO is less than 100.
   {lcd.print("  ");}
   
  }


 if (PbCValue == HIGH)  // store the state of Pb3 in the variable "PbCValue" to be used to call each function.
 {
  { 
  page++;  //if PbCValue is TRUE, increment by one and store in the variable "page".
  delay(200);  //delay for 0.2 seconds.
  }
 }


//Section for next page.

 else if (page == 1)
 {
  if (PbADD == HIGH) //if PbADD is pressed, do the following:
   { if (iGain > 100)  //do a comaparison
     iGain = 100;  //move 100 to this variable.
     else
      iGain+= 0.01; //increment by 0.01.
      delay(200);  //delay for 0.2 seconds.
   }
  if (PbSUB == HIGH) //if Pbsub is pressed, do the following:
   { if (iGain < 0 ) //do a comparison
     iGain= 0.00; //move 0 to this variable.
    
    else
    iGain-= 0.01;//decrement by 0.01.
     delay(200); //delay for 0.2 seconds.
   }

 
 if (UPstate == HIGH)  //if PbUP(pushbutton to increment) is pressed, the code below will be executed.
  {
  if (Sp > 1022 ) // if SP is greater than 1022, store the value to Sp.
  {Sp = Sp; }  //store the value of Sp to the same variable to not exceed the max limit.
    
  else   //else add 1 to the variable count.
  Sp = count++; //increment by one and store it in the variable "Sp" to keep track of the times the PB was pressed.
 }
 
  if (DOWNstate == HIGH)  //if PbDOWN(pushbutton to decrement the count) is pressed, the code below is executed.
  {
   if (Sp < 1)  //if SP is less than 1, store the value to Sp.
   { Sp = Sp; } //store the current value of Sp back to Sp to create a cap.

   else  //else subtract 1 from the variable "count".
  Sp = count--;  // decrement by one and store it in Sp.
   } 
  
  if (currentMillis - previousMillis >= interval) //subtract currentMillis and previousMillisi, then compare it to the interval.
  {
  previousMillis = currentMillis;  //store the new value of currentMillis in the variable "previousMillis" for the next execution.
  SSE = Sp - PV;  //find the steady state error by subracting SP to PV.
  Ti = Ti + SSE;  //add the integralAction(Ti) to the SSE.
  CO = (pGain * SSE) + (iGain * Ti); //to find the CO, do the following calculation: (pGain x SSE) + (iGain x Ti).
  
//creating a cap to limit the controller output.
  if (CO > 255)  //if CO is greater than 255, do the following.
  { CO = 255; } //store the value that is greater than 254 in CO.

  if (CO < 0)  //if CO is less than 0, do the following.
  { CO = 0; }  //store the value to CO to create a cap.
  
   analogWrite(led, CO);  //use the value from the controller output to control the brightness of the LED.
  }
   Serial.print("SP: "); //print the word "SP:" in the serial monitor.
   Serial.print(Sp);  //display the value of SP.
   Serial.print(" \t");  //create a space.
   Serial.print("PV: ");  //print "PV:" in the serial monitor.
   Serial.print(PV);  //display the value of PV.
   Serial.print(" \t");  //create a space.
   Serial.print("CO: ");  //print "CO:" in the serial monitor.
   Serial.print(CO);  //display the value of CO.
   Serial.print(" \t");  //create a space.
   Serial.print("SSE: ");  //print "SSE" in the serial monitor.
   Serial.print(SSE);  //display the value of SSE.
   Serial.println(); //print these vertically.

   
   lcd.setCursor(9,0);  //print the word "PV:" on the first row, 8th column on the screen.
   lcd.print("PV:");  //print "PV: ".
   lcd.print(PV);  //display the value of PV on the lcd screen.
   if (PV < 100);  //delete extra character if PV is less than 100.
   {lcd.print(" ");}
   
   lcd.setCursor(9,1);  //print "SP: " on the second row, 8th column on the lcd screen.
   lcd.print("SP:");  //print "SP:".
   lcd.print(Sp);  //display the value of SP.
   if (Sp < 100);  //delete extra character if SP is less than 100.
   {lcd.print("           ");}

   lcd.setCursor(0,1);  //print "kI: " on the second row, first column.
   lcd.print("kI:");  //print "kI: "
   lcd.print(iGain);// display the value of "iGain".
   if (iGain < 100);  //delete extra character if iGain is less than 100.
   {lcd.print(" ");}

   lcd.setCursor(0,0);  //print "SSE: " on the second row, first column.
   lcd.print("SSE:");  //print "SSE: "
   lcd.print(SSE);// display the value of "SSE".
   if (SSE < 100);  //delete extra character if SSE is less than 100.
   {lcd.print(" ");}
     
}

else if (page == 2)
{
  page = 0;
  lcd.clear();
}

}
