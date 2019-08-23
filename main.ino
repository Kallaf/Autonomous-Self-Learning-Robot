#define S1 13 // left1 sensor
#define S2 12 // left2 sensor
#define S3 11 // center sensor
#define S4 10 // right1 sensor
#define S5 9 // right1 sensor

#define btn1 3
#define btn2 2

#define LM1 5 // left motor M1a
#define LM2 4 // left motor M2a
#define RM1 7 // right motor M2a
#define RM2 6 // right motor M2b

#define North 0
#define West 1
#define South 2
#define East 3

#define going 4
#define coming 5

typedef struct
{
  byte Entry;
  byte roads[4];
  byte rightRoad;
}crossRoad;

byte carDirection;
int mode;
boolean metion1Started;
boolean metion1Ended;
boolean metion2Started;
boolean metion2Ended;
byte counter;
byte crossRoadsSize;
crossRoad crossRoads[100];

void setup()
{
pinMode(S1, INPUT);
pinMode(S2, INPUT);
pinMode(S3, INPUT);
pinMode(S4, INPUT);
pinMode(S5, INPUT);

pinMode(btn1, INPUT);
pinMode(btn2, INPUT);

pinMode(LM1, OUTPUT);
pinMode(LM2, OUTPUT);
pinMode(RM1, OUTPUT);
pinMode(RM2, OUTPUT);

carDirection = North;
mode = going;
metion1Started=false;
metion1Ended=false;
metion2Started=false;
metion2Ended=false;
counter=0;
crossRoadsSize=-1;
}



void addCrossRoad(byte road1,byte road2,byte road3)
{
  crossRoadsSize++;
  byte Temp=(carDirection+2);
  Temp%=4;
  crossRoads[crossRoadsSize] = {Temp,{road1,road2,road3,4},0};
}

void removeCrossRoad()
{
  crossRoadsSize--;
}

void goForword()
{
  if(!(digitalRead(S3))) // Move Forward on line
{
analogWrite(LM1, 55);
digitalWrite(LM2, LOW);
digitalWrite(RM1, LOW);
analogWrite(RM2, 55);
}
else{
  if(!(digitalRead(S2)) && (digitalRead(S4))) // turn left by rotationg left motors in forward and right ones in backward direction
{
analogWrite(LM1, 55);
digitalWrite(LM2, LOW);
digitalWrite(RM1, LOW);
digitalWrite(RM2, LOW);
}
if((digitalRead(S2))  && !(digitalRead(S4))) // Turn right by rotating right motors in forward and left ones in backward direction
{
digitalWrite(LM1, LOW);
digitalWrite(LM2, LOW);
digitalWrite(RM1, LOW);
analogWrite(RM2, 55);
}

}
}

void passCrossSection()
{
  while( !(digitalRead(S1)) || !(digitalRead(S5)) )
  goForword();
}

void turnLeft()
{
  while((!(digitalRead(S1))) || (!(digitalRead(S2))))
  {
    analogWrite(LM1, 65);
    digitalWrite(LM2, LOW);
    analogWrite(RM1, 0);
    digitalWrite(RM2, LOW);
  } 
  wait();
  if(!((digitalRead(S1)) && (!(digitalRead(S2)) || !(digitalRead(S3)) || !(digitalRead(S4))) && (digitalRead(S5))))
  {
    Stop();
    return;
  }
  carDirection+=3;
  carDirection%=4;  
}

void turnRight()
{ 
  while((!(digitalRead(S4))) ||(!(digitalRead(S5))))
  {
    digitalWrite(LM1, LOW);
    analogWrite(LM2, 0);
    digitalWrite(RM1, LOW);
    analogWrite(RM2, 65);
  }
  wait();
  if(!((digitalRead(S1)) && (!(digitalRead(S2)) || !(digitalRead(S3)) || !(digitalRead(S4))) && (digitalRead(S5))))
  {
    Stop();
    return;
  }
  carDirection+=1;
  carDirection%=4;
}

void goBack()
{
  while(digitalRead(S2))
  {
    digitalWrite(LM1, LOW);
    analogWrite(LM2, 0);
    digitalWrite(RM1, LOW);
    analogWrite(RM2, 65);
  }
  while(!(digitalRead(S2)))
  {
    analogWrite(LM1, 65);
    digitalWrite(LM2, LOW);
    analogWrite(RM1, 0);
    digitalWrite(RM2, LOW);
  }
 carDirection+=2;
 carDirection%=4;
}

void Stop()
{
digitalWrite(LM1, LOW);
digitalWrite(LM2, LOW);
digitalWrite(RM1, LOW);
digitalWrite(RM2, LOW);
metion1Ended=true;
if(metion2Started)
metion2Ended=true;
carDirection=North;
mode = going;
counter=0;
}

void wait()
{
digitalWrite(LM1, LOW);
digitalWrite(LM2, LOW);
digitalWrite(RM1, LOW);
digitalWrite(RM2, LOW);
delay(150);
}

void goNorth()
{
  switch(carDirection)
  {
    case North:
    passCrossSection();
    break;
    case West:
    turnLeft();
    break;
    case East:
    turnRight();
    break;
    default:
    break;
  }
}

void goSouth()
{
  switch(carDirection)
  {
    case South:
    passCrossSection();
    break;
    case East:
    turnLeft();
    break;
    case West:
    turnRight();
    break;
    default:
    break;
  }
}

void goWest()
{
  switch(carDirection)
  {
    case West:
    passCrossSection();
    break;
    case South:
    turnLeft();
    break;
    case North:
    turnRight();
    break;
    default:
    break;
  }
}

void goEast()
{
  switch(carDirection)
  {
    case East:
    passCrossSection();
    break;
    case North:
    turnLeft();
    break;
    case South:
    turnRight();
    break;
    default:
    break;
  }
}

void goIndirectionOf(byte direct)
{
  switch(direct)
  {
    case North:
    goNorth();
    break;
    case West:
    goWest();
    break;
    case South:
    goSouth();
    break;
    default:
    goEast();
    break;
  }
}


void crossRoadFunction(byte road1,byte road2,byte road3)
{
 if(metion2Started)
  {
    goIndirectionOf(crossRoads[counter].roads[crossRoads[counter].rightRoad]);
    counter++;  
  }
  else
  {
     if(mode==going)
     {
      addCrossRoad(road1,road2,road3);
      goIndirectionOf(road1);
      mode = going;
      }
       else
      {
        crossRoads[crossRoadsSize].rightRoad++;
        if(digitalRead(S1))
        {
          if(carDirection==North && crossRoads[crossRoadsSize].roads[crossRoads[crossRoadsSize].rightRoad]==East)
           crossRoads[crossRoadsSize].roads[crossRoads[crossRoadsSize].rightRoad]=4;
          if(carDirection==West && crossRoads[crossRoadsSize].roads[crossRoads[crossRoadsSize].rightRoad]==North)
           crossRoads[crossRoadsSize].roads[crossRoads[crossRoadsSize].rightRoad]=4;
          if(carDirection==South && crossRoads[crossRoadsSize].roads[crossRoads[crossRoadsSize].rightRoad]==West)
           crossRoads[crossRoadsSize].roads[crossRoads[crossRoadsSize].rightRoad]=4;
          if(carDirection==East && crossRoads[crossRoadsSize].roads[crossRoads[crossRoadsSize].rightRoad]==South)
           crossRoads[crossRoadsSize].roads[crossRoads[crossRoadsSize].rightRoad]=4;
        }
        if(digitalRead(S5))
        {
          if(carDirection==North && crossRoads[crossRoadsSize].roads[crossRoads[crossRoadsSize].rightRoad]==West)
           crossRoads[crossRoadsSize].roads[crossRoads[crossRoadsSize].rightRoad]=4;
          if(carDirection==West && crossRoads[crossRoadsSize].roads[crossRoads[crossRoadsSize].rightRoad]==South)
           crossRoads[crossRoadsSize].roads[crossRoads[crossRoadsSize].rightRoad]=4;
          if(carDirection==South && crossRoads[crossRoadsSize].roads[crossRoads[crossRoadsSize].rightRoad]==East)
           crossRoads[crossRoadsSize].roads[crossRoads[crossRoadsSize].rightRoad]=4;
          if(carDirection==East && crossRoads[crossRoadsSize].roads[crossRoads[crossRoadsSize].rightRoad]==North)
           crossRoads[crossRoadsSize].roads[crossRoads[crossRoadsSize].rightRoad]=4;
        }
        if(crossRoads[crossRoadsSize].roads[crossRoads[crossRoadsSize].rightRoad]==4)
        {
         goIndirectionOf(crossRoads[crossRoadsSize].Entry);
         removeCrossRoad();
         mode = coming;
        }
        else
        {
        goIndirectionOf(crossRoads[crossRoadsSize].roads[crossRoads[crossRoadsSize].rightRoad]);
         mode = going;
        }
       }
    }   
}











void loop()
{
if(digitalRead(btn1))metion1Started=true;
if(metion1Ended)if(digitalRead(btn2))metion2Started=true;

  if((metion1Started && !metion1Ended) || (metion2Started && !metion2Ended))
 {

if((digitalRead(S1)) && (!(digitalRead(S2)) || !(digitalRead(S3)) || !(digitalRead(S4))) && (digitalRead(S5)))
  {
    goForword();
  }
  else
  {
        wait();
    if (!(digitalRead(S1)) && !(digitalRead(S2))&& !(digitalRead(S4)) && !(digitalRead(S5)))
       {
         crossRoadFunction((carDirection+3)%4,(carDirection+1)%4,carDirection);
       }
    else if (!(digitalRead(S1))  && (digitalRead(S5)))
       {
         crossRoadFunction((carDirection+3)%4,carDirection,4);
       }
    else if ((digitalRead(S1)) && !(digitalRead(S5)))
      {
        crossRoadFunction((carDirection+1)%4,carDirection,4);
      }
    else if((digitalRead(S1)) && (digitalRead(S2)) && (digitalRead(S3)) && (digitalRead(S4))  && (digitalRead(S5))) // Finish line, stop both the motors
      {
        mode=coming;
        goBack();
      }  
  }
 }
}
