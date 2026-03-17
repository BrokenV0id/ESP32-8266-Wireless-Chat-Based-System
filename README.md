# ESP32-8266-Wireless-Chat-Based-System
A text-based chat system that allows for wireless communication between ESP32 and ESP8266 devices using the protocol ESP-NOW.

**This project serves as way to learn Espressifs protocol ESP-NOW. This is not a project that will receive constant updates in days that follow.**
***
This project allows for wireless communication between multiple ESP32 or ESP8266 devices at once using the ESP-NOW protocol. 

_Features:_
1. Plug and play - Upload the code and it should work straight away.
2. Name identification - Received messages will show who they are from based on a user set name.
3. Wide user support - Allows for multple devices to communicate at once. While the exact number is unknown, it is believed to
   handle more then 5 users.
4. Message chunking - Messages exceeding a certain length will be split up and sent individually.
5. Message encryption - Messages are now encrypted and only devices with the same key can read sent messages.

_Examples:_
<img width="1328" height="258" alt="image" src="https://github.com/user-attachments/assets/406142ec-3c8b-4c3f-9476-71bb154edf84" />
<img width="1328" height="243" alt="image" src="https://github.com/user-attachments/assets/0d784d00-e589-4ed1-bca0-b2d320fd09ae" />


**Note - A status message will appear on the device that sent the message indicating if the message sent or not. '0' means it did while a '1' means it didn't.**
