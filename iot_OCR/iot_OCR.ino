#include "esp_camera.h"
#include <WiFi.h>
#include <Arduino.h>

// Gantilah dengan kredensial Wi-Fi Anda
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Konfigurasi pin kamera untuk ESP32-CAM
#define CAMERA_MODEL_AI_THINKER // Untuk model AI Thinker ESP32-CAM
#include "camera_pins.h"

// Fungsi untuk memulai server kamera (opsional)
void startCameraServer() {
  // Misalnya untuk streaming gambar via antarmuka web
}

void setup() {
  Serial.begin(115200);

  // Menghubungkan ke Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Menghubungkan ke WiFi...");
  }
  Serial.println("Terhubung ke WiFi!");

  // Menginisialisasi kamera
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
  config.pixel_format = PIXFORMAT_GRAYSCALE;  // Menggunakan format grayscale untuk deteksi angka
  config.frame_size = FRAMESIZE_QVGA;  // Resolusi rendah untuk efisiensi
  config.fb_count = 2;

  // Menginisialisasi kamera
  esp_camera_init(&config);

  Serial.println("Kamera diinisialisasi!");
}

void loop() {
  // Menangkap gambar
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Gagal menangkap gambar");
    return;
  }

  // Proses gambar untuk deteksi angka
  processImage(fb->buf, fb->len);

  // Membebaskan buffer frame
  esp_camera_fb_return(fb);

  delay(5000); // Menangkap gambar setiap 5 detik (sesuaikan sesuai kebutuhan)
}

void processImage(uint8_t* imageBuffer, size_t imageLength) {
  Serial.println("Memproses gambar...");

  // Langkah 1: Mengubah gambar menjadi grayscale sudah dilakukan karena kamera diatur untuk grayscale
  // Langkah 2: Terapkan thresholding sederhana (untuk membedakan angka dari latar belakang)
  uint8_t threshold = 128;  // Nilai threshold untuk konversi
  for (size_t i = 0; i < imageLength; i++) {
    if (imageBuffer[i] < threshold) {
      imageBuffer[i] = 0;  // Pixel gelap menjadi hitam
    } else {
      imageBuffer[i] = 255;  // Pixel terang menjadi putih
    }
  }

  // Langkah 3: Deteksi kontur untuk menemukan angka
  // (Pendekatan ini hanya memberikan gambaran sederhana karena ESP32 terbatas dalam pengolahan citra yang kompleks)
  int detectedNumbers = detectNumbers(imageBuffer, imageLength);

  // Tampilkan angka yang terdeteksi
  if (detectedNumbers > 0) {
    Serial.print("Angka yang terdeteksi: ");
    Serial.println(detectedNumbers);
  } else {
    Serial.println("Tidak ada angka yang terdeteksi.");
  }
}

int detectNumbers(uint8_t* imageBuffer, size_t imageLength) {
  // Fungsi deteksi angka sederhana (contoh: pencocokan pola atau analisis kontur)
  // Di sini kita hanya akan menghitung jumlah pixel putih sebagai deteksi kasar angka
  int count = 0;
  for (size_t i = 0; i < imageLength; i++) {
    if (imageBuffer[i] == 255) {  // Pixel putih
      count++;
    }
  }
  
  // Berdasarkan jumlah pixel putih, kita bisa mencoba mendeteksi angka
  // Misalnya, kita bisa menyesuaikan nilai deteksi angka berdasarkan ukuran atau pola tertentu
  // Ini adalah cara yang sangat sederhana dan bukan OCR yang sesungguhnya.
  
  return (count > 500) ? 1 : 0;  // Deteksi angka jika lebih dari 500 pixel putih
}
