#include "MQ135.h"
#include<SoftwareSerial.h>
#include<stdlib.h>
#define DEBUG true
//int ledPin=13;
String apiKey="TX3YZ37PSC8O1IZL";
SoftwareSerial ser(2,3);
int air_quality;


void setup() 
{
  // put your setup code here, to run once:
//pinMode(ledPin,OUTPUT);
Serial.begin(9600);
ser.begin(9600);
ser.println("AT+RST");
sendData("AT+RST\r\n",2000,DEBUG);
 sendData("AT+CWMODE_CUR=3\r\n",1000,DEBUG);
sendData("AT+CWJAP=\"jio\",\"sujaygijre123\"\r\n",5000,DEBUG);
delay(3000);
sendData("AT+CIFSR\r\n",3000,DEBUG);
delay(1000);
sendData("AT+CIPMUX=0\r\n",2000,DEBUG);
delay(1000);
}

void loop() 
{
MQ135 gasSensor = MQ135(A0);
float air_quality = gasSensor.getPPM();
int ldr=analogRead(A2);
int temp=analogRead(A1);
if(air_quality>=1000)
{
  air_quality=air_quality/10;
}
delay(200);
int val=0;
for(int i=0;i<10;i++)  
{
  val+=analogRead(temp);
  delay(500);
}
float temp1=(temp * 1.8) + 32;
char buf[26];
String strTemp=String(temp1,1);
Serial.print("the temperature is :");
Serial.println(strTemp);

char buf2[26];
String strldr=String(ldr,1);
Serial.print("the luminosity is :");
Serial.println(strldr);
//Serial.println(strldr);

char buf3[26];
String strair_quality=String(air_quality,1);
Serial.print("the air_quality is :");
Serial.println(strair_quality);
//Serial.println(strair_quality);

String cmd= "AT+CIPSTART=\"TCP\",\"";
cmd+= "184.106.153.149";
cmd+="\",80";
ser.println(cmd);
if(ser.find("ERROR"))
{
  Serial.println("AT+CIPSTART error");
  return;
  }
  String getStr= "GET /update?api_key=";
  getStr+= apiKey;
  getStr+="&field1=";
  getStr+= String(strTemp);
  
  getStr+="&field2=";
  getStr+= String(strldr);
  
  getStr+="&field3=";
  getStr+= String(strair_quality);
  getStr+= "\r\n\r\n";

  cmd="AT+CIPSEND=";
  cmd+=String(getStr.length());
  ser.println(cmd);
  if(ser.find(">"))
  {
    ser.println(getStr);
    }
    else
    {
      ser.print("AT+CIPCLOSE");
      Serial.print("AT+CIPCLOSE");
      }
  delay(16000);
  }
 
  
  
  String sendData(String command,const int timeout,boolean debug)
{
  String response="";
  ser.print(command);
  long int time=millis();
  while((time+timeout)>millis())
  {
    while(ser.available())
    {
      char c = ser.read();
      response+=c;
      }
    }
    if(debug)
    {
      Serial.print(response);
      }
      return response;
  }

