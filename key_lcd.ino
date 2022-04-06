#include <HX711.h>  
#include<LiquidCrystal.h>
#include<Keypad.h>
#include<Servo.h>
LiquidCrystal lcd(A0,A1,A2,A3,A4,A5);
int inval=0;
int p=0;
int pos=10;
float weight; 
Servo myservo;
long val =0;
// defines pins numbers
const int trigPin = 22;
const int echoPin  =23;

// defines variables
long duration;
int distance;
const byte ROWS = 4; // Four rows for keypad
const byte COLS = 4; // Four columns for keypad
  char keys[ROWS][COLS] = {
  {'1','2','3', 'A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};                            //for keypad
byte rowPins[ROWS] = {9,8,7,6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5,4,3,2}; //connect to the column pinouts 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
HX711 cell(A9,A10);

void setup() {
  lcd.begin(16,1);
  lcd.print("ENTER AMOUNT");
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);// Sets the echoPin as an Input
  pinMode(25,INPUT);// relay pin
  Serial.begin(9600);
  myservo.attach(30);
  myservo.write(0);
  inval=GetNumber();
}

void loop() 
{
  digitalWrite(25,HIGH);
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*.034/2;

// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);
if(distance >=8 && distance<=18){
  digitalWrite(25,LOW);
  if(pos!=0){
  myservo.write(90);}
  val=cell.read();
  weight=((val-247123.00-546.00)/221353.00f*500)-41.49; //calibration
  Serial.println(weight); 
  if(weight>inval)
  {
    Serial.println("Equal");
    if(pos!=0){
    for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);
    if(pos==0)
    {
      digitalWrite(25,HIGH);
      delay(2000);
      return;
    }
  }
    }
    
    
  }
 
}
else
{ digitalWrite(25,HIGH);
}
  
}

int GetNumber()
{
   char key;
   int num=0;
   while(key != '#')
   {
      switch (key)
      {
         case NO_KEY:
            break;

         case '0': case '1': case '2': case '3': case '4':
         case '5': case '6': case '7': case '8': case '9':
            lcd.print(key);
            num = num * 10 + (key - '0');
            break;

         case '*':
            num = 0;
            lcd.clear();
            break;
      }

      key = keypad.getKey();
   }

   return num;
}

void sr(){
  int pos;
  for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);
    if(pos==0)
    {return;}// waits 15ms for the servo to reach the position
  }
}

