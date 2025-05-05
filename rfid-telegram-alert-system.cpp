#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <MFRC522.h>
#include <WiFiClientSecure.h> // For HTTPS
#include <WiFiClient.h>

#define SS_PIN 2
#define RST_PIN 0
MFRC522 mfrc522(SS_PIN, RST_PIN);

// WiFi credentials
const char* ssid = "Wifi-name";
const char* password = "wifi-password";

// ThingSpeak settings
const char* thingSpeakServer = "api.thingspeak.com";
const unsigned long channelID = ;//enter channel id of the project from thingspeak
const char* writeAPIKey = "Your write api key";

// Telegram settings
const char* telegramBotToken = "Bot token from telegram"; 
const char* chatID = "chat id from telegram";             
const char* telegramHost = "api.telegram.org";

WiFiClient client;
WiFiClientSecure telegramClient;

void setup() {
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
