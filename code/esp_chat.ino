// ============ LIBRARIES ============
#include <esp_now.h>
#include <WiFi.h>

#include <Crypto.h>
#include <AES.h>

// ============ VARIABLES ============
#define CHANNEL 1
#define BLOCK_SIZE 16
#define MAX_MESSAGE_LENGTH 128   // must be multiple of 16 for AES

String name = "device1"; // name of user

esp_now_peer_info_t peer;
AES128 aes;


// change this but keep same on all devices
byte aesKey[16] = {
  0x73, 0x19, 0xA4, 0xC8,
  0x55, 0x2F, 0x91, 0xE3,
  0x0B, 0x6D, 0x44, 0xFA,
  0x9C, 0x27, 0x81, 0x5E
};

byte plain[BLOCK_SIZE];
byte cipher[BLOCK_SIZE];
byte decrypted[BLOCK_SIZE];


// ============ FUNCTIONS ============

void encryptBlock(byte *input, byte *output) {
  aes.setKey(aesKey, 16);
  aes.encryptBlock(output, input);
}

void decryptBlock(byte *input, byte *output) {
  aes.setKey(aesKey, 16);
  aes.decryptBlock(output, input);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  delay(2000);
  Serial.println("MAC: " + WiFi.macAddress());

  uint8_t peerAddress[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

  memcpy(peer.peer_addr, peerAddress, 6);
  peer.channel = CHANNEL;
  peer.encrypt = false;

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  //esp_now_register_send_cb(onDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  if (esp_now_add_peer(&peer) != ESP_OK) {
    Serial.println("Peer add failed");
    return;
  }

  Serial.println("Setup done");
}

void loop() {
  if (Serial.available()) {
    String text = Serial.readStringUntil('\n');
    String message = name + ": " + text;

    Serial.println("You: " + text);
    send_chunk_message(message);
  }

}

void send_chunk_message(String message) {
  int len = message.length();

  for (int i = 0; i < len; i += BLOCK_SIZE) {
    memset(plain, 0, BLOCK_SIZE);
    int copyLen = BLOCK_SIZE;

    if (i + BLOCK_SIZE > len)
      copyLen = len - i;

    memcpy(plain, message.c_str() + i, copyLen);
    encryptBlock(plain, cipher);

    esp_now_send(peer.peer_addr, cipher, BLOCK_SIZE);

    delay(5);
  }
}

void onDataSent(const uint8_t *mac_address, esp_now_send_status_t status) {
  Serial.print("Send status: ");
  Serial.println(status);
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data,int data_len) {
  if (data_len != BLOCK_SIZE) return;

  decryptBlock((byte*)data, decrypted);
  for (int i = 0; i < BLOCK_SIZE; i++) {
    if (decrypted[i] == 0) break;

    Serial.print((char)decrypted[i]);
  }

  Serial.println();
}
