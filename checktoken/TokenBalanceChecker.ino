// ===== Neuraiproject ========

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// ======= User-configurable parameters =======
const char* WIFI_SSID     = "YOUR_SSID";                // WiFi SSID
const char* WIFI_PASS     = "YOUR_PASSWORD";            // WiFi password
const char* RPC_URL       = "https://rpc-main.neurai.top/rpc";  // RPC endpoint
const char* ADDRESS       = "ADDRESS";               // Blockchain address
const char* TOKEN_NAME    = "TOKEN_NAME";                    // Token name
// =============================================

int CHECK = 0;  // Will be set to 1 if balance >= 1, or 0 otherwise

void setup() {
  Serial.begin(115200);
  delay(100);

  // Connect to WiFi
  Serial.print("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClientSecure client;
    client.setInsecure();  // Skip certificate validation

    // Initialize HTTP request
    http.begin(client, RPC_URL);
    http.addHeader("Content-Type", "application/json");

    // Build JSON payload: {"method":"listassetbalancesbyaddress","params":["DIRECCION"]}
    String payload = String("{\"method\":\"listassetbalancesbyaddress\",\"params\":[\"") 
                     + ADDRESS + "\"]}";

    // Send POST
    int httpCode = http.POST(payload);
    if (httpCode == HTTP_CODE_OK) {
      String response = http.getString();

      // Parse JSON response
      // Expected structure: { "result": { "ABCD": <value>, ... }, ... }
      const size_t capacity = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE( /* result */ 10 ) + 200;
      DynamicJsonDocument doc(capacity);
      DeserializationError err = deserializeJson(doc, response);

      if (!err) {
        // Extract the numeric value of result[TOKEN_NAME]
        double balance = 0.0;
        JsonVariant result = doc["result"][TOKEN_NAME];
        if (result.is<double>() || result.is<long>() || result.is<int>()) {
          balance = result.as<double>();
        } else {
          Serial.println("Token not found or invalid format");
        }

        // Update CHECK according to the balance
        if (balance >= 1.0) {
          CHECK = 1;
        } else {
          CHECK = 0;
        }

        Serial.print("Balance of ");
        Serial.print(TOKEN_NAME);
        Serial.print(" = ");
        Serial.print(balance, 8);
        Serial.print("  →  CHECK = ");
        Serial.println(CHECK);
      } else {
        Serial.print("JSON parse error: ");
        Serial.println(err.c_str());
      }
    } else {
      Serial.print("HTTP error: ");
      Serial.println(httpCode);
    }

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }

  delay(30000);  // Wait 30 seconds before next check
}
