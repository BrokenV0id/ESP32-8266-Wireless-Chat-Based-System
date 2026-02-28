// ============ LIBRARIES ============
#include <esp_now.h>
#include <WiFi.h>
#include <BluetoothSerial.h>

// ============ VARIABLES ============
#define CHANNEL 1
#define MAX_MESSAGE_LENGTH 150

String name = "device1"; // name of device
const bool device_logs = false; // set true if you want 

esp_now_peer_info_t peer;

BluetoothSerial SerialBT;

// ============ FUNCTIONS ============
void setup() {
  SerialBT.begin("ESP32");
  WiFi.mode(WIFI_STA);

  delay(2000); // give serial time to establish connection
  SerialBT.println("Device MAC: " + WiFi.macAddress());

  uint8_t peerAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // broadcast MAC address
  memcpy(peer.peer_addr, peerAddress, 6);
  peer.channel = CHANNEL;
  peer.encrypt = false;

  if (esp_now_init() != ESP_OK) {
    SerialBT.println("Failed to init ESP-NOW.");
    return;
  }

  esp_now_register_send_cb(onDataSent);
  esp_now_register_recv_cb(OnDataRecv);
  if (esp_now_add_peer(&peer) != ESP_OK) {
    SerialBT.println("Failed to add peer.");
    return;
  }

  SerialBT.println("Setup Complete.");
}

void loop() {
  if (SerialBT.available()) {
    String text = SerialBT.readStringUntil('\n');
    String message = name + ": " + text;

    if (message.length() < MAX_MESSAGE_LENGTH) {
      SerialBT.println("You: " + text);
      esp_now_send(peer.peer_addr, (uint8_t*)message.c_str(), message.length());
    } else {
      send_chunk_message(text);
    }
  }
}

void send_chunk_message(String message) {
  String sender_info = name + ": ";
  esp_now_send(peer.peer_addr, (uint8_t*)sender_info.c_str(), message.length());
  SerialBT.print("You: ");

  // send chunks
  for (int i = 0; i < message.length(); i += MAX_MESSAGE_LENGTH) {
    String part = message.substring(i, i + MAX_MESSAGE_LENGTH);
    esp_now_send(peer.peer_addr, (uint8_t*)part.c_str(), message.length());
    SerialBT.print(part);
  }

  SerialBT.println();
}

void onDataSent(const uint8_t *mac_address, esp_now_send_status_t status) {
  if (device_logs == true) {
    SerialBT.print("Status: ");
    SerialBT.println(status);
  }
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  for (int i = 0; i < data_len; i++) {
    SerialBT.print((char)data[i]);
  }

  SerialBT.println();
}