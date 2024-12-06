#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>

// RFID Module Pins
#define RST_PIN D3 // Reset pin
#define SS_PIN D4  // Slave Select (SDA) pin

// LED Pins
#define VALID_LED D1    // LED for valid card
#define INVALID_LED D2  // LED for invalid card

// Servo Pin
#define SERVO_PIN D8    // Servo control pin (reassigned)

// Buzzer Pin
#define BUZZER_PIN D0   // Buzzer control pin

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance
Servo servoMotor;                // Servo motor instance

// WiFi Credentials
const char* ssid = "KALHARAHOME";
const char* password = "1ca81d0D";

// API Endpoint
const char* apiEndpoint = "http://192.99.35.63:3000/validate-card";

WiFiClient client;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  Serial.println("Initializing...");

  // Initialize SPI and RFID reader
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("RFID reader initialized.");

  // Initialize LED and buzzer pins
  pinMode(VALID_LED, OUTPUT);
  pinMode(INVALID_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(VALID_LED, LOW);
  digitalWrite(INVALID_LED, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // Initialize Servo
  servoMotor.attach(SERVO_PIN);
  servoMotor.write(0); // Start at 0 degrees

  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
}

void successTone() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200); // Short beep for success
  digitalWrite(BUZZER_PIN, LOW);
}

void errorTone() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(150); // Quick beep for error
    digitalWrite(BUZZER_PIN, LOW);
    delay(150); // Small gap between beeps
  }
}

void loop() {
  // Check if a card is present
  if (!mfrc522.PICC_IsNewCardPresent()) {
    delay(500); // Wait and check again
    return;
  }

  // Read the card's UID
  if (!mfrc522.PICC_ReadCardSerial()) {
    Serial.println("Error reading card.");
    delay(500); // Wait and check again
    return;
  }

  // Construct the card UID string
  String cardUID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    cardUID += String(mfrc522.uid.uidByte[i], HEX);
  }
  while (cardUID.length() < 8) {
    cardUID = "0" + cardUID;
  }
  cardUID.toUpperCase();

  Serial.print("Card UID: ");
  Serial.println(cardUID);

  // Send the card UID to the API
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(client, apiEndpoint);
    http.addHeader("Content-Type", "application/json");

    String payload = "{\"cardUID\":\"" + cardUID + "\"}";
    Serial.println("Sending POST request to API...");
    int httpCode = http.POST(payload);

    Serial.print("HTTP Response code: ");
    Serial.println(httpCode);

    if (httpCode > 0) {
      String response = http.getString();
      Serial.println("API Response: " + response);

      if (response.indexOf("\"success\":true") != -1) {
        Serial.println("Card is valid.");
        digitalWrite(VALID_LED, HIGH); // Turn on valid card LED
        successTone();                 // Play success tone
        servoMotor.write(180);        // Rotate servo to 180 degrees
        delay(3000);                  // Wait 3 seconds
        servoMotor.write(0);          // Reset servo to 0 degrees
        digitalWrite(VALID_LED, LOW); // Turn off valid card LED
      } else {
        Serial.println("Card is invalid.");
        digitalWrite(INVALID_LED, HIGH); // Turn on invalid card LED
        errorTone();                     // Play error tone
        delay(2000);                     // Wait 2 seconds
        digitalWrite(INVALID_LED, LOW);  // Turn off invalid card LED
      }
    } else {
      Serial.println("Error: Failed to send API request.");
    }

    http.end();
  } else {
    Serial.println("Error: WiFi disconnected.");
  }

  // Halt the reader to prepare for the next scan
  mfrc522.PICC_HaltA();
}
