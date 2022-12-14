#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

byte mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
EthernetClient ethClient;
PubSubClient client(ethClient);

#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
#define I2C_ADDRESS 0x3C

SSD1306AsciiAvrI2c oled;

#include <Keypad.h>

const byte ROWS = 4; 
const byte COLS = 3;
char hexaKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
}; byte rowPins[ROWS] = {8,7,6,5};
byte colPins[COLS] = {4,3,2};
String keyp; 
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void callback(char* topic, byte* payload, unsigned int length) {
  if (!strncmp((char *)payload, "on", length)) {
      digitalWrite(7, 1);
      client.publish("Arduino", "Led On");
      oled.setRow(7);
      oled.println("> Led On          ");
  } else if (!strncmp((char *)payload, "off", length)) {
      digitalWrite(7, 0);
      client.publish("Arduino", "Led Off");
      oled.setRow(7);
      oled.println("> Led Off         ");
  }
}

void reconnect() {
  while (!client.connected()) {
    oled.clear();
    oled.setRow(0);
    oled.println(Ethernet.localIP());
    oled.println("> Connecting...              ");
    if (client.connect("arduinoClient00001","emqx","public")) {
      client.publish("arduino","Arduino Connected");
      client.subscribe("nodejs");
      oled.println(F("> Mqtt Connected              "));
    } else {
      oled.print("Failed, rc=");
      oled.println(client.state());
      oled.println("Trying again in 5 seconds");
      delay(5000);
    }
  }
}

void setup(){

  //====Serial/Pins Setup====
  Serial.begin(9600);
  pinMode(7, OUTPUT);
  
  //====oled Setup====
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(Adafruit5x7);
  oled.clear();

  //====Ethernet Connection====
  oled.println(F("Obtaining IP address..."));
  if (Ethernet.begin(mac) == 0) {
    oled.println(F("Failed to obtain IP address"));
    if (Ethernet.hardwareStatus() == EthernetNoHardware)
      oled.println(F("Ethernet shield was not found"));
    if (Ethernet.linkStatus() == LinkOFF)
      oled.println(F("Ethernet cable is not connected."));
    while (true);
  } oled.clear();
  oled.println("Arduino's IP address:");
  oled.println(Ethernet.localIP());
  oled.println("Gateway's IP address:");
  oled.println(Ethernet.gatewayIP());
  oled.println("Network's subnet mask:");
  oled.println(Ethernet.subnetMask());
  oled.println("DNS server's IP address:");
  oled.println(Ethernet.dnsServerIP());
  delay(5000); oled.clear();
  oled.println(Ethernet.localIP());
  
  //====MQTT Connection====
  client.setServer("broker.emqx.io", 1883);
  client.setCallback(callback);
  
}

void loop(){

  //====MQTT Reconnection====
  if (!client.connected()) {
    reconnect();
  } client.loop();

  //====Keypad Message====
  char customKey = customKeypad.getKey();
  if (customKey){
    oled.set2X();
    if (customKey == '*'){
      if (keyp.length() > 0){ 
        keyp.remove(keyp.length()-1); 
      }
    } else if (customKey == '#') {
      if (keyp.length() > 0){
        char charBuf[50]; keyp.toCharArray(charBuf, 50);
        client.publish("arduino", charBuf);
        oled.setCursor(0,0); oled.setRow(5);
        oled.print("> SENT        ");
        while (keyp.length() > 0) {
          keyp.remove(keyp.length()-1);
        } delay(500);
      }
    } else if (keyp.length() < 5){
      keyp.concat(customKey);
      Serial.println(keyp);
    } 
    oled.setCursor(0,0); oled.setRow(5);
    oled.print("                     ");
    oled.setCursor(0,0); oled.setRow(5);
    oled.print("> "); oled.print(keyp);
    oled.set1X();
  }
  
}