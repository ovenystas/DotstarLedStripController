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

#ifndef CONFIG_H_
#define CONFIG_H_


// --- Includes ---
#include <Arduino.h>

// --- Constants ---
const uint16_t NUMPIXELS = 30;  // Number of LEDs in strip

const char* SSID = "<myssid>";            // WiFi SSID
const char* PASSWORD = "<mypassword>";  // WiFi password

const uint8_t LED_PIN = 0;

const uint8_t DOTSTAR_DATA_PIN = 4;
const uint8_t DOTSTAR_CLOCK_PIN = 5;

const uint8_t DHT_PIN = 6;

#endif /* CONFIG_H_ */
