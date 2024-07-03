#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
int pin = 2;

//#define BLYNK_TEMPLATE_ID "TMPLAhjSDFhl"
//#define BLYNK_TEMPLATE_NAME "ESP32 wifi test"
//#define BLYNK_AUTH_TOKEN "zYtCREIzhegPZFo_d_pegmy3BCBx1mbP"

char auth[] = "zYtCREIzhegPZFo_d_pegmy3BCBx1mbP";       // You should get Auth Token in the Blynk App.
char ssid[] = "4GWiFi_53537";                    // Your Wi-Fi Credentials
char pass[] = "12345678";
void setup() {  
  pinMode(pin, OUTPUT); 
  pinMode(pin, HIGH);
  Serial.begin(115200);
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  int wifi_ctr = 0;
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
  Serial.println("WiFi connected");  
  Blynk.begin("Your auth. Token", ssid, pass);
}
void loop(){
    Blynk.run();
}
