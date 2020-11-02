// Programa: Web Server com modulo ESP8266
// Alteracoes e adaptacoes: FILIPEFLOP
 
#include <SoftwareSerial.h>
 
//RX pino 2, TX pino 3
SoftwareSerial monitor(2, 3);
 
#define DEBUG true
 
void setup()
{
  monitor.begin(9600);
  Serial.begin(115200);
  pinMode(11,OUTPUT);
  digitalWrite(11,LOW);
  
  pinMode(12,OUTPUT);
  digitalWrite(12,LOW);
  
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  
  pinMode(10,OUTPUT);
  digitalWrite(10,HIGH);
 
  sendData("AT+RST\r\n", 5000, DEBUG); // rst
  // Conecta a rede wireless
  sendData("AT+CWMODE=1\r\n", 1000, DEBUG);
  
  sendData("AT+CWJAP=\"MADHU P\",\"madhuorl\"\r\n", 2000, DEBUG);
  delay(3000);
  
  // Mostra o endereco IP
  sendData("AT+CIFSR\r\n", 5000, DEBUG);
  // Configura para multiplas conexoes
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG);
  // Inicia o web server na porta 80
  sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG);
}
 
void loop()
{
  if(Serial.available()) // check if the esp is sending a message 
  {
 
    
    if(Serial.find("+IPD,"))
    {
     delay(300); // wait for the serial buffer to fill up (read all the serial data)
     // get the connection id so that we can then disconnect
     int connectionId = Serial.read()-48; // subtract 48 because the read() function returns 
                                           // the ASCII decimal value and 0 (the first decimal number) starts at 48
          
     Serial.find("pin="); // advance cursor to "pin="
     
     int pinNumber = (Serial.read()-48)*10; // get first number i.e. if the pin 13 then the 1st number is 1, then multiply to get 10
     pinNumber += (Serial.read()-48); // get second number, i.e. if the pin number is 13 then the 2nd number is 3, then add to the first number
     
     digitalWrite(pinNumber, !digitalRead(pinNumber)); // toggle pin    
     
     // make close command
     String closeCommand = "AT+CIPCLOSE="; 
     closeCommand+=connectionId; // append connection id
     closeCommand+="\r\n";
     
     sendData(closeCommand,1000,DEBUG); // close connection
    }
  }

}
 
String sendData(String command, const int timeout, boolean debug)
{
  // Envio dos comandos AT para o modulo
  String response = "";
  Serial.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (Serial.available())
    {
      // The esp has data so display its output to the serial window
      char c = Serial.read(); // read the next character.
      response += c;
    }
  }
  if (debug)
  {
    monitor.print(response);
  }
  return response;
}
