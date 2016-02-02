// This #include statement was automatically added by the Particle IDE.
#include "neopixel/neopixel.h"
#include "Adafruit_DHT/Adafruit_DHT.h"

// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#define DHTTYPE DHT11		// DHT 22 (AM2302)



SYSTEM_MODE(AUTOMATIC);

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D6
#define PIXEL_COUNT 25
#define PIXEL_TYPE WS2812B

int DHTPIN=5;     // what pin we're connected to
int PHOTORESISTOR=A0;
int POWER=A5;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);


void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


DHT dht(DHTPIN, DHTTYPE);

int light;
double humid;
double temp;
int ledMode;

void setup() {
    
    
    pinMode(A0,INPUT);
    pinMode(A5,OUTPUT);

    ledMode = 0;

	Serial.begin(9600); 
	Serial.println("DHTxx test!");


    // Write less than 5v to power, analog pin would burn?
    analogWrite(POWER,180);

    Particle.variable("light", light);
    Particle.variable("humid", humid);
    Particle.variable("temp", temp);
    

    // We are also going to declare a Spark.function so that we can turn the LED on and off from the cloud.
    Particle.function("led",ledControl);
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'


	dht.begin();
}

// Fill the dots one after the other with a color, wait (ms) after each one
void gridWipe(uint32_t c, uint32_t c1, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    if (i % 2 == 0 ) {
    strip.setPixelColor(i, c);
        } else {
    strip.setPixelColor(i, c1);
    }
    strip.show();
    delay(wait);
  }
}

// Fill the dots one after the other with a color, wait (ms) after each one
void colorWipe(uint32_t c,  uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}


int ledControl(String command){
    if (command  == "red") {
        ledMode = 1;   
    }  else if (command == "blue"){
        ledMode = 2;   
    } else if (command == "mix"){
        ledMode = 3;   
    } else {
        ledMode = 0;
    }
}
    

void loop() {
// Wait a few seconds between measurements.
	delay(2000);




    light = analogRead(PHOTORESISTOR);


// Reading temperature or humidity takes about 250 milliseconds!
// Sensor readings may also be up to 2 seconds 'old' (its a 
// very slow sensor)
	float h = dht.getHumidity();
// Read temperature as Celsius
	float t = dht.getTempCelcius();
// Read temperature as Farenheit
	float f = dht.getTempFarenheit();
  
    humid = h;
    temp = t;
  
// Check if any reads failed and exit early (to try again).
	if (isnan(h) || isnan(t) || isnan(f)) {
		Serial.println("Failed to read from DHT sensor!");
		return;
	}

    if ( ledMode == 0) {
        colorWipe(strip.Color(0,0,0),20);
    }  else if ( ledMode == 1 ){
        colorWipe(strip.Color(255,0,0),20);       
    } else if (ledMode == 2) {
        colorWipe(strip.Color(0,0,255),20);       
    } else if ( ledMode == 3){
        gridWipe(strip.Color(0,0,255), strip.Color(255,0,0),20);
    }	

// Compute heat index
// Must send in temp in Fahrenheit!
//	float hi = dht.getHeatIndex();
//	float dp = dht.getDewPoint();
//	float k = dht.getTempKelvin();

// 	Serial.print("Humid: "); 
// 	Serial.print(h);
// 	Serial.print("% - ");
// 	Serial.print("Temp: "); 
// 	Serial.print(t);
// 	Serial.print("*C ");
// 	Serial.print(f);
// 	Serial.print("*F ");
// 	Serial.print(k);
// 	Serial.print("*K - ");
// 	Serial.print("DewP: ");
// 	Serial.print(dp);
// 	Serial.print("*C - ");
// 	Serial.print("HeatI: ");
// 	Serial.print(hi);
// 	Serial.println("*C");
// 	Serial.println(Time.timeStr());
}


