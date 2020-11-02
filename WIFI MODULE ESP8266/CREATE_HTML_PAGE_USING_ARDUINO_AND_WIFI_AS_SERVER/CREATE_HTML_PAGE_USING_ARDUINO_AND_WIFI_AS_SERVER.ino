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
  // Verifica se o ESP8266 esta enviando dados
  if (Serial.available())
  {
    if (Serial.find("+IPD,"))
    {
      delay(300);
      int connectionId = Serial.read() - 48;
 
      String webpage = "<head><meta http-equiv=""refresh"" content=""3"">";
      webpage += "</head><h1><u>ESP8266 - Web Server</u></h1><h2>Porta";
      webpage += "Digital 8: ";
      int a = digitalRead(8);
      webpage += a;
      webpage += "<h2>Porta Digital 9: ";
      int b = digitalRead(9);
      webpage += b;
      webpage += "</h2>";
 
      String cipSend = "AT+CIPSEND=";
      cipSend += connectionId;
      cipSend += ",";
      cipSend += webpage.length();
      cipSend += "\r\n";
 
      sendData(cipSend, 1000, DEBUG);
      sendData(webpage, 1000, DEBUG);
 
      String closeCommand = "AT+CIPCLOSE=";
      closeCommand += connectionId; // append connection id
      closeCommand += "\r\n";
 
      sendData(closeCommand, 3000, DEBUG);
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
