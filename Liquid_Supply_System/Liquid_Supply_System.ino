/*
Project: "Liquid Supply System"
Developed by Sameer Gupta
Date: August 2022
16X2 LCD I2C Connection for NodeMCU: D1 = SCL, D2 = SDA
Buttons D5 = Start, D6 = Menu/Stop, D3 = Motor
*/


#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Preferences.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);
Preferences preferences;

int Button1 = D5, Button2 = D6, motor = D3;
unsigned int counter, percent=100, limit, value, key=1;



void setup() {

  Serial.begin(9600);
  pinMode(Button1, INPUT), pinMode(Button2, INPUT), pinMode(motor, OUTPUT);
  digitalWrite(motor, LOW);
  
  lcd.begin();
  lcd.backlight();
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Liquid Supply");
  lcd.setCursor(0,1);
  lcd.print("System");
  delay(2000);
  
  preferences.begin("my-app", false);
  counter = preferences.getUInt("counter", 0);
  Serial.printf("Saved value: %u\n", counter);
  homeScreen();
}


void loop() {
  if(!digitalRead(Button1) && key!=0){  // Regular filling
     delay(300);
     limit=counter;
     digitalWrite(motor, HIGH);
     while(digitalRead(Button2) && counter!=0){
     counter--;
     value=map(counter, 0, limit, percent, 0);
     Serial.printf("%u percent, %u counts\n", value, counter);
     lcd.clear(),lcd.setCursor(0,0),lcd.print(value),lcd.print("% filled");     
     delay(327);
     }
     digitalWrite(motor, LOW);
     counter=limit; 
     delay(2000);
     homeScreen();                         
   }

   
   if(!digitalRead(Button1) && key==0){    // For saving
      delay(300);
      counter=0;
      lcd.clear(),lcd.setCursor(0,0),lcd.print("Press STOP"),lcd.setCursor(0,1),lcd.print("to finish");
      digitalWrite(motor, HIGH);
      while(digitalRead(Button2)){
        counter++;;
        Serial.printf("counter set to %u\n", counter);
        delay(350);
             }
      digitalWrite(motor, LOW);       
      preferences.putUInt("counter", counter);
      Serial.printf("%u Saved to counter\n", counter); 
      lcd.clear(),lcd.setCursor(0,0),lcd.print("Amount Saved");       
      key=1,percent=100,delay(2000);
      homeScreen();        
   }


   if(!digitalRead(Button2)){
      delay(300);
      counter = preferences.getUInt("counter", 0);
      key++;
      if(key==1)percent=100,homeScreen();
      else if(key==2)counter=counter*2,percent=200,homeScreen();
      else if(key==3)counter=counter*3,percent=300,homeScreen();
      else if(key==4)counter=counter*4,percent=400,homeScreen();
      else if(key==5)counter=counter/2,percent=50,homeScreen();
      else if(key>5)key=0,lcd.clear(),lcd.setCursor(0,0),lcd.print("Press START to"),lcd.setCursor(0,1),lcd.print("reset 100% value");
      Serial.printf("Fill %u percent,Key=%u\n", percent, key);  
   }
 
  
} //loop end



void homeScreen(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Press START");
  lcd.setCursor(0,1);
  lcd.print("to fill "),lcd.print(percent),lcd.print('%');
}
