#if defined(ESP8266)
#include <ArduinoWiFiServer.h>
#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>
#elif defined(ESP32)
#include <WiFi.h>
#else
#error
#endif

#include <espress_stream_for_esp32.h>

#include "net_config.h"

const char *arg_wifi_ssid = WIFI_SSID;
const char *arg_wifi_password = WIFI_PASSWORD;
const char *arg_address = ADDRESS;
const char *arg_port = PORT;

extern "C" {
extern void test_main(void);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(arg_wifi_ssid, arg_wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  printf("WiFi connected (%s)\n", WiFi.localIP().toString().c_str());
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned u;
  static bool isFin = false;
  if (!isFin) {
    printf("start\n\n");
    u = espress_stream_time();
    test_main();
    u = espress_stream_time() - u;
    printf("\nfinish (elapsed time = %u us)\n", u);
    isFin = true;
  }
}
