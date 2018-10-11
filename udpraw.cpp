#include <Arduino.h>
#include <WiFiUdp.h>

#include "ESPixelStick.h"
#include "udpraw.h"

extern  config_t        config;

WiFiUDP             RAWudp;             /* Raw UDP packet Server */
unsigned int        RAWPort = 2801;      // local port to listen for UDP packets
unsigned long       RAW_ctr = 0;

void setupUDPraw() {

  if (!RAWudp) {
    LOG_PORT.println("RE-Starting UDP");
    RAWudp.begin(RAWPort);
    MDNS.addService("hyperiond-rgbled", "udp", RAWPort);
    LOG_PORT.print("Local RAWport: ");
    LOG_PORT.println(RAWudp.localPort());
  }
}

void handleUDPraw() {
  if (!RAWudp) {
    return;
  }

// Do we have a packet?
  int size = RAWudp.parsePacket();

// yep, go read it
  if (size) {
    int i=0;
    for (i = 0; i < _min(size,config.channel_count); i++) {
#if defined(ESPS_MODE_PIXEL)
        pixels.setValue(i, RAWudp.read());
#elif defined(ESPS_MODE_SERIAL)
        serial.setValue(i, RAWudp.read());
#endif
    }
    /* fill the rest with 0s*/
    for (      ; i < config.channel_count; i++) {
#if defined(ESPS_MODE_PIXEL)
        pixels.setValue(i, 0);
#elif defined(ESPS_MODE_SERIAL)
        serial.setValue(i, 0);
#endif
    }
  }
}


