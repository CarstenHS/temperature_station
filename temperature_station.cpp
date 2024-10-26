#include <LittleFS.h>
#include <DHT.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <base64.h>
#include <time.h>
#include <ArduinoJson.h>
#include "Config.h"

DHT dht;
String oauthUrl = String("https://login.microsoftonline.com/") + TENANT_ID + "/oauth2/v2.0/token";
String tableUrl = String("https://") + STORAGEACCOUNT_NAME + ".table.core.windows.net/" + TABLE_NAME;

// Generate PartitionKey and RowKey dynamically
String partitionKey = "SensorData";
String rowKey;

// Function to get current UTC date and time (needed for x-ms-date header)
String getCurrentUTCDate() {
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  time_t now = time(nullptr);
  struct tm *timeinfo = gmtime(&now);
  char buf[80];
  strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
  return String(buf);
}

void setup()
{
   if( WIFI_SSID == nullptr || WIFI_PASS == nullptr)
 {
   throw "wifi not set";
 }

  Serial.begin(9600);
  Serial.println();
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)");
  dht.setup(D7); // data pin 2
  WiFi.begin(WIFI_SSID, WIFI_PASS);

 while (WiFi.status() != WL_CONNECTED) 
 {
   delay(1000);
   Serial.print("\nConnecting.."); 
 }  
 Serial.println("\nConnected to Wi-Fi.");
}

// Function to get access token
String getAccessToken() {
    String response = "";
   if (WiFi.status() == WL_CONNECTED) 
   {
    WiFiClientSecure client;
    client.setInsecure();  // Use insecure connection for testing, consider using certificate
    HTTPClient http;

    // Prepare the request body
    String requestBody = "grant_type=client_credentials";
    requestBody += "&client_id=" + String(CLIENT_ID);
    requestBody += "&client_secret=" + String(CLIENT_SECRET);
    requestBody += "&scope=https%3A%2F%2Fstorage.azure.com%2F.default";

    // Set headers
    String contentType = "application/json";
    http.begin(client, oauthUrl);

    // Send POST request
    int httpResponseCode = http.POST(requestBody);
    if (httpResponseCode > 0) {
      response = http.getString();
      Serial.println("HTTP Response Code: " + String(httpResponseCode));
    } else {
      Serial.println("Error on sending POST: " + String(httpResponseCode));
    }

    http.end();  // End the HTTP request
   }
  
  DynamicJsonDocument doc(2048);
  deserializeJson(doc, response);
  
  String token = doc["access_token"];
  return token;  
}

void sendTemp(float temperature, float humidity, String token)
{
   if (WiFi.status() == WL_CONNECTED) 
   {

    WiFiClientSecure client;
    client.setInsecure();  // Use insecure connection for testing, consider using certificate

    HTTPClient http;
    rowKey = String(millis());           // Generate a unique RowKey based on time
    String date= getCurrentUTCDate();

    // JSON body for the entity to insert
    String jsonBody = "{"
                      "\"DeviceId\": \"" + DEVICE_ID + "\","
                      "\"Time\": \"" + date + "\","
                      "\"Temperature\": \"" + String(temperature) + "\","
                      "\"Humidity\": \"" + String(humidity) + "\","
                      "\"PartitionKey\": \"" + partitionKey + "\","
                      "\"RowKey\": \"" + rowKey + "\""                      
                      "}";                   

    http.begin(client, tableUrl);
    http.addHeader("Authorization", "Bearer " + token);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Accept", "application/json");
    http.addHeader("Host", "chstablestorage1.table.core.windows.net");
    http.addHeader("x-ms-version", "2019-02-02");
    http.addHeader("x-ms-date", date);

    // Send POST request
    int httpResponseCode = http.POST(jsonBody);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("HTTP Response Code insert: " + String(httpResponseCode));
    } else {
      Serial.println("Error on sending insert POST: " + String(httpResponseCode));
    }

    http.end();
   }
}

void loop()
{
  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.println(dht.toFahrenheit(temperature), 1);

  String token = getAccessToken();
  sendTemp(temperature, humidity, token);
  delay(UPLOAD_TIMEOUT);
}
