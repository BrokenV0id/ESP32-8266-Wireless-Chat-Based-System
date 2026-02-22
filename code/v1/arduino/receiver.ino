// ============ LIBRARIES ============
#include <esp_now.h>
#include <WiFi.h>

// ============ VARIABLES ============
#define CHANNEL 1

// ============ FUNCTIONS ============

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  esp_now_init();
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // put your main code here, to run repeatedly:
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  Serial.print("Recived: ");
  
  for (int i = 0; i < data_len; i++) {
    Serial.print((char)data[i]);
  }

  Serial.println();
}
