/*

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 
*/

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

byte mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
EthernetClient ethClient;
PubSubClient client(ethClient);

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  } Serial.println();
  if (!strncmp((char *)payload, "on", length)) {
      digitalWrite(7, 1);
      client.publish("nodejs", "led on");
  } else if (!strncmp((char *)payload, "off", length)) {
      digitalWrite(7, 0);
      client.publish("nodejs", "led off");
  }
}

long lastReconnectAttempt = 0;
boolean reconnect() {
  if (client.connect("arduinoClient")) {
    client.publish("outTopic","hello world");
    client.subscribe("inTopic");
  } return client.connected();
}

void setup(){

  //====Serial/Pins Setup====
  Serial.begin(9600);
  pinMode(7, OUTPUT);
  
  //====Display Setup====
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  } Serial.println(F("SSD1306 allocation success"));
  display.clearDisplay(); delay(1500);
  display.setTextSize(1);            
  display.setTextColor(SSD1306_WHITE); 
  display.setCursor(0,0);
  
  //====Ethernet Connection====
  display.println(F("Obtaining an IP address using DHCP"));
  if (Ethernet.begin(mac) == 0) {
    display.println(F("Failed to obtaining an IP address"));
    if (Ethernet.hardwareStatus() == EthernetNoHardware)
      display.println(F("Ethernet shield was not found"));
    if (Ethernet.linkStatus() == LinkOFF)
      display.println(F("Ethernet cable is not connected."));
    display.display();
    while (true);
  } 
  display.print("- Arduino's IP address   : ");
  display.println(Ethernet.localIP());
  display.print("- Gateway's IP address   : ");
  display.println(Ethernet.gatewayIP());
  display.print("- Network's subnet mask  : ");
  display.println(Ethernet.subnetMask());
  display.print("- DNS server's IP address: ");
  display.println(Ethernet.dnsServerIP());
  
  //====MQTT Connection====
  client.setServer("broker.emqx.io", 1883);
  client.setCallback(callback);
  lastReconnectAttempt = 0;
  
}

void loop(){

  //====MQTT Reconnection====
  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      if (reconnect()) lastReconnectAttempt = 0;
    }
  } else { client.loop();}

  
}
