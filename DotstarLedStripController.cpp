/*
 MIT License

 Copyright (c) 2018 Ove Nystås

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

// --- Includes ---
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>

#include <Adafruit_DotStar.h>
#include <SPI.h>

#include <DHTesp.h>

#include "Config.h"


// --- Variables ---
ESP8266WebServer server(80);

String output0State = "off";

Adafruit_DotStar strip =
    Adafruit_DotStar(NUMPIXELS, DOTSTAR_DATA_PIN, DOTSTAR_CLOCK_PIN, DOTSTAR_BRG);
int16_t head = 0;    // Index of first 'on' pixel
int16_t tail = -10;  // Index of first 'off' pixel
uint32_t color = 0xFF0000;  // 'On' color (starts red)

DHTesp dht;
float humidity = 0.0;
float temperature = 0.0;


// --- Local function declarations ---
static String getContentType(String filename);  // convert the file extension to the MIME type
static bool handleFileRead(String path);        // send the right file to the client (if it exists)
static void handleLedStrip();
static void handleDhtSensor();


// --- Public function definitions ---

// Initial setups
void setup()
{
  // Setup LED strip
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP

  // Initialize the output variables as outputs
  pinMode(LED_PIN, OUTPUT);
  // Set outputs to LOW
  digitalWrite(LED_PIN, LOW);

  // Setup DHT22 temperature and humidity sensor
  dht.setup(DHT_PIN, DHTesp::DHT22);

  // Connect serial port
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  // Print local IP address
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Start the SPI Flash Files System
  SPIFFS.begin();

  // If the client requests any URI, send it if it exists, otherwise, respond with a 404 (Not Found) error
  server.onNotFound([]()
  {
    if (!handleFileRead(server.uri()))
    server.send(404, "text/plain", "404: Not Found");
  });

  // Actually start the server
  server.begin();
  Serial.println("HTTP server started");
}


// Main loop
void loop()
{
  server.handleClient();
  handleLedStrip();
  handleDhtSensor();
}


// --- Local function definitions ---

static void handleLedStrip()
{
  strip.setPixelColor(head, color);
  strip.setPixelColor(tail, 0);
  strip.show();
  delay(20);

  if (++head >= NUMPIXELS)
  {
    head = 0;
    if ((color >>= 8) == 0)
      color = 0xFF0000;
  }
  if (++tail >= NUMPIXELS)
    tail = 0;
}


static void handleDhtSensor()
{
  humidity = dht.getHumidity();
  temperature = dht.getTemperature();
}


// convert the file extension to the MIME type
static String getContentType(String filename)
{
  if (filename.endsWith(".html"))
  {
    return "text/html";
  }
  else if (filename.endsWith(".css"))
  {
    return "text/css";
  }
  else if (filename.endsWith(".js"))
  {
    return "application/javascript";
  }
  else if (filename.endsWith(".ico"))
  {
    return "image/x-icon";
  }
  return "text/plain";
}


/* Send the right file to the client (if it exists)
 * Return true if file exists else false
 */
static bool handleFileRead(String path)
{
  bool ret;

  Serial.println("handleFileRead: " + path);

  // Turns the GPIOs on and off
  if (path.indexOf("GET /0/on") >= 0)
  {
    Serial.println("GPIO 0 on");
    output0State = "on";
    digitalWrite(LED_PIN, HIGH);
  }
  else if (path.indexOf("GET /0/off") >= 0)
  {
    Serial.println("GPIO 0 off");
    output0State = "off";
    digitalWrite(LED_PIN, LOW);
  }

  // If a folder is requested, send the index file
  if (path.endsWith("/"))
  {
    path += "index.html";
  }

  // Get the MIME type
  String contentType = getContentType(path);

  // If the file exists, open it, send it to the client, close the file
  if (SPIFFS.exists(path))
  {
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    ret = true;
  }
  else
  {
    // If the file doesn't exist, return false
    Serial.println("\tFile Not Found");
    ret = false;
  }

  return ret;
}
