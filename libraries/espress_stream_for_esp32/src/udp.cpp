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

#include "udp.h"

WiFiUDP udp;

extern "C" int udp_begin(uint16_t port)
{
    return udp.begin(port);
}

extern "C" int udp_begin_packet(char const *address, uint16_t port)
{
    return udp.beginPacket(address, port);
}

extern "C" size_t udp_write(uint8_t const *buffer, size_t size)
{
    return udp.write(buffer, size);
}

extern "C" int udp_end_packet()
{
    return udp.endPacket();
}

extern "C" int udp_parse_packet()
{
    return udp.parsePacket();
}

extern "C" size_t udp_read(uint8_t *buffer, size_t size)
{
    return udp.read(buffer, size);
}

extern "C" char *udp_remote_ip(char *address, size_t size)
{
    strncpy(address, udp.remoteIP().toString().c_str(), size);
    address[size - 1] = '\0';
    return address;
}

extern "C" uint16_t udp_remote_port()
{
    return udp.remotePort();
}
