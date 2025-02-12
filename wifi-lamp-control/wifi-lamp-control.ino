#include <ESP8266WiFi.h>      // Library to handle WiFi functionality for ESP8266
#include <ESP8266WebServer.h> // Library to host an HTTP server

/*-------------CONFIG--------------------*/
// Access Point (AP) configuration
const char *AP_SSID = "ESP01_AccessPoint"; // SSID for the ESP-01 Access Point
const char *AP_Password = "12345678";      // Password for the Access Point (min. 8 characters)
const int Relay_PIN = 0;                   // GPIO0 on ESP-01 used to control the relay

// Static IP configuration for the Access Point
IPAddress AP_IP(192, 168, 4, 1);       // Static IP address for the Access Point
IPAddress AP_Subnet(255, 255, 255, 0); // Subnet mask for the Access Point
/*-------------CONFIG--------------------*/

ESP8266WebServer server(80); // Create an HTTP server instance listening on port 80
String State = "OFF";        // Stores the current state of the relay ("ON" or "OFF")

void setup()
{
  // Configure the relay pin as an output
  pinMode(Relay_PIN, OUTPUT);
  digitalWrite(Relay_PIN, HIGH); // Start with the relay off (LOW)

  // Initialize serial communication for debugging
  Serial.begin(115200);
  Serial.println();

  // Configure the ESP-01 as an Access Point with a static IP
  WiFi.softAPConfig(AP_IP, AP_IP, AP_Subnet); // Set static IP and subnet mask for the AP
  WiFi.softAP(AP_SSID, AP_Password);          // Start the Access Point with SSID and password

  // Print the AP's static IP address to the serial monitor
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  // Configure the web server's root route ("/")
  server.on("/", handleRoot);

  // Start the HTTP server
  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  // Handle incoming HTTP client requests
  server.handleClient();
}

void handleRoot()
{
  // Check if the HTTP request method is POST
  if (server.method() != HTTP_POST)
  {
    // If the method is not POST, respond with a 405 Method Not Allowed error
    server.send(405, "text/plain", "Method Not Allowed");
  }
  else
  {
    // Read the "command" parameter sent in the POST request
    String command = server.arg("command");

    // If the command is "ON", turn the relay on
    if (command == "ON")
    {
      State = "ON";                         // Update the state
      digitalWrite(Relay_PIN, LOW);         // Turn the LED on (Note that LOW is the voltage level. This is because it is active low on the ESP-01
      server.send(200, "text/plain", "ON"); // Respond with a success message
    }
    // If the command is "OFF", turn the relay off
    else if (command == "OFF")
    {
      State = "OFF";                         // Update the state
      digitalWrite(Relay_PIN, HIGH);         // Turn the LED off by making the voltage HIGH
      server.send(200, "text/plain", "OFF"); // Respond with a success message
    }
    // If the command is "STATE", return the current relay state
    else if (command == "STATE")
    {
      server.send(200, "text/plain", State); // Respond with the current state
    }
    // If the command is invalid, return an error message
    else
    {
      server.send(400, "text/plain", "Invalid Command");
    }
  }
}