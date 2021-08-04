#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
#define redLED 5
#define greenLED 6
#define blueLED 2
#define servoPin 3

 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

Servo myPointer; //Create a servo object called myPointer

// Init array that will store new NUID 
byte nuidPICC[4];
int delayTime = 2000;
int servoPos = 0;

void setup() { 
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  pinMode(redLED, OUTPUT); //RED LED PINMODE
  pinMode(greenLED, OUTPUT); //GREEN LED PINMODE
  pinMode(blueLED, OUTPUT); //BLUE LED PINMODE
  digitalWrite(blueLED, HIGH);
  myPointer.attach(servoPin);
  myPointer.write(servoPos);

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
}
 
void loop() {

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] ) {
    Serial.println(F("A new card has been detected."));

    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
   
    if (rfid.uid.uidByte[0] != 41 || 
    rfid.uid.uidByte[1] != 129 || 
    rfid.uid.uidByte[2] != 141 || 
    rfid.uid.uidByte[3] != 194 ){
      Serial.println("Access Denied");
      digitalWrite(blueLED, LOW);
      digitalWrite(redLED, HIGH);
      delay(delayTime);
      digitalWrite(redLED, LOW);
      digitalWrite(blueLED, HIGH);
    }else{
      Serial.println("Access Granted");
      digitalWrite(blueLED, LOW);
      digitalWrite(greenLED, HIGH);
      for (servoPos = 0; servoPos <= 90; servoPos += 1){
        myPointer.write(servoPos);
        delay(70);
      }
      delay(delayTime);
      for (servoPos = 90; servoPos >= 0; servoPos -= 1){
        myPointer.write(servoPos);
        delay(70);
      }
      digitalWrite(greenLED, LOW);
      digitalWrite(blueLED, HIGH);
    }

  }
  else{
    Serial.println(F("Card read previously."));
    if (rfid.uid.uidByte[0] != 41 || 
    rfid.uid.uidByte[1] != 129 || 
    rfid.uid.uidByte[2] != 141 || 
    rfid.uid.uidByte[3] != 194 ){
      Serial.println("Access Denied");
      digitalWrite(blueLED, LOW);
      digitalWrite(redLED, HIGH);
      delay(delayTime);
      digitalWrite(redLED, LOW);
      digitalWrite(blueLED, HIGH);
    }else{
      Serial.println("Access Granted");
      digitalWrite(blueLED, LOW);
      digitalWrite(greenLED, HIGH);
      for (servoPos = 0; servoPos <= 90; servoPos += 1){
        myPointer.write(servoPos);
        delay(70);
      }
      delay(delayTime);
      for (servoPos = 90; servoPos >= 0; servoPos -= 1){
        myPointer.write(servoPos);
        delay(70);
      }
      digitalWrite(greenLED, LOW);
      digitalWrite(blueLED, HIGH);
      
    }
  } 

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}


/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}
