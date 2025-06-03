#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <MFRC522.h>
#include <WiFiClientSecure.h> // For HTTPS
#include <WiFiClient.h>
#include <Servo.h>

Servo gateServo;
#define SERVO_PIN 5 // D4 (GPIO2)

#define SS_PIN 2
#define RST_PIN 0
MFRC522 mfrc522(SS_PIN, RST_PIN);

// WiFi credentials
const char* ssid = "oppoReno";
const char* password = "12345678";

// ThingSpeak settings
const char* thingSpeakServer = "api.thingspeak.com";
const unsigned long channelID = 2929829;
const char* writeAPIKey = "Y3TH0DBCYMTL7XT1";

// Telegram settings
const char* telegramBotToken = "7810056173:AAFKSBV69XBbDLgaJAXC6ddXEt-dEzrJZ2Y"; // 🔁 Replace this
const char* chatID = "1261477420";             // 🔁 Replace this
const char* telegramHost = "api.telegram.org";

WiFiClient client;
WiFiClientSecure telegramClient;

void setup() {
  gateServo.attach(SERVO_PIN);
  gateServo.write(0); // Initial position

  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  Serial.println("RFID system ready. Scan your card...");

  // Connect to WiFi
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.println(WiFi.localIP());

  ThingSpeak.begin(client);
  telegramClient.setInsecure(); // Required for HTTPS with ESP8266
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    content += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    content += String(mfrc522.uid.uidByte[i], HEX);
    if (i < mfrc522.uid.size - 1) content += " ";
  }
  content.toUpperCase();

  Serial.println("Scanned UID: " + content);

  if (content == "A3 B0 5E 14") {
    Serial.println("✅ Authorized access");
    gateServo.write(180);        // Rotate to 90°
    delay(10000);                // Wait 5 seconds
    gateServo.write(0);         // Return to original position
  } else {
    Serial.println("❌ Access denied");
    sendThingSpeakAlert(content);
    sendTelegramAlert(content);
  }

  delay(1000);
}

void sendThingSpeakAlert(String uid) {
  ThingSpeak.setField(1, uid);
  ThingSpeak.setField(2, "Unauthorized");
  ThingSpeak.setField(3, String(millis()));

  int httpCode = ThingSpeak.writeFields(channelID, writeAPIKey);
  if (httpCode == 200) {
    Serial.println("✅ ThingSpeak update successful");
  } else {
    Serial.println("❌ Failed to update ThingSpeak");
  }
}

void sendTelegramAlert(String uid) {
  if (telegramClient.connect(telegramHost, 443)) {
    String message = "🚨 Unauthorized RFID Detected! UID: " + uid;

    String url = "/bot" + String(telegramBotToken) + "/sendMessage?chat_id=" + String(chatID) + "&text=" + message;


    telegramClient.print(String("GET ") + url + " HTTP/1.1\r\n" +
                         "Host: " + telegramHost + "\r\n" +
                         "Connection: close\r\n\r\n");
    while (telegramClient.connected()) {
    String line = telegramClient.readStringUntil('\n');
    Serial.println(line); // 💬 Print Telegram API response
}


    Serial.println("📩 Telegram alert sent!");
  } else {
    Serial.println("⚠️ Failed to connect to Telegram server.");
  }

  delay(1000);
  telegramClient.stop();
}
