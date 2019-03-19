#include<Servo.h>
#define LS 2// left sensor
#define RS 12// right sensor
#define MS 13// midle sensor
#define LM1 8// left motor
#define LM2 9// left motor
#define RM1 10// right motor
#define RM2 11// right motor


#define S0 4// frequency translation
#define S1 3// FREQUENCY TRANSLATION
#define S2 6// ACTIVATION
#define S3 5// ACTIVATION
#define sensorOut 7

Servo shoulder;
Servo elbow;
Servo finger;


int angle=0;

int redfrequency=0;
int greenfrequency=0;
int bluefrequency=0;
int redcolor=0;
int greencolor=0;
int bluecolor=0;

/*<-----Prototypes of fucntions used------->*/
int  isRed()  ;  // for color sensor code
int  isStop() ;  // to detect permanent STOP condition
void StopBot();  // to stop the bot altogether
void Move() ;    // normal functioning of bot
void Pick() ;    // robo arm code to pick the weed

/*<--------Function Definitions----------> */


       
int isRed()
  {
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);   
  redfrequency=pulseIn(sensorOut,LOW);
  redcolor=map(redfrequency,70,120,255,0);
  Serial.print("R=");
  Serial.print(redcolor);
  delay(50);   // red color

  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  greenfrequency=pulseIn(sensorOut,LOW);
  greencolor=map(greenfrequency,86,199,255,0);
  Serial.print("G=");
  Serial.print(greencolor);
  delay(50);   // green color
  
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  bluefrequency=pulseIn(sensorOut,LOW);
  bluecolor=map(bluefrequency,38,84,255,0);
  Serial.print("B=");
  Serial.print(bluecolor);
  delay(50);   // blue color

  if(redcolor>greencolor && redcolor>bluecolor)
   { Serial.println("RED DETECTED");
     return 1 ;
   }  
  if(greencolor>redcolor && greencolor>bluecolor)
   { Serial.println("GREEN DETECTED");
     return 0 ;
   }  
  if(bluecolor>redcolor && bluecolor>greencolor)
   { Serial.println("BLUE DETECTED"); 
     return 0 ;
   }  
}

int isStop()
{  if(digitalRead(LS) && digitalRead(MS)  && digitalRead(RS)) // FINAL STOP CONDITION
     return 1;
   else return 0 ;   
}

void StopBot()
{  digitalWrite(LM1,LOW) ;
   digitalWrite(LM2,LOW) ;
   digitalWrite(RM1,LOW) ;
   digitalWrite(RM2,LOW) ;
}

void Move()
{  if(digitalRead(LS) && !digitalRead(MS)  && digitalRead(RS)) //Move Forward
    {  digitalWrite(LM1,HIGH) ;
       digitalWrite(LM2,LOW) ;
       digitalWrite(RM1,HIGH) ;
       digitalWrite(RM2,LOW) ;
     }
  if(!digitalRead(LS) && !digitalRead(MS)  && digitalRead(RS)) //Move TURN
    { digitalWrite(LM1,HIGH) ;
      digitalWrite(LM2,LOW) ;
      digitalWrite(RM1,LOW) ;
      digitalWrite(RM2,LOW) ;
    }
  /*if(digitalRead(LS) && digitalRead(MS)  && digitalRead(RS)) //STOP  
    { digitalWrite(LM1,LOW) ;
      digitalWrite(LM2,LOW) ;
      digitalWrite(RM1,LOW) ;
      digitalWrite(RM2,LOW) ;
    }*/
    
  if(!digitalRead(LS) && !digitalRead(MS)  && !digitalRead(RS)) // left TYRE right turn 
    { digitalWrite(LM1,LOW) ;
      digitalWrite(LM2,LOW) ;
      digitalWrite(RM1,HIGH) ;
      digitalWrite(RM2,LOW) ;
    }
  if(digitalRead(LS) && digitalRead(MS)  && !digitalRead(RS)) // MOVE LEFT TURN 
    { digitalWrite(LM1,LOW) ;
      digitalWrite(LM2,LOW) ;
      digitalWrite(RM1,HIGH) ;
      digitalWrite(RM2,LOW) ;
    }
  if(digitalRead(LS) && !digitalRead(MS)  && !digitalRead(RS)) //Move turn
        { 
          digitalWrite(LM1,LOW) ;
          digitalWrite(LM2,LOW) ;
          digitalWrite(RM1,HIGH) ;
          digitalWrite(RM2,LOW) ;
        }
        if(!digitalRead(LS) && digitalRead(MS)  && digitalRead(RS)) //Move turn
        { 
          digitalWrite(LM1,HIGH) ;
          digitalWrite(LM2,LOW) ;
          digitalWrite(RM1,LOW) ;
          digitalWrite(RM2,LOW) ;
        }
}  
void Pick()
{  StopBot() ;

   
    for(angle=60;angle>=0;--angle)//picking
    {  
      StopBot();
       shoulder.write(angle) ;
       delay(50) ;
    }
   for(angle=0;angle<=130;++angle)//picking
    {
      StopBot();
      elbow.write(angle) ;
      delay(70) ;
    }
   
   for(angle=90;angle>=0;--angle)//picking
   { 
    StopBot();
     finger.write(angle) ;
     delay(90) ;
   }

   
   for(angle=130;angle>=90;--angle)//picking
    {
      StopBot();
      elbow.write(angle) ;
      delay(70) ;
    }
    
   for(angle=0; angle<=180;++angle)//dropping
   { 
    StopBot();
     shoulder.write(angle) ;
     delay(120) ; 
   }
   
   for(angle=0;angle<=90;++angle)//return
   {   
    StopBot();
     finger.write(angle) ;
     delay(90) ;
   }
     for(angle=90;angle>=0;--angle)
   {
    StopBot();
     elbow.write(angle) ;
     delay(120) ; 
   }
     for(angle=180;angle>=60;--angle)
   {
    StopBot();
     shoulder.write(angle) ;
     delay(120) ; 
   }
  Move() ; 
}


/* <-------- One-time-execute Function------->*/
void setup()
{

 pinMode(LS, INPUT);   // LFR SETUP
  pinMode(RS, INPUT);
  pinMode(MS, INPUT) ;
  pinMode(LM1, OUTPUT);
  pinMode(LM2, OUTPUT);
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);

  pinMode(S0,OUTPUT);   // COLOR SENSOR SETUP
  pinMode(S1,OUTPUT);
  pinMode(S2,OUTPUT);
  pinMode(S3,OUTPUT);
  pinMode(sensorOut,INPUT);
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  Serial.begin(9600);

  shoulder.attach(A0);  
  elbow.attach(A1);  // ROBO ARM SETUP
  finger.attach(A3); 
  elbow.write(0) ;
  shoulder.write(70) ;
  finger.write(0)  ;
}


/*<----------Driver Function---------> */
void loop() 
{
   if(isRed()) 
   { StopBot() ;
     Pick() ;
    // Beep() ;
   }
   if(isStop())
   { StopBot() ;
     //Beep() ;
   }
   else
     Move() ;      
}
