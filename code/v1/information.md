Established a communication between one device and another where only one device at the current time can send messages to the other.

_Sender:_
<img width="1327" height="243" alt="image" src="https://github.com/user-attachments/assets/66ed4731-9cdf-4f54-b615-9b9bd127db9e" />
The sender sends text that is inputed via serial and sends it to the receiver. Successfully sent messages is indicated via a status code of '0'.

_Receiver:_
<img width="1327" height="246" alt="image" src="https://github.com/user-attachments/assets/5e46e150-3255-49f1-95c0-b9f321ea6ad1" />
The receiver successfully recives the text and prints via Serial to see.

_How it works?_

The sender first waits for any text given via Serial. Once text has been read it encodes the string into bytes and then sends it to the receiver.
The receiver then waits for any message being sent to it. Once a message arrives it decodes the bytes back into characters and prints the completed message.
