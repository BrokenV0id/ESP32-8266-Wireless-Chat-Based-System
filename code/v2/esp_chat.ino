// ============ LIBRARIES ============
#include <esp_now.h>
#include <WiFi.h>

// ============ VARIABLES ============
#define CHANNEL 1
String name = "device1"; // name of device

esp_now_peer_info_t slave;

// ============ FUNCTIONS ============
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  delay(1000); // give serial time to establish connection
  Serial.println("Device MAC: " + WiFi.macAddress());

  uint8_t slaveAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // broadcast MAC address
  memcpy(slave.peer_addr, slaveAddress, 6);
  slave.channel = CHANNEL;
  slave.encrypt = false;

  if (esp_now_init() != ESP_OK) {
    Serial.println("Failed to init ESP-NOW.");
    return;
  }

  esp_now_register_send_cb(onDataSent);
  esp_now_register_recv_cb(OnDataRecv);
  if (esp_now_add_peer(&slave) != ESP_OK) {
    Serial.println("Failed to add peer.");
    return;
  }
}

void loop() {
  if (Serial.available()) {
    String text = Serial.readStringUntil('\n');
    String message = name + ": " + text;
    esp_now_send(slave.peer_addr, (uint8_t*)message.c_str(), message.length());
  }
}

void onDataSent(const uint8_t *mac_address, esp_now_send_status_t status) {
  Serial.print("Status: ");
  Serial.println(status);
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  for (int i = 0; i < data_len; i++) {
    Serial.print((char)data[i]);
  }

  Serial.println();
}