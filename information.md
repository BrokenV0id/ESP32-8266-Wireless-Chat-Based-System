The first thing to do was to create two seperate files and make sure both included the libraires that was needed.
```
#include <esp_now.h>
#include <WiFi.h>
```
Next declare what channel the devices will communicate on. (1-14)
```
#define CHANNEL 1
```

Now in the sender program we declare a variable that will store information relating to the receiver.
```
esp_now_peer_info_t receiver;
```

And now we can begin coding the setup function.
We start by declaring Serial and making the ESP act as a WiFi station.
```
Serial.begin(115200);
WiFi.mode(WIFI_STA);
```

We now set the peer variable to include the receiver's MAC address as well as what channel to communicate on and if to encryt data. For this project I deicded not too.
```
uint8_t receiverAddress[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // change with mac address of device you want to talk to
memcpy(receiver.peer_addr, receiverAddress, 6);
receiver.channel = CHANNEL;
receiver.encrypt = false;
```

And to find the MAC address of the receiver run this code.
```
// ============ LIBRARIES ============
#include <WiFi.h>

// ============ FUNCTIONS ============

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  delay(3000); // give time for serial to establish

  Serial.println("Device MAC: " + WiFi.macAddress()); // print MAC address of device
}

void loop() {
  // put your main code here, to run repeatedly:
}
```

Once you have the MAC address replace the values inside the {} with it.
```
uint8_t receiverAddress[] = {}; // <- put it here
```

Now we use "esp_now_init()" to start ESP-NOW and use a if statement to check if it successfully started or not.
```
if (esp_now_init() != ESP_OK) {
  Serial.println("Failed to init ESP-NOW.");
  return;
}
```

Next we register a function that will be called everytime a message is sent.
```
esp_now_register_send_cb(onDataSent);
```

And the function will print the status of the sent message. '0' means it sent and '1' means it didn't.
```
void onDataSent(const uint8_t *mac_address, esp_now_send_status_t status) {
  Serial.print("Status: ");
  Serial.println(status);
}
```

And now to finish the setup function, add 'esp_now_add_peer(&receiver)' so that the ESP knows who to talk too.
```
if (esp_now_add_peer(&receiver) != ESP_OK) {
  Serial.println("Failed to add peer.");
  return;
}
```

Now the finished setup function should look like this.
```
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  Serial.println("Device MAC: " + WiFi.macAddress());

  uint8_t receiverAddress[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
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
```

And all that's left to add to the loop function is.
```
esp_now_send(receiver.peer_addr, (uint8_t*)message.c_str(), message.length());
```
This will send the message to receiver.

After adding some of my own logic to read given text from Serial to then send. The finished loop function.
```
void loop() {
  if (Serial.available()) {
    String message = Serial.readStringUntil('\n');
    esp_now_send(receiver.peer_addr, (uint8_t*)message.c_str(), message.length());
  }

  delay(2000);
}
```

Now after all that the sender script is now complete and should look like.
```
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

  uint8_t receiverAddress[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
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
```

Now all thats left is program the reciever script to handle incoming messages.

Most of it remains the same except for after we start ESP-NOW we instead link a callback to a different function with.
```
esp_now_register_recv_cb(OnDataRecv);
```

So now setup should look like this for the receiver.
```
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  esp_now_init();
  esp_now_register_recv_cb(OnDataRecv);
}
```

And for a function to handle incoming messages, we will just get the data from a parameter and deocde it into the final message.
```
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  Serial.print("Recived: ");
  
  for (int i = 0; i < data_len; i++) {
    Serial.print((char)data[i]);
  }

  Serial.println();
}
```

And with that the receiver script is complete and should look like this.
```
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
```

When both scirpts are uplaoded and ran we now get a demonstration of it successfully working.

_Sender:_
<img width="1327" height="243" alt="image" src="https://github.com/user-attachments/assets/66ed4731-9cdf-4f54-b615-9b9bd127db9e" />
The sender sends text that is inputed via serial and sends it to the receiver. Successfully sent messages is indicated via a status code of '0'.

_Receiver:_
<img width="1327" height="246" alt="image" src="https://github.com/user-attachments/assets/5e46e150-3255-49f1-95c0-b9f321ea6ad1" />
The receiver successfully recives the text and prints via Serial to see.

Continuing on with the project the next thing is to add communication between both devices instead of limiting it to one way.

By changing the peer MAC address to '0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF' the device begins broadcasting its data to anyone that
can hear it. 
This allows for multiple deviecs to communicate to each other without the need of knowing each devieces personal MAC address.

Next I modify the sender code to also print to the serial any information it detects. 
As a result of this a single script can act as bother the receiver and sender, mimicing the functionality of a regular text messaging app.

```
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  Serial.print("Recived: ");
  
  for (int i = 0; i < data_len; i++) {
    Serial.print((char)data[i]);
  }

  Serial.println();
}
```
While the function is transfered from the receiver script into sender script it allows for a single script to act as both.

Also add a link to the function so that it's called whenever information is recevied.
```
esp_now_register_recv_cb(OnDataRecv);
```

To make it easier while testing I will modify both devices to have a unqiue name making it easier to know the difference between the two.
```
String name = "device: ";
```

After adding these new features a completed stand alone script now operates and allows devices to receive and send messages among eachother.

Device 1:
<img width="1328" height="258" alt="image" src="https://github.com/user-attachments/assets/a9e0a24c-9fcb-448d-8240-ff5daafe7e7e" />


Device 2:
<img width="1328" height="243" alt="image" src="https://github.com/user-attachments/assets/de055fc2-4062-4de3-ab3d-1525fac24daa" />

These images show messages sent between the two with the deviecs with the status code of '0', indicating that messages was sent successfully for bother devices.
