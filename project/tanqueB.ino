// ====== Wokwi ESP32 - Tanque B (Temperatura + pH) via MQTT (HiveMQ) ======
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <time.h>

// ---------- Wi-Fi ----------
const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASS = "";

// ---------- MQTT ----------
const char* MQTT_HOST = "broker.hivemq.com";
const uint16_t MQTT_PORT = 1883;

// Tópicos do Tanque B
const char* TOPIC_TELEM  = "aquario/tanqueB/telemetry";
const char* TOPIC_STATUS = "aquario/tanqueB/status";

// Pinos
#define TEMP_PIN 32
#define PH_PIN   33

const unsigned long PUBLISH_INTERVAL_MS = 10 * 1000UL;

WiFiClient espClient;
PubSubClient mqtt(espClient);
unsigned long lastPublish = 0;

String nowISO8601() {
  time_t now; struct tm ti;
  time(&now); gmtime_r(&now, &ti);
  char buf[25]; strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", &ti);
  return String(buf);
}

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println(" OK - IP: " + WiFi.localIP().toString());

  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  Serial.print("NTP");
  time_t now = 0;
  while (now < 1700000000) { delay(200); Serial.print("."); time(&now); }
  Serial.println(" OK");
}

float readTempC() { int raw = analogRead(TEMP_PIN); return 20.0f + (raw / 4095.0f) * 10.0f; }
float readPH()    { int raw = analogRead(PH_PIN);   return (raw / 4095.0f) * 14.0f; }

void publishStatus(const char* state, bool retained) {
  StaticJsonDocument<96> doc;
  doc["ts"] = nowISO8601();
  doc["state"] = state;
  char payload[96]; serializeJson(doc, payload, sizeof(payload));
  mqtt.publish(TOPIC_STATUS, (uint8_t*)payload, strlen(payload), retained);
}

void connectMQTT() {
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  while (!mqtt.connected()) {
    String clientId = String("esp32-wokwi-tanqueB-") + String(random(0xffff), HEX);

    StaticJsonDocument<64> willDoc;
    willDoc["ts"] = "0"; willDoc["state"] = "offline";
    char willPayload[64]; serializeJson(willDoc, willPayload, sizeof(willPayload));

    Serial.print("MQTT(B)...");
    if (mqtt.connect(clientId.c_str(), NULL, NULL, TOPIC_STATUS, 0, true, willPayload)) {
      Serial.println("conectado");
      publishStatus("online", true);
    } else {
      Serial.printf("falha rc=%d, retry...\n", mqtt.state());
      delay(2000);
    }
  }
}

void publishTelemetry() {
  StaticJsonDocument<160> doc;
  doc["ts"] = nowISO8601();
  doc["aquario"] = "tanqueB";
  doc["temp_c"]  = roundf(readTempC() * 10.0f) / 10.0f;
  doc["ph"]      = roundf(readPH()    * 100.0f) / 100.0f;
  char payload[160]; serializeJson(doc, payload, sizeof(payload));
  mqtt.publish(TOPIC_TELEM, (uint8_t*)payload, strlen(payload), false);
}

void setup() {
  Serial.begin(115200); delay(200);
  analogReadResolution(12);
  analogSetPinAttenuation(TEMP_PIN, ADC_11db);
  analogSetPinAttenuation(PH_PIN,   ADC_11db);
  connectWiFi(); connectMQTT();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) connectWiFi();
  if (!mqtt.connected()) connectMQTT();
  mqtt.loop();

  unsigned long now = millis();
  if (now - lastPublish >= PUBLISH_INTERVAL_MS) {
    lastPublish = now;
    publishTelemetry();
  }
}
