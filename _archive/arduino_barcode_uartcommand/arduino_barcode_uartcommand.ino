#include <SoftwareSerial.h>

SoftwareSerial ScannerSerial(10, 11); // RX, TX

bool typeStringComplete = false;
String typeString = "";
String inputString = "";  // String to store incoming data

const uint8_t scanCommand[] = {0x7E, 0x00, 0x08, 0x01, 0x00, 0x02, 0x01, 0xAB, 0xCD};
const int scanCommandLength = 9;
const uint8_t scanResponse[] = {0x02, 0x00, 0x00, 0x01, 0x00, 0x33, 0x31};
const int scanResponseLength = 7;

void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 bps

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  Serial.println("Serial connected!");

  
  // set the data rate for the SoftwareSerial port
  ScannerSerial.begin(9600);

  Serial.println("ScannerSerial connected!");

}

void loop() {


  if (typeStringComplete) {
    if (typeString.startsWith("scan")) {
      Serial.println("Asking scanner to scan...");


      for (int i = 0; i < scanCommandLength; i++) {
        ScannerSerial.write(scanCommand[i]);
      }


    }
    
    typeString = "";  //clear the string
    typeStringComplete = false;

  }



  if (ScannerSerial.available()) {
    char receivedChar = ScannerSerial.read();  // Read the incoming character


    // Check if the received character is the carriage return (hex value 0x0D)
    if (receivedChar == '\r') {  // '\r' is the escape character for carriage return
      Serial.println(inputString);  // Print the complete string
      inputString = "";  // Clear the string for new data
    } else {
      inputString += receivedChar;  // Add the received character to the string
    }

    if (inputString.length() >= scanResponseLength) {


      uint8_t inputStringByteArray[scanResponseLength];
      for (int i = 0; i < scanResponseLength; i++) {
        inputStringByteArray[i] = (uint8_t)inputString.charAt(i);
      }

      // Compare the extracted array to the target sequence
      if (memcmp(inputStringByteArray, scanResponse, scanResponseLength) == 0) {
        Serial.println("Scanner is scanning!");
        inputString = "";  // Clear the String if a match is found or restart the match process
      }

    }
  }
  delay(5);
}

//Reading the Serial Monitor
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    typeString += inChar;
    if (inChar == '\n') {
      typeStringComplete = true;
    }
  }
}
