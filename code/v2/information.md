Continuing on with the project the next thing is to add communication between bother devices instead of limiting it to one way.

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

These images show messages sent between the two with the deviecs with the status codes of '0', indicating that messages was sent successfully for bother devices.
