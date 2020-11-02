#include <SoftwareSerial.h>
 
//RX pino 2, TX pino 3
SoftwareSerial monitor(2, 3);

#define DEBUG true

void setup()
{
  monitor.begin(9600);
  Serial.begin(115200);
  pinMode(10,OUTPUT);
  digitalWrite(10,HIGH);
  
  pinMode(11,OUTPUT);
  digitalWrite(11,LOW);
  
  
  pinMode(12,OUTPUT);
  digitalWrite(12,LOW);
  
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  wificonnect(); 
}
 
void loop()
{ 
  if(Serial.available()) 
  {
    if(Serial.find("+IPD,"))
    {
     delay(300);
     int connectionId = Serial.read()-48;
     Serial.find("pin=");
     
     int pinNumber = (Serial.read()-48)*10;
     pinNumber += (Serial.read()-48);
     
     digitalWrite(pinNumber, !digitalRead(pinNumber)); 
     
     // make close command
     String closeCommand = "AT+CIPCLOSE="; 
     closeCommand+=connectionId; // append connection id
     closeCommand+="\r\n";
 
     sendData(closeCommand,1000,DEBUG); // close connection
     delay(500);
     digitalWrite(10,HIGH);
     delay(500);
     wificonnect();
     delay(500);  
  }
  }
}
 
String sendData(String command, const int timeout, boolean debug)
{

  String response = "";
  Serial.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (Serial.available())
    {
      char c = Serial.read();
      response += c;
    }
  }
  if (debug)
  {
    monitor.print(response);
  }
  return response;
}

void wificonnect(void)
{
  sendData("AT+RST\r\n", 1000, DEBUG); // rst
  // Conecta a rede wireless
  sendData("AT+CWMODE=1\r\n", 500, DEBUG);
  
  sendData("AT+CWJAP=\"MADHU P\",\"madhuorl\"\r\n", 2000, DEBUG);
  delay(2000);
  
  // Mostra o endereco IP
  sendData("AT+CIFSR\r\n", 1000, DEBUG);
  // Configura para multiplas conexoes
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG);
  // Inicia o web server na porta 80
  sendData("AT+CIPSERVER=1,80\r\n",1000, DEBUG);
  delay(10);
  digitalWrite(10,LOW);
}
