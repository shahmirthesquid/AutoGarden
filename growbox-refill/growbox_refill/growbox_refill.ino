/*
 * Created by 
 * Ark Bramwell, July 2010
 * Modified by keen 
 * Date: 05/05/2017
 * Function: this program will test the LCD panel and the buttons
 */ 
//Sample using LiquidCrystal library
#include <LiquidCrystal.h>
 
// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
 
// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
 
// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1500) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 500)  return btnLEFT; 
 if (adc_key_in < 700)  return btnSELECT;   
 return btnNONE;  // when all others fail, return this...
}
 

















#include "DFRobot_EC.h"
#include "DFRobot_PH.h"
#include <EEPROM.h>

#define EC_PIN A5
#define PH_PIN A4
float voltage,ecValue,temperature,voltageEC = 25.0;
DFRobot_EC ec;
DFRobot_PH ph;


#define SensorPin A4            //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.1           //deviation compensate
#define LED 13
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;

unsigned long lastpHDose;

void setup(void)
{

  for(int i =22;i<=37;i++){
    pinMode(i,OUTPUT);
    digitalWrite(i,HIGH);
  }


  lcd.begin(16, 2);              // start the library
  lcd.setCursor(0,0);
  pinMode(LED,OUTPUT);
  Serial.begin(115200);
  Serial.println("pH meter experiment!");    //Test the serial monitor
  ec.begin();
  //ph.begin();

  lastpHDose = millis();

  
}
void loop(void)
{
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue,voltage;

  // if Time to sample PH
  if(millis()-samplingTime > samplingInterval)
  {
      delay(1);
      pHArray[pHArrayIndex++]=analogRead(SensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
      pHValue = 3.5*voltage+Offset;
      samplingTime=millis();
      /*
      delay(50);
      voltage = analogRead(PH_PIN)/1024.0*5000;
      delay(50);
      pHValue = ph.readPH(voltage,25.0);
      */
  }

  // if time to sample EC
  static unsigned long timepoint = millis();
  if(millis()-timepoint>1000U)  //time interval: 1s
  {
    delay(1);
    timepoint = millis();
    voltageEC = analogRead(EC_PIN)/1024.0*5000;   // read the voltage
    //temperature = readTemperature();          // read your temperature sensor to execute temperature compensation
    //Serial.println(voltageEC);
    ecValue =  ec.readEC(voltageEC,25.0);  // convert voltage to EC with temperature compensation
  }

  //lcd.print("W1:");
  if(digitalRead(3)==LOW){
    //lcd.print("F");
    digitalWrite(22,LOW);
  }
  else{
    //lcd.print("T");
    digitalWrite(22,HIGH);
  }
  //lcd.print("W2:");
  if(pHValue>6.5  &&  (millis() - lastpHDose) > 10000){
    lastpHDose = millis();
    //lcd.print("F");
    digitalWrite(23,LOW);
    delay(2000);
    digitalWrite(23,HIGH);
  }
  else{
    //lcd.print("T");
    digitalWrite(23,HIGH);
  }
  //if time for LCD print
  if(millis() - printTime > printInterval)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
    lcd.setCursor(0,0);  
    //lcd.print("V:");
        //lcd.print(voltage);
        lcd.print(" pH:");
    lcd.println(pHValue);
        digitalWrite(LED,digitalRead(LED)^1);
        printTime=millis();
    lcd.setCursor(0,1);  
    //lcd.print(temperature,1);
    lcd.print("EC:");
    lcd.print(ecValue,2);
    Serial.println(ecValue);
    lcd.print("ms/cm");
  }
  //Serial.println(voltageEC);
  ec.calibration(voltageEC,25.0);
  //delay(10);
  //ph.calibration(voltage,25.0);
}
double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}



