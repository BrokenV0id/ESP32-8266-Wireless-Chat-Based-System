// ============ LIBRARIES ============
#include <esp_now.h>
#include <WiFi.h>

// ============ VARIABLES ============
#define CHANNEL 1

esp_now_peer_info_t receiver;

// ============ FUNCTIONS ============
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  Serial.println("Device MAC: " + WiFi.macAddress());

  uint8_t receiverAddress[] = {0x4C, 0xC3, 0x82, 0xBE, 0xA5, 0xF4}; // change with mac address of device you want to talk to
  memcpy(receiver.peer_addr, receiverAddress, 6);
  receiver.channel = CHANNEL;
  receiver.encrypt = false;

  if (esp_now_init() != ESP_OK) {
    Serial.println("Failed to init ESP-NOW.");
    return;
  }

  esp_now_register_send_cb(onDataSent);
  if (esp_now_add_peer(&receiver) != ESP_OK) {
    Serial.println("Failed to add peer.");
    return;
  }
}

void loop() {
  if (Serial.available()) {
    String message = Serial.readStringUntil('\n');
    esp_now_send(receiver.peer_addr, (uint8_t*)message.c_str(), message.length());
  }

  delay(2000);
}

void onDataSent(const uint8_t *mac_address, esp_now_send_status_t status) {
  Serial.print("Status: ");
  Serial.println(status);
}
