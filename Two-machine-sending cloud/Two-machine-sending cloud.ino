// Libraries
#include <ESP8266WiFi.h>
// Wi-Fi network SSID and password
const char* ssid = "Shehab DSL";
const char* password = "@#$123SHvod";
// Host
const char* host = "dweet.io";
int moistureReading = 0; // holds value soil moisture sensor

void setup() {
 Serial.begin(115200); // initialize serial interface
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
 //get soil moisture reading
 moistureReading = analogRead(A0);

 // We now create a URI for the request
 String url = "/dweet/for/garden-monitor-11448?moisture=";
 url += String(moistureReading);

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