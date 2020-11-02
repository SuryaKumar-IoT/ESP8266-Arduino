#include<stdlib.h>
#include <SoftwareSerial.h>

#define SSID "ORL"
#define PASS "orl_india"

#define IP "184.106.153.149" // thingspeak.com

String GET = "GET /update?key=ARYSD7H26G7TLXSN&field1=0";

SoftwareSerial monitor(10, 11); // RX, TX

void setup()
{
  pinMode(A1,INPUT);
  Serial.begin(115200);
  monitor.begin(9600);
  sendDebug("AT");
  delay(5000);
  if(Serial.find("OK"))
  {
    monitor.println("RECEIVED: OK");
    connectWiFi();
  }
}
void loop()
{
  
  int n=analogRead(A1);
  String tempF1 = String(n);
  int m=analogRead(A2);
  String tempF2 = String(m);
  int p=analogRead(A3);
  String tempF3 = String(p);
  int q=analogRead(A4);
  String tempF4 = String(q);
  updateTemp(tempF1,tempF2,tempF3,tempF4);

  
}
void updateTemp(String tenmpF1,String tenmpF2,String tenmpF3,String tenmpF4)
{
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "api.thingspeak.com";
  cmd += "\",80";
  sendDebug(cmd);
  delay(2000);
  if(Serial.find("Error"))
  {
    monitor.print("RECEIVED: Error");
    return;
  }
  cmd = GET;
  cmd += tenmpF1;
  cmd += "&field2=";
  cmd += tenmpF2;
  cmd += "&field3=";
  cmd += tenmpF3;
  cmd += "&field4=";
  cmd += tenmpF4;
  cmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  if(Serial.find(">"))
  {
    monitor.print(">");
    monitor.print(cmd);
    Serial.print(cmd);
  }
  else
  {
    sendDebug("AT+CIPCLOSE");
  }
  if(Serial.find("OK"))
  {
    monitor.println("RECEIVED: OK");
  }
  else
  {
    monitor.println("RECEIVED: Error");
  }
}

void sendDebug(String cmd)
{
  monitor.print("SEND: ");
  monitor.println(cmd);
  Serial.println(cmd);
}

boolean connectWiFi()
{
  Serial.println("AT+CWMODE=3");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  sendDebug(cmd);
  delay(5000);
  if(Serial.find("OK"))
  {
    monitor.println("RECEIVED: OK");
    return true;
  }
  else
  {
    monitor.println("RECEIVED: Error");
    return false;
  }
}


