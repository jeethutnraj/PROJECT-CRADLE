// project cradle helps the working parents to get status about the baby on the room while they are working
// calls and messages are recieved on the mobile phones

#include <SoftwareSerial.h>
#define rxPin 10  //GSM Module TX pin to Arduino 2
#define txPin 11  //GSM Module RX pin to Arduino 3
const String PHONE = "+91xxxxxxxxxx"; // your phone number
int avg1;
int sum1;
int val;
int sample;
int sum2;
int level;
int flg;
int call;
int msg;
SoftwareSerial sim800(rxPin,txPin);

void setup() {
    sim800.begin(9600);// gsm enabling
  Serial.println("SIM800L software serial initialize");
  sim800.println("AT");
  Serial.begin(9600);
  pinMode (2, INPUT);  // radar sensor
  pinMode (A0, INPUT); // microphone
  Serial.println("Waiting for motion");
}
void loop() {

 for (int i = 0; i < 5; i = i + 1) 
 {                                   // get the average value of radar sensor
    val = digitalRead(2); 
    sum1 = sum1+ val;
    }  
    avg1 = sum1 / 5; 
 for(int i = 0; i<=20; i++)  
   {                                 // to get the avaerage value of the sound sensor
    sample=analogRead(A0); 
    sum2 = sum2 + sample;       
   }
   val= sum2 / 20;
   
   if((avg1 > 0) && (flg==0) && (val>35)) // check if there is movement and crying, change the (val) condition accordingly
       {
        Serial.println("baby woke up "); 
        Serial.println(sample); 
        Serial.println(avg1);           
        flg = 1;  
        call=1;    
       } 
       
       if((avg1 > 0) && (flg==0)) // check movement
       {
        Serial.println("motion detected "); 
        Serial.println(sample); 
        Serial.println(avg1);
        call=0;  
        msg=1;               
       }

        if( (val>35))  // check loud noise or crying, change the (val) condition accordingly
       {
        Serial.println("baby is crying "); 
        Serial.println(sample); 
        Serial.println(avg1);             
        msg=2;
        
       }

     if(avg1 == 0)
     {     
       flg = 0;
       Serial.println("no motion"); 
       Serial.println(val); 
     }
     
  while(sim800.available()){
  Serial.println(sim800.readString());
    }
while(Serial.available())  {
  sim800.println(Serial.readString());
}

if (call == 1) { // calling when baby cries and movement starts
    Serial.println("Motion detected!");
    Serial.println("calling....");
    delay(1000);
    sim800.println("ATD"+PHONE+";");
    delay(20000);
  }
  
if (msg == 1) { // sending msg when movement detected

  sim800.println("AT+CMGF=1");    
  delay(200);
  sim800.println("AT+CMGS=\"" +PHONE + "\"\r");
  delay(200);
  String SMS = "baby is awake";
  sim800.println(SMS);
  delay(100);
  sim800.println((char)26);
  delay(200);

      msg=0;
  }

  if (msg == 2) { // sending msg when noise detected

  sim800.println("AT+CMGF=1");    
  delay(200);

  sim800.println("AT+CMGS=\"" +PHONE + "\"\r");
  delay(200);
  String SMS = "noise detected";
  sim800.println(SMS);
  delay(100);
  sim800.println((char)26);
  delay(200);
  
      msg=0;
  }
   sum2=0;  //audio sum reset
   sum1=0;  // motion sum reset
 delay(500);
}
