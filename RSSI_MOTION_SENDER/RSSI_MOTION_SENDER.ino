#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <CodeCell.h>  

CodeCell myCodeCell;

// Motion variables
float Roll = 0.0, Pitch = 0.0, Yaw = 0.0;
float ax = 0.0, ay = 0.0, az = 0.0;
float gx = 0.0, gy = 0.0, gz = 0.0;

// Receiver's MAC address (from Serial Monitor)
uint8_t receiverMac[] = {0xE8, 0x06, 0x90, 0x2A, 0x7E, 0x51};  

//Define struct to store sent data packets
//Must match structure of receiver
typedef struct struct_message {
  float r, p, y;
  float a1, a2, a3;
  float g1, g2, g3;
  int rssi;
} struct_message;

//Define global variable to store most recent data
struct_message myData;
esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_STA);
  WiFi.begin("ESP_NOW_AP");  // Connect to receiver’s SoftAP

  Serial.println("Connecting to ESP_NOW_AP...");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFailed to connect to ESP_NOW_AP");
    return;
  }

  Serial.println("\nConnected!");
  Serial.print("RSSI to AP: ");
  Serial.println(WiFi.RSSI());

  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);  // Match receiver's channel

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

//Register receiver as peer
  memcpy(peerInfo.peer_addr, receiverMac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

//Initialize motion sensing
  myCodeCell.Init(MOTION_ROTATION + MOTION_ACCELEROMETER + MOTION_GYRO);
}

//Read motion sensing  values
void loop() {
  if (myCodeCell.Run(10)) {
    myCodeCell.Motion_RotationRead(Roll, Pitch, Yaw);
    myCodeCell.Motion_AccelerometerRead(ax, ay, az);
    myCodeCell.Motion_GyroRead(gx, gy, gz);
  }

//Store data in the struct message
  myData = { Roll, Pitch, Yaw, ax, ay, az, gx, gy, gz, WiFi.RSSI() };

  esp_err_t result = esp_now_send(receiverMac, (uint8_t *)&myData, sizeof(myData));
  if (result == ESP_OK) {
    Serial.println("Data sent successfully.");
  } else {
    Serial.println("Send failed!");
  }

  delay(100);  //data read every 100 milliseconds
}
