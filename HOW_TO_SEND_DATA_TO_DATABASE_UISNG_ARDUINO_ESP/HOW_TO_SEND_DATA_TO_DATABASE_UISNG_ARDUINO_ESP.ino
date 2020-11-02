#include "SoftwareSerial.h"
String ssid ="ORL";
String password="orl_india";
SoftwareSerial esp(10, 11);// RX, TX
String data;
String server = "192.168.1.8"; // www.example.com
String uri = "/d1mini.php";// our example is /esppost.php
byte dat [5];
String temp ,hum;
void setup() 
{
  esp.begin(9600);
  Serial.begin(115200);
  esp.println("DATA BASE TEST");
  delay(500);
  reset();
  delay(500);
  connectWifi();
  delay(500);
}
//reset the esp8266 module
void reset() 
{
  Serial.println("AT+RST");
  delay(1000);
  if(Serial.find("OK") ) esp.println("Module Reset");
}
//connect to your wifi network
void connectWifi() 
{
  Serial.println("AT+CWMODE=3");
  delay(2000);
  String cmd = "AT+CWJAP=\"" +ssid+"\",\"" + password + "\"";
  Serial.println(cmd);
  delay(4000);
  if(Serial.find("OK"))
  {
    esp.println("Connected!");
  }
  else 
  {
      connectWifi();
      esp.println("Cannot connect to wifi"); 
  }
}

void loop () 
{
  //reset();
  //delay(100);
  //connectwifi();
  //delay(100);
  int n=analogRead(A1);
  int m=analogRead(A2);
  
  hum =String(n);
  temp= String(m);
  data = "boot_value=" + temp + "&boot_value1=" + hum;// data sent must be under this form //name1=value1&name2=value2.
  httppost();
  delay(1000);
}
void httppost ()
{
  Serial.println("AT+CIPSTART=\"TCP\",\"" + server + "\",80");//start a TCP connection.
  if( Serial.find("OK")) 
  {
    esp.println("TCP connection ready");
  }
  delay(1000);
  String postRequest ="POST " + uri + " HTTP/1.0\r\n" +"Host: " + server + "\r\n" +"Accept: *" + "/" + "*\r\n" +
  "Content-Length: " + data.length() + "\r\n" +
  "Content-Type: application/x-www-form-urlencoded\r\n" +
  "\r\n" + data;

  String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.

  Serial.print(sendCmd);

  Serial.println(postRequest.length() );

  delay(500);

  if(Serial.find(">"))
  {
    esp.print(">");
    esp.print(postRequest);
    Serial.print(postRequest);
  }
  else
  {
    Serial.println("AT+CIPCLOSE");
  }
  if(Serial.find("OK"))
  {
    esp.println("RECEIVED: OK");
  }
  else
  {
    esp.println("RECEIVED: Error");
  }
}
