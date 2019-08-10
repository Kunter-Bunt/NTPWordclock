#define LED_R_ADDR 0
#define LED_G_ADDR 1
#define LED_B_ADDR 2

CRGB leds[NUM_LEDS];
byte R = 0, G = 0, B = 0;

void initLEDS() {
  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(LED_VOLTS, LED_AMPS); 
  Serial.print("LEDs: "); Serial.println(NUM_LEDS);
  Serial.print("Volts: "); Serial.println(LED_VOLTS);
  Serial.print("Amps: "); Serial.println(LED_AMPS);
  loadRGB();
}

/*
 * The Implementation is based around defining functions that are called like the spoken words. 
 * Define the functions as something the uses light_leds(first_led, last_led); to light up its leds.
 * Now you can simply combine the functions to the spoken sentence, for example it(); is(); half(); past(); eight();
 */
void showTime(byte h, byte m) {
  FastLED.clear(); //Reset the lighted LEDs

  //Time conversions
  m = roundToNext5Min(m); //Word clocks usually only display in 5 minute steps
  if (minutes >= 25) h++; //After that point we talk about the coming hour, like "quarter to ten"
  Serial.print("Showing: Hour "); Serial.print(h); Serial.print(" and minute "); Serial.println(m); //Debugging output

  //LED configuration for sentence
  es(); ist(); //All western german time-sentences start with "Es ist ..."
  showMinutes(m); //Then we go on by saying how far we are in the hour
  showHours(h); //And last comes the hour

  FastLED.show(); //Light the LEDs you just configured
  Serial.println("Show!"); Serial.println();
}

void showHours(byte h) {
  switch (h) {
    case 0: case 12: zwoelfUhr(); break;
    case 1: case 13: einUhr(); break;
    case 2: case 14: zweiUhr(); break;
    case 3: case 15: dreiUhr(); break;
    case 4: case 16: vierUhr(); break;
    case 5: case 17: fuenfUhr(); break;
    case 6: case 18: sechsUhr(); break;
    case 7: case 19: siebenUhr(); break;
    case 8: case 20: achtUhr(); break;
    case 9: case 21: neunUhr(); break;
    case 10: case 22: zehnUhr(); break;
    case 11: case 23: elfUhr(); break;
    default: break;
  }
}
/*
 * Sample: 4:10 the german would speak as "Es ist zehn nach vier". 
 * "Es" and "ist" are called at the top level function (showTime).
 * "zehn" "nach" are called here.
 * "vier" is called as vierUhr(); at the showHours function.
 */
void showMinutes(byte m) {
  switch (m) {
    case 0: uhr(); break;
    case 5: fuenf(); nach(); break;
    case 10: zehn(); nach(); break; 
    case 15: viertel(); nach(); break;
    case 20: zwanzig(); nach(); break; 
    case 25: fuenf(); vor(); halb(); break; 
    case 30: halb(); break;
    case 35: fuenf(); nach(); halb(); break;
    case 40: zwanzig(); vor(); break;
    case 45: viertel(); vor(); break;
    case 50: zehn(); vor(); break;
    case 55: fuenf(); vor(); break;
    default: break;
  }
}

/*
 * These are your "words", for my layout "sechs" is in the bottom left right besides my ESP8266, "es" is at the to left, far away from the controller.
 * With these Neopixel-like LEDs you specify the number counted from the controller through the LED strip.
 * So, sit down with your layout, figure where your Controller will be and start counting from 1. 
 * Keep in mind how you will connect your rows, most likely you will have to count every second row backwards ;)
 */
void es() { light_leds(109, 110); }
void ist() { light_leds(105, 107); }

void einUhr() { light_leds(47, 49); }
void zweiUhr() { light_leds(45, 48); }
void dreiUhr() { light_leds(40, 43); }
void vierUhr() { light_leds(30, 33); }
void fuenfUhr() { light_leds(34, 37); }
void sechsUhr() { light_leds(2, 6); }
void siebenUhr() { light_leds(50, 55); }
void achtUhr() { light_leds(18, 21); }
void neunUhr() { light_leds(26, 29); }
void zehnUhr() { light_leds(14, 17); }
void elfUhr() { light_leds(23, 25); }
void zwoelfUhr() { light_leds(57, 61); }

void uhr() { light_leds(9, 11); }
void nach() { light_leds(69, 72); }
void vor() { light_leds(73, 75); }
void halb() { light_leds(63, 66); }
void zwanzig() { light_leds(93, 99); }
void viertel() { light_leds(78, 84); }
void zehn() { light_leds(89, 92); }
void fuenf() { light_leds(100, 103); }

/* 
 * This function lights all LEDs in between. 
 * Control the Color by using setRGB(byte r, byte g, byte b)
 */
void light_leds(int start, int ende) {
  Serial.print("Lighting: ");
  for (int i = start - 1; i <= ende - 1; i++){
    leds[i].setRGB(R, G, B);
    Serial.print(i); Serial.print(" "); //The debugging output will always show which LEDs shall be lit
  }
  Serial.println();
}

int roundToNext5Min(byte m) {
  int rounded = (m / 5) * 5;
  if (m % 5 > 2) rounded += 5;
  return rounded % 60;
}

void setRGB(byte r, byte g, byte b) {
    R = r;
    G = g;
    B = b;
}

void saveRGB() {
  EEPROM.write(LED_R_ADDR, R);
  EEPROM.write(LED_G_ADDR, G);
  EEPROM.write(LED_B_ADDR, B);
  EEPROM.commit();
  Serial.println("Saved RGB to EEPROM");
}

void loadRGB() {
  R = EEPROM.read(LED_R_ADDR);
  G = EEPROM.read(LED_G_ADDR);
  B = EEPROM.read(LED_B_ADDR);
  Serial.println("Loaded RGB from EEPROM");
}
