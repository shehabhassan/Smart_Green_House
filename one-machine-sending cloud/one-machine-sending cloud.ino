// Libraries
#include <ESP8266WiFi.h>
#include "DHT.h"
// Wi-Fi network SSID and password
const char* ssid = "Shehab DSL";
const char* password = "@#$123SHvod";
// Host
const char* host = "dweet.io";
#define DHTPIN 2 // what digital pin DHT11 is connected to
#define DHTTYPE DHT11 // DHT 11 sensor
DHT dht(DHTPIN, DHTTYPE);
void setup() {
 Serial.begin(115200); // initialize serial interface
 dht.begin(); // initialize DHT11 sensor
 delay(10);
 // We start by connecting to a WiFi network
 Serial.println();
 Serial.println();
 Serial.print("Connecting to ");
 Serial.println(ssid);

 WiFi.begin(ssid, password);

 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }
 Serial.println("");
 Serial.println("WiFi connected");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
}
void loop() {
 delay(5000);
 Serial.print("connecting to ");
 Serial.println(host);
 WiFiClient client;
 const int httpPort = 80;
 // Use WiFiClient class to create TCP connections
 if (!client.connect(host, httpPort)) {
 Serial.println("connection failed");
 return;
 }
 // Read sensor inputs
 // get humidity reading
 float h = dht.readHumidity();
 // get temperature reading in Celsius
 float t = dht.readTemperature();
 // Check if any reads failed and exit early (to try again).
 while (isnan(h) || isnan(t)) {
 Serial.println("Failed to read from DHT sensor!");
 delay(2000); // delay before next measurements
 //get the measurements once more
 h = dht.readHumidity();
 t = dht.readTemperature();
 }

 // We now create a URI for the request
 String url = "/dweet/for/garden-monitor-11447?humidity=";
 url += String(h);
 url += "&temperature=";
 url += String(t);

 // Send request
 Serial.print("Requesting URL: ");
 Serial.println(url);

 client.print(String("GET ") + url + " HTTP/1.1\r\n" +
 "Host: " + host + "\r\n" +
 "Connection: close\r\n\r\n");
 unsigned long timeout = millis();
 while (client.available() == 0) {
 if (millis() - timeout > 5000) {
 Serial.println(">>> Client Timeout !");
 client.stop();
 return;
 }
 }
 // Read all the lines from the answer
 while(client.available()){
   String line = client.readStringUntil('\r');
 Serial.print(line);
 }
 // Close connecting
 Serial.println();
 Serial.println("closing connection");
}