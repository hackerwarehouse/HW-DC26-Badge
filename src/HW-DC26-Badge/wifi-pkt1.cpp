#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include <stdio.h>
#include <string>
#include <cstddef>
#include <Wire.h>
#include <Preferences.h>
using namespace std;

#include <SSD_13XX.h>

#include "core.h"

extern SSD_13XX mydisp;
extern byte btncounter;
extern volatile byte btnid;
extern byte appmode;
extern byte region_id;

///////////////////////////////////////////////////////////////////////////////////////////

#define SNAP_LEN 2324   // max len of each recieved packet
Preferences preferences;

esp_err_t event_handler(void* ctx, system_event_t* event) {
  return ESP_OK;
}

uint32_t lastDrawTime;
uint32_t lastButtonTime;
uint32_t tmpPacketCounter;
uint32_t pkts[MAX_X];       // here the packets per second will be saved
uint32_t deauths = 0;       // deauth frames per second
unsigned int ch = 1;        // current 802.11 channel
int rssiSum;

double getMultiplicator() {
  uint32_t maxVal = 1;
  for (int i = 0; i < MAX_X; i++) {
    if (pkts[i] > maxVal) maxVal = pkts[i];
  }
  if (maxVal > MAX_Y) return (double)MAX_Y / (double)maxVal;
  else return 1;
}

void wifi_promiscuous(void* buf, wifi_promiscuous_pkt_type_t type) {
  wifi_promiscuous_pkt_t* pkt = (wifi_promiscuous_pkt_t*)buf;
  wifi_pkt_rx_ctrl_t ctrl = (wifi_pkt_rx_ctrl_t)pkt->rx_ctrl;

  if (type == WIFI_PKT_MGMT && (pkt->payload[0] == 0xA0 || pkt->payload[0] == 0xC0 )) deauths++;

  if (type == WIFI_PKT_MISC) return;             // wrong packet type
  if (ctrl.sig_len > SNAP_LEN) return;           // packet too long

  uint32_t packetLength = ctrl.sig_len;
  if (type == WIFI_PKT_MGMT) packetLength -= 4;  // fix for known bug in the IDF https://github.com/espressif/esp-idf/issues/886

  //Serial.print(".");
  tmpPacketCounter++;
  rssiSum += ctrl.rssi;

  //if (useSD) sdBuffer.addPacket(pkt->payload, packetLength);
}

void setChannel(int newChannel) {
  ch = newChannel;
  if (ch > 14 || ch < 1) ch = 1;

  preferences.begin("packetmonitor32", false);
  preferences.putUInt("channel", ch);
  preferences.end();

  esp_wifi_set_promiscuous(false);
  esp_wifi_set_channel(ch, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous_rx_cb(&wifi_promiscuous);
  esp_wifi_set_promiscuous(true);
}

void draw() {
  double multiplicator = getMultiplicator();
  int len;
  int rssi;

  if (pkts[MAX_X - 1] > 0) rssi = rssiSum / (int)pkts[MAX_X - 1];
  else rssi = rssiSum;

  mydisp.clearScreen();

  mydisp.setCursor(0, 0);
  mydisp.print("CH: ");
  mydisp.print(ch);
  mydisp.setCursor(30, 0);
  mydisp.print("PKTS: ");
  mydisp.print(tmpPacketCounter);
  mydisp.setCursor(0, 10);
  mydisp.print("DEAUTHS: ");
  mydisp.print(deauths);

  mydisp.drawLine(0, 63 - MAX_Y, MAX_X, 63 - MAX_Y, WHITE);
  for (int i = 0; i < MAX_X; i++) {
    len = pkts[i] * multiplicator;
    mydisp.drawLine(i, 63, i, 63 - (len > MAX_Y ? MAX_Y : len), WHITE);
    if (i < MAX_X - 1) pkts[i] = pkts[i + 1];
  }
}

void Pkt_Monitor(){
  appmode=1;
  btnid = 0;
  
  byte maxCh;
  if (region_id == 1) { maxCh = 11; }
  else if (region_id == 2) { maxCh = 13; }

  preferences.begin("packetmonitor32", false);
  ch = preferences.getUInt("channel", 1);
  preferences.end();
  // System & WiFi
  nvs_flash_init();
  tcpip_adapter_init();
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  //ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  //ESP_ERROR_CHECK(esp_wifi_set_country(WIFI_COUNTRY_EU));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL));
  ESP_ERROR_CHECK(esp_wifi_start());

  esp_wifi_set_channel(ch, WIFI_SECOND_CHAN_NONE);

  // start Wifi sniffer
  esp_wifi_set_promiscuous_rx_cb(&wifi_promiscuous);
  esp_wifi_set_promiscuous(true);

  uint32_t currentTime;

  while (btnid != 4)
  {
    if (btnid == 3){ 
      if (ch == maxCh) {ch = 1;} 
      else {ch++;}
      setChannel(ch);
      draw();
      btnid = 0;
    }
    else if (btnid == 2){ 
      if (ch == 1) {ch = maxCh; } 
      else {ch--; }
      setChannel(ch);
      draw();
      btnid = 0;
    }
    else if (btnid == 4) {break;}

    currentTime = millis();
    lastButtonTime = currentTime;
    if (currentTime - lastButtonTime >= 2000) { draw(); }

    if ( currentTime - lastDrawTime > 1000 ) 
      {
      lastDrawTime = currentTime;
      // Serial.printf("\nFree RAM %u %u\n", heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT), heap_caps_get_minimum_free_size(MALLOC_CAP_32BIT));// for debug purposes

      pkts[MAX_X - 1] = tmpPacketCounter;
      draw();

      //Serial.println((String)pkts[MAX_X - 1]);

      tmpPacketCounter = 0;
      deauths = 0;
      rssiSum = 0;
      }
    delay(100);
    
  }
  appmode=0;
  btncounter++;

  esp_wifi_set_promiscuous(false);
  esp_wifi_restore();
  ESP.restart(); // can't figure out how to clean reset from promiscuous mode, so do a reset instead
}

///////////////////////////////////////////////////////////////////////////////////////////

//void chn_deauth_detect_display(){
//  mydisp.clearScreen();
//  mydisp.setTextColor(RED);
//  mydisp.setCursor(0, 0);
//  mydisp.setTextScale(1);
//  mydisp.println(F("Chn: current / high"));
//  mydisp.println(" 1: " + (String)val[1] + "/" + (String)highval[1] + "     8: " + (String)val[8] + "/" + (String)highval[8]);
//  mydisp.println(" 2: " + (String)val[2] + "/" + (String)highval[2] + "     9: " + (String)val[9] + "/" + (String)highval[9]);
//  mydisp.println(" 3: " + (String)val[3] + "/" + (String)highval[3] + "    10: " + (String)val[10] + "/" + (String)highval[10]);
//  mydisp.println(" 4: " + (String)val[4] + "/" + (String)highval[4] + "    11: " + (String)val[11] + "/" + (String)highval[11]);
//  if (region_id == 1) { mydisp.println(" 5: " + (String)val[5] + "/" + (String)highval[5]); }
//  else { mydisp.println(" 5: " + (String)val[5] + "/" + (String)highval[5] + "    12: " + (String)val[12] + "/" + (String)highval[12]); }
//  if (region_id == 1) { mydisp.println(" 6: " + (String)val[6] + "/" + (String)highval[6]); }
//  else { mydisp.println(" 6: " + (String)val[6] + "/" + (String)highval[6] + "    13: " + (String)val[13] + "/" + (String)highval[13]); }
//  if (region_id == 3) { mydisp.println(" 7: " + (String)val[7] + "/" + (String)highval[7] + "    14: " + (String)val[14] + "/" + (String)highval[14]); }
//  else { mydisp.println(" 7: " + (String)val[7] + "/" + (String)highval[7]); }
//}
//
//void DeauthDetect(){
//  appmode=1;
//  btnid = 0;
//  
//  byte curchn = 1;
//  byte maxCh;
//  unsigned long prevTime = 0;
//  unsigned long curTime = 0;
//
//  if (region_id == 1) { maxCh = 11; }
//  else if (region_id == 2) { maxCh = 13; }
//
//  for(int i=1;i<maxCh;i++){
//    val[i] = 0;
//    highval[i] = 0;
//  }
//
//  wifi_set_opmode(STATION_MODE);
//  wifi_promiscuous_enable(0);
//  WiFi.disconnect();
//  wifi_set_promiscuous_rx_cb(pkt_sniffer_callback);
//  wifi_set_channel(curchn);
//  wifi_promiscuous_enable(1);
//  chn_activity_reset(1);
//  
//  while (1)
//  {
//    if (btnid == 4) {break;}
//
//    curTime = millis();
//    if(curTime - prevTime >= 1000){
//      prevTime = curTime;
//
//      val[curchn] = deauths;
//      if (highval[curchn] > val[curchn]) { highval[curchn] = deauths; }
//      chn_deauth_detect_display();
//
//      if (curchn < maxCh) { curchn++; }
//      else if (curchn == maxCh) { curchn = 1;}
//      wifi_set_channel(curchn);
//
//      //reset counters
//      deauths = 0;
//      pkts = 0;
//    }
//    delay(100);
//  }
//  wifi_promiscuous_enable(0);
//  appmode=0;
//  btncounter++;
//}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "string.h"

/*
 * This is the (currently unofficial) 802.11 raw frame TX API,
 * defined in esp32-wifi-lib's libnet80211.a/ieee80211_output.o
 *
 * This declaration is all you need for using esp_wifi_80211_tx in your own application.
 */
//esp_err_t esp_wifi_80211_tx(wifi_interface_t ifx, const void *buffer, int len, bool en_sys_seq);
extern "C" {
  esp_err_t esp_wifi_80211_tx(wifi_interface_t ifx, const void *buffer, int len, bool en_sys_seq);
}


uint8_t beacon_raw[] = {
  0x80, 0x00,             // 0-1: Frame Control
  0x00, 0x00,             // 2-3: Duration
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff,       // 4-9: Destination address (broadcast)
  0xba, 0xde, 0xaf, 0xfe, 0x00, 0x06,       // 10-15: Source address
  0xba, 0xde, 0xaf, 0xfe, 0x00, 0x06,       // 16-21: BSSID
  0x00, 0x00,             // 22-23: Sequence / fragment number
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,     // 24-31: Timestamp (GETS OVERWRITTEN TO 0 BY HARDWARE)
  0x64, 0x00,             // 32-33: Beacon interval
  0x31, 0x04,             // 34-35: Capability info
  0x00, 0x00, /* FILL CONTENT HERE */       // 36-38: SSID parameter set, 0x00:length:content
  0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x0c, 0x12, 0x18, 0x24, // 39-48: Supported rates
  0x03, 0x01, 0x01,           // 49-51: DS Parameter set, current channel 1 (= 0x01),
  0x05, 0x04, 0x01, 0x02, 0x00, 0x00,       // 52-57: Traffic Indication Map
  
};

char *rick_ssids[] = {
  "01 Never gonna give you up",
  "02 Never gonna let you down",
  "03 Never gonna run around",
  "04 and desert you",
  "05 Never gonna make you cry",
  "06 Never gonna say goodbye",
  "07 Never gonna tell a lie",
  "08 and hurt you"
};

#define BEACON_SSID_OFFSET 38
#define SRCADDR_OFFSET 10
#define BSSID_OFFSET 16
#define SEQNUM_OFFSET 22
#define TOTAL_LINES (sizeof(rick_ssids) / sizeof(char *))

void spam_task(void *pvParameter) {
  uint8_t line = 0;

  // Keep track of beacon sequence numbers on a per-songline-basis
  uint16_t seqnum[TOTAL_LINES] = { 0 };

  for (;;) {
    vTaskDelay(100 / TOTAL_LINES / portTICK_PERIOD_MS);

    // Insert line of Rick Astley's "Never Gonna Give You Up" into beacon packet
    printf("%i %i %s\r\n", strlen(rick_ssids[line]), TOTAL_LINES, rick_ssids[line]);

    uint8_t beacon_rick[200];
    memcpy(beacon_rick, beacon_raw, BEACON_SSID_OFFSET - 1);
    beacon_rick[BEACON_SSID_OFFSET - 1] = strlen(rick_ssids[line]);
    memcpy(&beacon_rick[BEACON_SSID_OFFSET], rick_ssids[line], strlen(rick_ssids[line]));
    memcpy(&beacon_rick[BEACON_SSID_OFFSET + strlen(rick_ssids[line])], &beacon_raw[BEACON_SSID_OFFSET], sizeof(beacon_raw) - BEACON_SSID_OFFSET);

    // Last byte of source address / BSSID will be line number - emulate multiple APs broadcasting one song line each
    beacon_rick[SRCADDR_OFFSET + 5] = line;
    beacon_rick[BSSID_OFFSET + 5] = line;

    // Update sequence number
    beacon_rick[SEQNUM_OFFSET] = (seqnum[line] & 0x0f) << 4;
    beacon_rick[SEQNUM_OFFSET + 1] = (seqnum[line] & 0xff0) >> 4;
    seqnum[line]++;
    if (seqnum[line] > 0xfff)
      seqnum[line] = 0;

    esp_wifi_80211_tx(WIFI_IF_AP, beacon_rick, sizeof(beacon_raw) + strlen(rick_ssids[line]), false);

    if (++line >= TOTAL_LINES)
      line = 0;
  }
}

void RickRollSSID(){
  nvs_flash_init();
  tcpip_adapter_init();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

  //ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));

  // Init dummy AP to specify a channel and get WiFi hardware into
  // a mode where we can send the actual fake beacon frames.
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
//  wifi_config_t ap_config = {
//    .ap = {
//      .ssid = "esp32-beaconspam",
//      .ssid_len = 0,
//      .password = "dummypassword",
//      .channel = 1,
//      .authmode = WIFI_AUTH_WPA2_PSK,
//      .ssid_hidden = 1,
//      .max_connection = 4,
//      .beacon_interval = 60000
//    }
//  };
   wifi_config_t ap_config = {};
   strcpy((char*)ap_config.ap.ssid, "esp32-beaconspam");
   ap_config.ap.ssid_len = 0;
   strcpy((char*)ap_config.ap.password, "password");
   ap_config.ap.channel = 6;
   strcpy((char*)ap_config.ap.authmode, "WIFI_AUTH_WPA2_PSK");
   ap_config.ap.ssid_hidden = 0;
   ap_config.ap.max_connection = 4;
   ap_config.ap.beacon_interval = 60000;

  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));
  ESP_ERROR_CHECK(esp_wifi_start());
  ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));

  xTaskCreate(&spam_task, "spam_task", 2048, NULL, 5, NULL); //crash

//  uint8_t line = 0;
//  // Keep track of beacon sequence numbers on a per-songline-basis
//  uint16_t seqnum[TOTAL_LINES] = { 0 };
//  while (1) {
//    //vTaskDelay(100 / TOTAL_LINES / portTICK_PERIOD_MS);
//    delay(50);
//
//    // Insert line of Rick Astley's "Never Gonna Give You Up" into beacon packet
//    printf("%i %i %s\r\n", strlen(rick_ssids[line]), TOTAL_LINES, rick_ssids[line]);
//
//    uint8_t beacon_rick[200];
//    memcpy(beacon_rick, beacon_raw, BEACON_SSID_OFFSET - 1);
//    beacon_rick[BEACON_SSID_OFFSET - 1] = strlen(rick_ssids[line]);
//    memcpy(&beacon_rick[BEACON_SSID_OFFSET], rick_ssids[line], strlen(rick_ssids[line]));
//    memcpy(&beacon_rick[BEACON_SSID_OFFSET + strlen(rick_ssids[line])], &beacon_raw[BEACON_SSID_OFFSET], sizeof(beacon_raw) - BEACON_SSID_OFFSET);
//
//    // Last byte of source address / BSSID will be line number - emulate multiple APs broadcasting one song line each
//    beacon_rick[SRCADDR_OFFSET + 5] = line;
//    beacon_rick[BSSID_OFFSET + 5] = line;
//
//    // Update sequence number
//    beacon_rick[SEQNUM_OFFSET] = (seqnum[line] & 0x0f) << 4;
//    beacon_rick[SEQNUM_OFFSET + 1] = (seqnum[line] & 0xff0) >> 4;
//    seqnum[line]++;
//    if (seqnum[line] > 0xfff)
//      seqnum[line] = 0;
//
//    esp_wifi_80211_tx(WIFI_IF_AP, beacon_rick, sizeof(beacon_raw) + strlen(rick_ssids[line]), false);
//
//    if (++line >= TOTAL_LINES)
//      line = 0;
//  }

}



