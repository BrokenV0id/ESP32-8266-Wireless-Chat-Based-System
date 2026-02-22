# ESP32-8266-Wireless-Chat-Based-System
A text-based chat system that allows for wireless communication between ESP32 and ESP8266 devices using the protocol ESP-NOW.




**So far (V1):**

Established a communication between one device and another where only one device at the current time can send messages to the other.

_Sender:_
<img width="1327" height="243" alt="Screenshot_20260222_130808" src="https://github.com/user-attachments/assets/6642b8b7-ccf2-4f69-a2fc-2a87eb2e80d6" />
The sender sends text that is inputed via serial and sends it to the receiver. Successfully sent message is indicated via a status code of '0'.

_Receiver:_
<img width="1327" height="246" alt="Screenshot_20260222_130726" src="https://github.com/user-attachments/assets/b39960b3-ad30-4da9-b564-2ff3a028a09f" />
The receiver successfully recives the text and prints via Serial to see.

_How it works?_

The sender first waits for any text given via Serial. Once text has been read it encodes the string into bytes and then sends it to the receiver.The receiver then waits for any message being sent to it. Once a message arrives it decodes the bytes back into characters and prints the completed message.
