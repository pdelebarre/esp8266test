#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "iMaison";
const char* password = "17nov2005";

String form = "<form action='led'><input type='radio' name='state' value='1' checked>On<input type='radio' name='state' value='0'>Off<input type='submit' value='Submit'></form>";

String imagepage = "<img src='/led.png'>";

// This is a png file (led.png)
const char image[] = {
  0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52,
  0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x08, 0x02, 0x00, 0x00, 0x00, 0x90, 0x91, 0x68,
  0x36, 0x00, 0x00, 0x00, 0x01, 0x73, 0x52, 0x47, 0x42, 0x00, 0xae, 0xce, 0x1c, 0xe9, 0x00, 0x00,
  0x00, 0x04, 0x67, 0x41, 0x4d, 0x41, 0x00, 0x00, 0xb1, 0x8f, 0x0b, 0xfc, 0x61, 0x05, 0x00, 0x00,
  0x00, 0x20, 0x63, 0x48, 0x52, 0x4d, 0x00, 0x00, 0x7a, 0x26, 0x00, 0x00, 0x80, 0x84, 0x00, 0x00,
  0xfa, 0x00, 0x00, 0x00, 0x80, 0xe8, 0x00, 0x00, 0x75, 0x30, 0x00, 0x00, 0xea, 0x60, 0x00, 0x00,
  0x3a, 0x98, 0x00, 0x00, 0x17, 0x70, 0x9c, 0xba, 0x51, 0x3c, 0x00, 0x00, 0x00, 0x18, 0x74, 0x45,
  0x58, 0x74, 0x53, 0x6f, 0x66, 0x74, 0x77, 0x61, 0x72, 0x65, 0x00, 0x50, 0x61, 0x69, 0x6e, 0x74,
  0x2e, 0x4e, 0x45, 0x54, 0x20, 0x76, 0x33, 0x2e, 0x33, 0x36, 0xa9, 0xe7, 0xe2, 0x25, 0x00, 0x00,
  0x00, 0x57, 0x49, 0x44, 0x41, 0x54, 0x38, 0x4f, 0x95, 0x52, 0x5b, 0x0a, 0x00, 0x30, 0x08, 0x6a,
  0xf7, 0x3f, 0xf4, 0x1e, 0x14, 0x4d, 0x6a, 0x30, 0x8d, 0x7d, 0x0d, 0x45, 0x2d, 0x87, 0xd9, 0x34,
  0x71, 0x36, 0x41, 0x7a, 0x81, 0x76, 0x95, 0xc2, 0xec, 0x3f, 0xc7, 0x8e, 0x83, 0x72, 0x90, 0x43,
  0x11, 0x10, 0xc4, 0x12, 0x50, 0xb6, 0xc7, 0xab, 0x96, 0xd0, 0xdb, 0x5b, 0x41, 0x5c, 0x6a, 0x0b,
  0xfd, 0x57, 0x28, 0x5b, 0xc2, 0xfd, 0xb2, 0xa1, 0x33, 0x28, 0x45, 0xd0, 0xee, 0x20, 0x5c, 0x9a,
  0xaf, 0x93, 0xd6, 0xbc, 0xdb, 0x25, 0x56, 0x61, 0x01, 0x17, 0x12, 0xae, 0x53, 0x3e, 0x66, 0x32,
  0xba, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};

// HTTP server will listen at port 80
ESP8266WebServer server(80);

const int led = 1;

void handle_adc() {
  float val = analogRead(0);
  server.send(200, "text/plain", String(val));
}

void handle_led() {
  // get the value of request argument "state" and convert it to an int
  int state = server.arg("state").toInt();

  digitalWrite(led, state);
  server.send(200, "text/plain", String("LED is now ") + ((state) ? "on" : "off"));
}

void handle_image() {
  server.send(200, "image/png", "");
  WiFiClient client = server.client();
  client.write(image, sizeof(image));
}

void handle_webpage_with_image() {
  server.send(200, "text/html", imagepage);
}

void setup(void) {
  Serial.begin(115200);
  Serial.println("");
  pinMode(led, OUTPUT);

  // Connect to WiFi network
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Set up the endpoints for HTTP server
  //
  // Endpoints can be written as inline functions:
  server.on("/", []() {
    server.send(200, "text/html", form);
  });

  // And as regular external functions:
  server.on("/adc", handle_adc);
  server.on("/led", handle_led);
  server.on("/led.png", handle_image);
  server.on("/showled", handle_webpage_with_image);

  // Start the server
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  // check for incomming client connections frequently in the main loop:
  server.handleClient();
}
