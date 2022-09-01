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

void liga(){
  digitalWrite(7, 1);
} void desliga(){
  digitalWrite(7, 0);
}

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

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("arduinoClient","emqx","public")) {
      Serial.println("connected");
      client.publish("nodejs","Arduino Connected");
      client.subscribe("nodejs");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup(){
  
  Serial.begin(57600);
  client.setServer("broker.emqx.io", 1883);
  client.setCallback(callback);

  Serial.println("Obtaining an IP address using DHCP");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to obtaining an IP address");
    if (Ethernet.hardwareStatus() == EthernetNoHardware)
      Serial.println("Ethernet shield was not found");
    if (Ethernet.linkStatus() == LinkOFF)
      Serial.println("Ethernet cable is not connected.");
    while (true);
  }

  Serial.print("- Arduino's IP address   : ");
  Serial.println(Ethernet.localIP());
  Serial.print("- Gateway's IP address   : ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("- Network's subnet mask  : ");
  Serial.println(Ethernet.subnetMask());
  Serial.print("- DNS server's IP address: ");
  Serial.println(Ethernet.dnsServerIP());

  pinMode(7, OUTPUT);
  
  delay(1500);
  
}

void loop(){
  if (!client.connected()) {
    reconnect();
  } client.loop();
}
