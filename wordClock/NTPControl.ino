#define NTP_PACKET_SIZE 48
#define UDP_PORT  2390
#define UTC_EEPROM_ADDR 3

WiFiUDP udp;
byte packetBuffer[NTP_PACKET_SIZE];
IPAddress timeServerIP;
byte utc_offset =  0;

void initNTP() {
  udp.begin(UDP_PORT);
  Serial.print("UDP Local port: "); Serial.println(udp.localPort()); 
  Serial.print("Requesting Time from "); Serial.print(TIMESERVER); Serial.print(" every "); Serial.print(INTERVAL/1000); Serial.println(" seconds.");
  loadUTCOffset();
}

void getCurrentTime() {
  WiFi.hostByName(TIMESERVER, timeServerIP);
  
  int i = 0;
  while (i++ <= 5) { //Using UDP has no inherent retry, we will implement one.
    if (tryToRequestTime(timeServerIP)) break;
  }
}

bool tryToRequestTime(IPAddress& address) {
  sendNTPpacket(timeServerIP);

  int cb = 0; int i = 0;
  while (!cb && i++ < 30)
  {
    cb = udp.parsePacket();
    delay(100);
  }

  if (cb > 0) {
    udp.read(packetBuffer, NTP_PACKET_SIZE);
    setHoursMinutes(calcSecsSince1900());
    return true;
  }
  else return false;
}

unsigned long calcSecsSince1900() {
  //The response contains the seconds that have passed since 01.01.1900 in 40..43
  unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
  unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);

  return highWord << 16 | lowWord;
}

void setHoursMinutes(unsigned long epoch) {
  const long day = 86400L;
  hours = (epoch  % day) / 3600 + utc_offset; //UTC_OFFSET is applied because NTP returns UTC time
  minutes = (epoch  % 3600) / 60;
  Serial.print("Current Time is "); Serial.print(hours); Serial.print(":"); if (minutes < 10) Serial.print("0"); Serial.println(minutes); 
}

void setUTCOffset(byte offset) { 
  utc_offset = offset; 
}

void saveUTCOffset() {
  EEPROM.write(UTC_EEPROM_ADDR, utc_offset);
  EEPROM.commit();
  Serial.println("Saved UTC Offset to EEPROM: "); Serial.println(utc_offset);
}

void loadUTCOffset() {
  utc_offset = EEPROM.read(UTC_EEPROM_ADDR);
  Serial.print("Loaded UTC Offset from EEPROM: "); Serial.println(utc_offset);
}

void sendNTPpacket(IPAddress& address) {
  //Ugly packet hacking...
  Serial.print("sending NTP packet...");
  memset(packetBuffer, 0, NTP_PACKET_SIZE);

  packetBuffer[0] = 0b11100011;   
  packetBuffer[1] = 0;     
  packetBuffer[2] = 6;     
  packetBuffer[3] = 0xEC;  
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  udp.beginPacket(address, 123); 
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
  Serial.println("Done!");
}
