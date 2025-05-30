#include <WiFi.h>
#include <esp_now.h>
#include <math.h>

//Define struct to store received data packets
//Must match structure of sender
typedef struct struct_message {
  float r, p, y;
  float a1, a2, a3;
  float g1, g2, g3;
  int rssi;
} struct_message;

//Define global variable to store most recent data
struct_message myData;

//Converting rssi value to approximate distance in meters
float calculateDistance(int rssi) {
  int A = -45;      // RSSI at 1 meter
  float n = 2.0;    // Environmental factor
  return pow(10.0, ((A - rssi) / (10.0 * n)));
}

//Print data to serial monitor
//Separated by spaces for processing in Max
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  float distance = calculateDistance(myData.rssi);
  Serial.printf("%d %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n", myData.rssi, distance, myData.r, myData.p, myData.y, myData.a1, myData.a2, myData.a3, myData.g1, myData.g2, myData.g3);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  //Set receiver as soft access point
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP_NOW_AP", "", 1);  // Channel 1, no password
  delay(1000);

  Serial.print("Receiver MAC: ");
  Serial.println(WiFi.softAPmacAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
  Serial.println("Receiver ready and waiting for data...");
}

void loop() {
  // Empty — everything happens in the callback
}
