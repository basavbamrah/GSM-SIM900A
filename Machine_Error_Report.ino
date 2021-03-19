#include <SoftwareSerial.h>
#include <LiquidCrystal.h> 

SoftwareSerial mySerial(2,3);

// buttons..........
int red_button = 8;
int yellow_button = 9;
int green_button = 10;

// Press LED

int red_press = A4;
int yellow_press = A5;
int green_press = 13;

// 16*2 LCD...........
int Contrast=95;
LiquidCrystal lcd(12, 11, A0, A1, A2, A3);

// Message to send for different buttons

char red_str[] = "Machine has problem";
char yellow_str[] = "Repairing";
char green_str[] = "Repaired";
char fine[] = "All is Fine";


// Blink LED
int red = 4;
int yellow = 5;
int green = 7;

int sms_count1 = 0, sms_count2 = 0, sms_count3 = 0;


void setup() {
  //LED BLINK

  pinMode(red_press, OUTPUT);
  pinMode(yellow_press, OUTPUT);
  pinMode(green_press, OUTPUT);
  
  // LCD
  analogWrite(6,Contrast);
  lcd.begin(16, 2);

  // setup butttons
  pinMode(red_button, INPUT_PULLUP);
  pinMode(yellow_button, INPUT_PULLUP);
  pinMode(green_button, INPUT_PULLUP);

  // LED PINMODE
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);

  // Serial Monitor Begin
  Serial.begin(9600);
}

void sendMessage(char *message)
{
 
    //Begin serial communication with Arduino and SIM900
    mySerial.begin(9600);
  
    Serial.println("Initializing..."); 
    delay(1000);
  
    mySerial.println("AT"); //Handshaking with SIM900
    updateSerial();
  
    mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
    updateSerial();
    mySerial.println("AT+CMGS=\"+91***********\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
    updateSerial();
    mySerial.print(message); //text content
    updateSerial();
    mySerial.write(26);
    printMe("SMS sent");
   
 
}
void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}

void printMe(char *message)
{
  lcd.setCursor(0,0);
     lcd.print(message);
     delay(750);
     lcd.scrollDisplayLeft();//shifting data on LCD
     lcd.clear();
}

void red_blink()
{
    if(digitalRead(red_button)==HIGH)
    {
        digitalWrite(red_press, HIGH);
    }
    else
    {
        digitalWrite(red_press, LOW);  
    }
}

void yellow_blink()
{
    if(digitalRead(yellow_button)==HIGH)
    {
        digitalWrite(yellow_press, HIGH);    
    }
    else
    {
        digitalWrite(yellow_press, LOW);  
    }
}
void green_blink()
{
    if(digitalRead(green_button)==HIGH)
    {
        digitalWrite(green_press, HIGH);
    }
    else
    {
        digitalWrite(green_press, LOW);  
    }
  
}

void loop() {
  // put your main code here, to run repeatedly:  
  red_blink();
  yellow_blink();
  green_blink();
  
  if (digitalRead (red_button) == HIGH && digitalRead(yellow_button)==LOW && digitalRead(green_button) == LOW)
  {
      //digitalWrite(red_press, HIGH);
      digitalWrite(red, HIGH);
      Serial.println("Red Button pressed..");
      if (sms_count1<1 && digitalRead(red_button)== HIGH){
      sendMessage(red_str);  
      sms_count1++; 
        }
      printMe(red_str);
      delay(700);
  }
  else if (digitalRead (yellow_button) == HIGH && digitalRead(red_button) == HIGH && digitalRead(green_button)==LOW)
  {
      
      //digitalWrite(yellow_press, HIGH);
      digitalWrite(yellow, HIGH);
      Serial.println("Yellow Button pressed..");
        if(sms_count2<1 && digitalRead(yellow_button)== HIGH)
  {
      sendMessage(yellow_str); 
      sms_count2++;
  } 
        
      printMe(yellow_str);
      delay(700);
  }
  else if (digitalRead (green_button) == HIGH && digitalRead(red_button) == HIGH && digitalRead(yellow_button) == HIGH)
  {
      //digitalWrite(green_press, HIGH);
      digitalWrite(green, HIGH);
      Serial.println("Green Button pressed..");
       if(sms_count3<1 && digitalRead(green_button)== HIGH)
  {
      sendMessage(green_str); 
       sms_count3++;
  }
      printMe(green_str);
      delay(700);
  }
  else
  {
      digitalWrite(red, LOW);
      digitalWrite(yellow, LOW);
      digitalWrite(green, LOW);
       sms_count3=0;
      sms_count2=0;
      sms_count1=0;
      //Serial.println("No Input....");
      printMe(fine);
      delay(100);
      //lcd.clear();
  }
}
