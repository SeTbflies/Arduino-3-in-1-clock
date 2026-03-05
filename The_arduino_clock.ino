

#include <LiquidCrystal.h>

#include <DHT.h>
#include <Wire.h>
#include "pitches.h"
#include <SR04.h>
#include <DS3231.h>


#define HT_PIN 13
#define TRIG_PIN 11
#define ECHO_PIN 10

const int x_pin = A0;
const int y_pin = A1;


LiquidCrystal lcd(9, 8, 4, 5, 6, 7); 
DHT ht(HT_PIN, DHT11); 
DS3231   myRTC;
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);

int date, year, month, dow, hour, minute, second;
  bool mode = false;
  bool pm = false; //just to pass to function
  bool century; //again just for you know

int melody[] = { //To be updated sooner or later
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};
int duration = 500;  // 500 miliseconds
 

int viewfunction(){  //Returns Values to change screen view
   
   int measure = analogRead(y_pin);
    delay(1000);
    if(measure > 850){
      return 2;
     
    }
    if(measure < 100){
      return 1;
      
    }
    else{
      return NULL;
    }

}

String theday(int the_day){
  if (the_day == 2){
    return "Mon";
  }
  if (the_day == 3){
    return "Tue";
  }
  if (the_day == 4){
    return "Wed";
  }
  if (the_day == 5){
    return "Thu";
  }
  if (the_day == 6){
    return "Fri";
  }
  if (the_day == 7){
    return "Sat";
  }
  if (the_day == 1){
    return "Sun";
  }
}

bool alarmplay1(int alarmdow, int alarmhour, int alarmminute, int alarmsecond){ //A new view?
  if (alarmdow == 2 || alarmdow == 3 || alarmdow == 4 || alarmdow == 5 || alarmdow == 6){
    //This works
    if (alarmhour == 6 && alarmminute == 0 && alarmsecond == 0 ){ //This is the problem, but why?
      return true;
    }
    else{
      return false;
    }
  }
}

void setup() {
 

  ht.begin(); //start the humidity/temperature sensor
  Serial.begin(9600);
  lcd.begin(25,2); //25 columns, 2 rows
  Wire.begin();
  

  //DS3231 setup  
  myRTC.setClockMode(mode); //24 hour mode
  Serial.setTimeout(10000); //Make sure the parse int accepts inputs within 10 instead of 1 second

  //Below are the time setup lines. This is for a manual input of the current time and date by the user when the code is uploaded to the Arduino board
  Serial.print("Year" );
  while(Serial.available() == 0){

  }
  year = Serial.parseInt();
  Serial.println(year);
  myRTC.setYear(year);

  Serial.print("Month ");
  while(Serial.available() == 0){

  }
  month = Serial.parseInt();
  Serial.println(month);
  myRTC.setMonth(month);

  Serial.print("Date ");
  while(Serial.available() == 0){

  }
  date = Serial.parseInt();
  Serial.println(date);
  myRTC.setDate(date);

  Serial.print("Day of Week, 1 is Sunday ");
  while(Serial.available() == 0){

  }
  dow = Serial.parseInt();
  Serial.println(dow);
  myRTC.setDoW(dow); 

  Serial.print("Hour ");
  while(Serial.available() == 0){

  }
  hour = Serial.parseInt();
  Serial.println(hour);
  myRTC.setHour(hour);

  Serial.print("Minutes ");
  while(Serial.available() == 0){

  }
  minute = Serial.parseInt();
  Serial.println(minute);
  myRTC.setMinute(minute);

  Serial.print("Seconds ");
  while(Serial.available() == 0){

  }
  second = Serial.parseInt();
  Serial.println(second);
  myRTC.setSecond(second);
} //Setup finished


void loop() {
  
  //DHT 
  //float humidity = ht.readHumidity();
  //float temperature = ht.readTemperature();

  //view 1 = clock
  //view 2 = weather stats

  //There are 4 view options here. 1, 2, 4 and Null. Null is a placeholder to ensure that the board does not leave the while loop if the joystick is
  //at a "neutral" position
   
    int view;
    bool alarm;
    view = viewfunction();
    alarm = alarmplay1(myRTC.getDoW(), myRTC.getHour(mode, pm), myRTC.getMinute(), myRTC.getSecond());
    
    while (view != 2 && alarm != true) { //1 and null. 
      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print(myRTC.getHour(mode, pm));
      lcd.print(":");
      lcd.print(myRTC.getMinute());
      lcd.print(":");
      lcd.print(myRTC.getSecond());
      lcd.setCursor(3, 1);
      
      String day = theday(dow); 
      lcd.print(day);
      lcd.print(",");
      lcd.print(myRTC.getDate());
      lcd.print("/");
      lcd.print(myRTC.getMonth(century));
      lcd.print("/");
      lcd.print(myRTC.getYear());
      view = viewfunction();
      alarm = alarmplay1(myRTC.getDoW(), myRTC.getHour(mode, pm), myRTC.getMinute(), myRTC.getSecond());
      if (alarm == true){
        break;
      }
    }
    
    
   while (view != 1 && alarm != true){ //2 and null

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Humidity: ");
    lcd.print(ht.readHumidity());
    lcd.print("%");
    lcd.setCursor(3, 1);
    lcd.print("Temp: ");
    lcd.print(ht.readTemperature());
    view = viewfunction(); //If no value received, gets out of while loop
    alarm = alarmplay1(myRTC.getDoW(), myRTC.getHour(mode, pm), myRTC.getMinute(), myRTC.getSecond());
    if (alarm == true){
      break;
    }
    }

    if (alarm == true){
    int distance; 
      do{
      for(int count = 0; count < 8; count++){ //This must work
        lcd.clear();
        lcd.setCursor(5, 0);
        lcd.print("WAKE UP");
        tone(12, melody[count], duration);
        delay(1000); //If this delay function is not here, the melody will be played at an unlimited speed, regardless of the duration. This will give a delay before repeating the melody
        }
        distance = sr04.Distance();
      } while (distance > 4);
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Alarm Stop");
      delay(5000); //Wait 5 seconds before changing back to normal views
    }

}

  


  
  

