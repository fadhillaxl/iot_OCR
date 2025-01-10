#include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <Base64.h>

// Replace with your Wi-Fi credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Camera pin configuration for ESP32-CAM
#define CAMERA_MODEL_AI_THINKER // For AI Thinker ESP32-CAM
#include "camera_pins.h"

// Replace with your server URL (Python Flask server or other OCR API)
const char* serverUrl = "http://yourserver.com/ocr";

WiFiClient client;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");

  // Initialize the camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL;
  config.ledc_timer = LEDC_TIMER;
  config.pin_d0 = 0;
  config.pin_d1 = 1;
  config.pin_d2 = 2;
  config.pin_d3 = 3;
  config.pin_d4 = 4;
  config.pin_d5 = 5;
  config.pin_d6 = 6;
  config.pin_d7 = 7;
  config.pin_xclk = 8;
  config.pin_pclk = 9;
  config.pin_vsync = 10;
  config.pin_href = 11;
  config.pin_sscb_sda = 12;
  config.pin_sscb_scl = 13;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_VGA;
  config.jpeg_quality = 12;
  config.fb_count = 2;

  // Initialize camera
  esp_camera_init(&config);

  Serial.println("Camera initialized!");
}

void loop() {
  // Capture image
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  // Send the image for OCR processing
  sendImageForOCR(fb->buf, fb->len);

  // Free the frame buffer
  esp_camera_fb_return(fb);

  delay(5000); // Capture every 5 seconds (adjust as needed)
}

void sendImageForOCR(uint8_t* imageBuffer, size_t imageLength) {
  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");

  // Convert image to Base64
  String imageBase64 = base64::encode(imageBuffer, imageLength);

  // Prepare JSON body with the image data
  String body = "{\"image\": \"" + imageBase64 + "\"}";

  // Send POST request to the server
  int httpResponseCode = http.POST(body);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("OCR Response: " + response);
  } else {
    Serial.println("Error on HTTP request");
  }

  http.end();
}
