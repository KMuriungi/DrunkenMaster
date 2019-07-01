#include "SIM900.h"
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include "sms.h"
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  //lcd
SMSGSM sms;
int alert = 0;
int X = 0 , Y = 0 , Z = 0;            // variables for storing the o/p of ACCELEROMETER

/***pin connections for accelerometer***/
//const int xpin = A5;                  // x-axis of the accelerometer
//const int ypin = A4;                  // y-axis
const int zpin = A3;                  // z-axis

//Ultrasonic Sensor. 
const int pingPin = 22;               // Trigger Pin of Ultrasonic Sensor
const int echoPin = 23;               // Echo Pin of Ultrasonic Sensor

//Alcohol sensor
int value;                            // Alcohol sensor value
const int AOUTpin=A1;                  //the AOUT pin of the alcohol sensor goes into analog pin A0 of the arduino

//GSM S
int numdata;
boolean started=false;
char smsbuffer[160];
char n[20];

//debug begin
char sms_position;
char phone_number[20]; // array for the phone number string
char sms_text[160];
int i;
//debug end

int motor_Pin = 8;   //Motor Pin
int buzzer_Pin= 13;
int led_Pin = 10;
int change; 
//int led_Pin2 =7;
int sensorPin = A8;    // select the input pin for ignition
int sensorValue = 0;  // variable to store the value coming from the sensor


void ultrasonic_sensor(void);
void accelerometer(void);
//void alcohol_Sensor(void);
/*
 * 
 * INITIALIZATION
 * 
 */
void setup()
{
    
Serial.begin(9600);     //start serial port at baud rate of 9600
lcd.begin(16, 2);
pinMode(motor_Pin,OUTPUT);
pinMode(buzzer_Pin, OUTPUT);
//pinMode(led_Pin, INPUT); //CIRCUITRY IF ALTERING WITH SPARK-PLUG
//pinMode(led_Pin2, INPUT);
pinMode(AOUTpin, INPUT); 
//pinMode(xpin, INPUT); 
//pinMode(ypin, INPUT); 
pinMode(zpin, INPUT);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   SMART CAR   ");
  lcd.setCursor(0, 1);
  lcd.print("****WELCOME****");
  delay(10000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Mum: Gladys ");
  lcd.setCursor(0, 1);
  lcd.print("& fiance: Roselyn");
  delay(10000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GSM INITIALIZING ");
  lcd.setCursor(0, 1);
  lcd.print("PLEASE WAIT...  ");
  if (gsm.begin(4800)) 
    {
  started=true;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" GSM INITIALIZED ");
  delay(500);  
  } 
    else {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GSM Not Ready");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Re-check  ");
  lcd.setCursor(0, 1);
  lcd.print(" Connections...  ");
    }
if(started) 
    {
      delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("    Car is:");
  lcd.setCursor(0, 1);
  lcd.print("  Ready. ");
      delay(1000);
    }
}


/*
 * 
 * MAIN CODE
 * 
 */

void loop()
{
  if(started) 
{    
analogWrite(led_Pin, 150);
/**********************************************************************************************************************************************************************************
***********************************************************************************************************************************************************************************

Function Name : MQ-3 Alcohol Sensor
Function used to Monitor the driver's Alcohol levels. 

***********************************************************************************************************************************************************************************
**********************************************************************************************************************************************************************************/
value= analogRead(AOUTpin);//reads the analaog value from the alcohol sensor's AOUT pin
if (value > 250)
      {
    delay(300);
    analogWrite(motor_Pin, 0);
       delay(30);
    analogWrite(buzzer_Pin, 130);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Alcohol Limits: ");
    lcd.setCursor(0, 1);
    lcd.print("  HIGH !!!");
    delay(5000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("You're So Drunk!");
    lcd.setCursor(0, 1);
    lcd.print("Car is Locked..."); 
    delay(5000);
    lcd.setCursor(0, 1);
    lcd.print("Get a Taxi...!!");
    delay(100);
    sms.SendSMS("+254718605065", "Car KXX 123X driver is trying to drive while Drunk. Car Locked. Please talk to them.");
    delay(50000);
    sms.SendSMS("+254732746400", "Vehicle: KXX 123X. Violation: Trying to drive while Drunk. Red flag!!");
    delay(50000);
      }
  else 
      {
  sensorValue = analogRead(sensorPin);    
if (sensorValue>800){
     ultrasonic_sensor();//if threshold not reached, engine is engaged.
      }
      else{
        analogWrite(led_Pin, 0);
   accelerometer(); // Check if car moved
      }
    }
   }
 }
/**********************************************************************************************************************************************************************************
***********************************************************************************************************************************************************************************

Function Name : ultrasonic_sensor
Function used to Monitor the accident. 

***********************************************************************************************************************************************************************************
**********************************************************************************************************************************************************************************/
void ultrasonic_sensor(void)
      {
long duration, cm;
pinMode(pingPin, OUTPUT);
digitalWrite(pingPin, LOW);
delayMicroseconds(2);
digitalWrite(pingPin, HIGH);
delayMicroseconds(10);
digitalWrite(pingPin, LOW);
pinMode(echoPin, INPUT);
duration = pulseIn(echoPin, HIGH);
long microseconds = duration;
cm = (microseconds / 58);
delay(100);
if ((cm>31) && (cm<50))
      {

    analogWrite(motor_Pin, 128); 
     delay(150);
     analogWrite(buzzer_Pin, 128);
     delay(150);
     analogWrite(buzzer_Pin, 0);
     delay(150);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" Caution!!!");
      lcd.setCursor(0, 1);
      lcd.print(" Be carefull... ");
      }
else if ((cm>15) && (cm<30) )
      {      
    analogWrite(motor_Pin, 128); 
     delay(100);
     analogWrite(buzzer_Pin, 128);
     delay(100);
     analogWrite(buzzer_Pin, 128);
     delay(100);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   Caution!!!");
    lcd.setCursor(0, 1);
    lcd.print("Almost Colliding");
      }
else if (cm<14)
      {      
     analogWrite(buzzer_Pin, 128);
     delay(100);
     analogWrite(motor_Pin, 0);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Danger!!!");
    lcd.setCursor(0, 1);
    lcd.print(" Danger!!! ");
      }
else
      {
     analogWrite(buzzer_Pin, 0);
     delay(100);
     analogWrite(motor_Pin, 128);
      delay(100);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" Please Toto ");
      lcd.setCursor(0, 1);
      lcd.print(" Drive Safely...");
      }
      }
/**********************************************************************************************************************************************************************************
***********************************************************************************************************************************************************************************

Function Name : ACCELEROMETER
Function used to detect accident

***********************************************************************************************************************************************************************************
**********************************************************************************************************************************************************************************/
void accelerometer(void)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Car is on Halt.  ");
  analogWrite(motor_Pin, 0);
  sms_position=sms.IsSMSPresent(SMS_UNREAD);
  if (sms_position) 
   {
    sms.GetSMS(sms_position, phone_number, sms_text, 100);
            delay(2000);
            if((!strcmp(sms_text,"alert"))||(!strcmp(sms_text,"ALERT")))
            {
              alert=1;
            }
          }

 if ( alert == 1)
  {
 /* X = analogRead(xpin);
  delay(10);
  Y = analogRead(ypin);
  delay(10);*/
  Z = analogRead(zpin);
  delay(10);
    if(Z > 400)
    {
        sms.SendSMS("+254728210790", "ALERT!! Your Car has been moved. Check it out!! ");
          alert = 0;
          lcd.setCursor(0, 0);
          lcd.print("Vehicle moved");
        }
      }
    for(i=1;i<=20;i++)
    {
    sms.DeleteSMS(i);
    }
}
/**********************************************************************************************************************************************************************************
**********************************************************************************************************************************************************************************

The End. 

***********************************************************************************************************************************************************************************
**********************************************************************************************************************************************************************************/
