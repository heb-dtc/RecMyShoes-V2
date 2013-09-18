#include <LiquidCrystal.h>
#include <PinChangeInt.h>
#include <PinChangeIntConfig.h>

#define REDLITE 10
#define GREENLITE 11
#define BLUELITE 12

#define PIN 13

/*LCD wiring
1 --> GND
2 --> 5V
3 --> S (Potar)
4 --> Pin7
5 --> GND
6 --> Pin8
7 --> 
8 --> 
9 --> 
10 -->
11 --> Pin5
12 --> Pin4
13 --> Pin3
14 --> Pin2
15 --> 5V
16 --> Pin10
17 --> Pin11
18 --> Pin12
*/

//LCD stuff
int brightness = 255;
LiquidCrystal lcd(7, 8, 5, 4, 3, 2);
boolean lcdOff;

//BTN stuff
//int btnPin = 13;
int valBtn;
              
int previous = LOW;
long time = 0;
long debounce = 600;

//TIMER stuff
int heures,minutes,secondes;
char message[16] = "";

void setup(){
   lcdOff = true;
   lcd.begin(16, 2);
   lcd.print("Not Recording");
   Serial.begin(9600);

   heures = 0;
   minutes = 0;
   secondes = 0;
   
   pinMode(PIN, INPUT);
   pinMode(REDLITE, OUTPUT);
   pinMode(GREENLITE, OUTPUT);
   pinMode(BLUELITE, OUTPUT);
   digitalWrite(PIN, HIGH);
   PCintPort::attachInterrupt(PIN, buttonRoutine, RISING);
}

void loop(){
  if(!lcdOff){
    writeTime();
  }
}

void buttonRoutine(){
  valBtn = digitalRead(PIN);
  
  if (valBtn == HIGH && previous == LOW && millis() - time > debounce) {
    //Serial.println("btn pushed");
    sendCommandOverSerial();
    startLCD();
    time = millis();
  }
  
  previous = valBtn;
}

void startLCD(){
  if(lcdOff){
    setBacklight(0, 255, 255);
    lcd.print("Recording");
    lcdOff = false;
  }
  else{
    setBacklight(255, 0, 0);
    lcd.clear();
    lcd.print("Not Recording");
    heures = 0;
    minutes = 0;
    secondes = 0;
    lcdOff = true;
  }
}

void sendCommandOverSerial(){
  Serial.println("1");
}

void setBacklight(uint8_t r, uint8_t g, uint8_t b) {
  // normalize the red LED - its brighter than the rest!
  r = map(r, 0, 255, 0, 100);
  g = map(g, 0, 255, 0, 150);
 
  r = map(r, 0, 255, 0, brightness);
  g = map(g, 0, 255, 0, brightness);
  b = map(b, 0, 255, 0, brightness);
 
  // common anode so invert!
  r = map(r, 0, 255, 255, 0);
  g = map(g, 0, 255, 255, 0);
  b = map(b, 0, 255, 255, 0);
  //Serial.print("R = "); Serial.print(r, DEC);
  //Serial.print(" G = "); Serial.print(g, DEC);
  //Serial.print(" B = "); Serial.println(b, DEC);
  analogWrite(REDLITE, r);
  analogWrite(GREENLITE, g);
  analogWrite(BLUELITE, b);
}

void writeTime(){
  lcd.clear();
  if(secondes == 60) {
    secondes = 0;
    minutes++; 
  }
  if(minutes == 60) {
    minutes = 0;
    heures++;
  }
  if(heures == 24){
    heures = 0; 
  }

  sprintf(message,"   %2d:%2d:%2d",heures,minutes,secondes);
  lcd.home();           //met le curseur en position (0;0) sur l'écran
  lcd.write(message);   //envoi le message sur l'écran
  delay(1000);         
  secondes++;
}

