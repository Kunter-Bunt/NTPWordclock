#define REST_PORT   80
#define HOSTNAME    "wordclock"

#define WIFI_SSID_ADDR      32
#define WIFI_SSID_MAX  32
#define WIFI_PASS_ADDR      64
#define WIFI_PASS_MAX  32

ESP8266WebServer http_rest_server(REST_PORT);

void initWifi() {
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  
  Serial.print("Connecting to "); Serial.print(WLAN_SSID);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: "); Serial.println(WiFi.localIP());

  initMDNS(); //mDNS allows you to connect via hostname
  initRESTServer(); //Registers control operations
}

void initMDNS() {
  if (MDNS.begin(HOSTNAME)) {
    Serial.print("Registered to mDNS as "); Serial.print(HOSTNAME); Serial.println(".local");
    MDNS.addService("http", "tcp", REST_PORT);
  }
}

void initRESTServer() {
  //If wordclock.local cannot be reolved, use the IP instead.
  
  //wordclock.local/rgb?r=X&g=X&b=X
  http_rest_server.on("/rgb", HTTP_GET, restRGB); //Misusing GET to be able to just call these with a browser
  //wordclock.local/utc?offset=X
  http_rest_server.on("/utc", HTTP_GET, restUTC_offset);
  
  http_rest_server.begin();
}

void handleRESTServer() { http_rest_server.handleClient(); }

void restUTC_offset() {
  byte offset = http_rest_server.arg("offset").toInt(); Serial.print("offset="); Serial.println(offset);
  setUTCOffset(offset);
  saveUTCOffset();
  http_rest_server.send(200,"text/html","OK");
}

void restRGB() { 
  byte r = http_rest_server.arg("r").toInt(); Serial.print("r="); Serial.print(r); 
  byte g = http_rest_server.arg("g").toInt(); Serial.print(";g="); Serial.print(g);
  byte b = http_rest_server.arg("b").toInt(); Serial.print(";b="); Serial.println(b);
  setRGB(r, g, b);
  saveRGB();
  http_rest_server.send(200,"text/html","OK");
}

