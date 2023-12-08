//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#define GSM_TX_PIN 9 // GSM module TX pin
#define GSM_RX_PIN 10 // GSM module RX pin
SoftwareSerial mySerial(9, 10);



LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
const int fSensor = 3; // Pin connected to the resistor and LED
const int Buzzer = 2; // Pin connected to the buzzer
const int opSensor = 4; //for sensing open circuit

const int voltageSensorPin = A0;
float vIn;

float vOut;
float voltageSensorVal;
const float factor = 5.128;
const float vCC = 5.00;
const int currsensor = A1; // Analog pin for sensor
const int threshold = 500; // Define your threshold value here


void setup()
{
  delay(100);
  mySerial.begin(9600); // Setting the baud rate of GSM Module
  Serial.begin(9600); // Setting the baud rate of Serial Monitor (Arduino)
  pinMode(currsensor, INPUT);
  pinMode(fSensor, INPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(opSensor, INPUT);
  Serial.begin(9600); // Begin serial communication for debugging
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("ELECTRIC.POLE");
  lcd.setCursor(0,1);
  lcd.print("FAULT DETECTION");
  
  Serial.begin(9600);    // Setting the baud rate of GSM Module  
}


void loop()
{  lcd.init();                      // initialize the lcd 

  // Print a message to the LCD.
  lcd.backlight();

  //Fire
  bool value = digitalRead(fSensor);

  if (value == 0) {
    fireSMS();
    digitalWrite(Buzzer, HIGH);
    Serial.println("FIRE !!!");
    lcd.setCursor(0,0);
    lcd.print("FIRE !!!");
    delay(5000); // Delay before rechecking
  } else {
    delay(5000); // Delay before rechecking
    digitalWrite(Buzzer, LOW);
  }

  //open ckt
  int sensorValue = digitalRead(opSensor); // Read the sensor value
  
  if (sensorValue == LOW) { // If the circuit is open
  Serial.println("OPEN CIRCUIT");
    digitalWrite(Buzzer, HIGH); // Activate the buzzer
    digitalWrite(opSensor, LOW); // Turn on LED (assuming LOW is forward-biased)
    lcd.setCursor(0,0);
    lcd.print("OPEN CIRCUIT");
    opSMS();
    delay(500); // Delay for sound and LED indication
    digitalWrite(Buzzer, LOW); // Turn off the buzzer
    digitalWrite(opSensor, HIGH); // Turn off LED
    delay(500); // Delay before rechecking
  } else {
    digitalWrite(Buzzer, LOW); // Turn off the buzzer if the circuit is closed
    digitalWrite(opSensor, HIGH); // Turn off LED
}
//over voltage
voltageSensorVal = analogRead(voltageSensorPin);
  vOut = (voltageSensorVal / 1024) * vCC;

  vIn = vOut * factor;

   Serial.print("Voltage = ");
   Serial.print(vIn);
   Serial.println("V");
  if (vIn > 6.0){
    ovSMS();
    Serial.println("OVER VOLTAGE");
    digitalWrite(Buzzer, HIGH);
    delay(500);
    lcd.print("OVER VOLTAGE");
  }
  else {
    digitalWrite(Buzzer, LOW);
  }
  

   //short ckt
  int sValue = analogRead(currsensor);
  Serial.print("Current = ");
  Serial.print(sValue);
  Serial.println("A");
  if (sValue > 518) {
    scSMS();
    digitalWrite(Buzzer, HIGH);
    Serial.println("Short Circuit");
    lcd.setCursor(0,0);
   lcd.print("SHORT CIRCUIT");
  } else {
    digitalWrite(Buzzer, LOW); // Turn off LED or buzzer
  }  delay(1000);

}
void fireSMS() {
  // Use GSM library functions to send an SMS
  // Example:
  Serial.println("I am in send");
mySerial.println("AT+CMGF=1"); //Sets the GSM Module in Text Mode
delay(1000); // Delay of 1000 milli seconds or 1 second
mySerial.println("AT+CMGS=\"+916360609935\"\r"); // Replace x with mobile number
delay(1000);
mySerial.println("FIRE !!! DETECTED AT MACHE");// The SMS text you want to send
delay(100);
mySerial.println((char)26);// ASCII code of CTRL+Z
delay(1000);

}
void opSMS() {
  // Use GSM library functions to send an SMS
  // Example:
  
  Serial.println("I am in send");
  mySerial.println("AT+CMGF=1"); //Sets the GSM Module in Text Mode
  delay(1000); // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+916360609935\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("BROKEN LINE DETECTED AT UDYAMBAG");// The SMS text you want to send
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);

}
void ovSMS() {
  // Use GSM library functions to send an SMS
  // Example:
   Serial.println("I am in send");
mySerial.println("AT+CMGF=1"); //Sets the GSM Module in Text Mode
delay(1000); // Delay of 1000 milli seconds or 1 second
mySerial.println("AT+CMGS=\"+916360609935\"\r"); // Replace x with mobile number
delay(1000);
mySerial.println("OVER VOLTAGE DETECTED ! AT APMC YARD");// The SMS text you want to send
delay(100);
mySerial.println((char)26);// ASCII code of CTRL+Z
delay(1000);

   
}
void scSMS() {
  // Use GSM library functions to send an SMS
  // Example:
   Serial.println("I am in send");
mySerial.println("AT+CMGF=1"); //Sets the GSM Module in Text Mode
delay(1000); // Delay of 1000 milli seconds or 1 second
mySerial.println("AT+CMGS=\"+916360609935\"\r"); // Replace x with mobile number
delay(1000);
mySerial.println("SHORT CIRCUIT DETECTED AT RPD");// The SMS text you want to send
delay(100);
mySerial.println((char)26);// ASCII code of CTRL+Z
delay(1000);

}
