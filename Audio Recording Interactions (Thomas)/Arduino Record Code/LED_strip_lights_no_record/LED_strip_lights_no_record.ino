#include <Adafruit_NeoPixel.h>

#define PIXEL_PIN 6  // Digital IO pin connected to the NeoPixels.

#define PIXEL_COUNT 8  // Number of NeoPixels

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

const int buttonPin = 10;

int pixelNumber;

int buttonRead;
int buttonReadTwo;
const int flexPin = A1; //pin A0 to read analog input
const int vibrate = 2;
bool vibrating = false;

bool on;

int savingCounter = 0;

bool isRecording = false;

int flexValue;
int flexGoal = 380;

void setup() {
  Serial.begin(115200);
  pinMode(flexPin,INPUT);
  pinMode(vibrate, OUTPUT);


  strip.begin(); // Initialize NeoPixel strip object (REQUIRED)
  strip.show();  // Initialize all pixels to 'off'
  strip.clear();
  lightUpAllLights(strip.Color(0, 0, 0), 0);  

}


void loop() {
   flexValue = analogRead(flexPin);         //Read and save analog value from potentiometer
  
  if(flexValue < flexGoal && !isRecording){
    isRecording = true;
    Record();
  }
}

void Record(){
  pixelNumber = 0;
  on = false;
  Serial.println(1);
  
  vibrating = true;
  
  while(flexValue < flexGoal  ){
    flexValue = analogRead(flexPin);         //Read and save analog value from potentiometer
    buttonRead = digitalRead(buttonPin);
    digitalWrite(vibrate, HIGH);
    delay(40);
    digitalWrite(vibrate, LOW);
    if(!on){
      lightUpAllLights(strip.Color(255, 0, 0), 100); 
    }else{
      lightUpAllLights(strip.Color(0, 0, 0), 50);  
    }
    on = !on;   
    delay(500);
  }
  lightUpAllLights(strip.Color(0, 0, 0), 0);  
  StopRecord();
}

void StopRecord(){
  on = false;

  digitalWrite(vibrate, LOW);
  
  savingCounter = 5;
  while(savingCounter > 0){
    if(!on){
      lightUpAllLights(strip.Color(255, 150, 0), 50); 
    }else{
      lightUpAllLights(strip.Color(0, 0, 0), 25);  
    }
    on = !on;   
    delay(250);
    savingCounter--;
  }

  lightUpAllLights(strip.Color(0, 0, 0), 0);  

  isRecording = false;
  lightUpAllLights(strip.Color(0, 255, 0), 50);
  delay(500);
  lightUpAllLights(strip.Color(0, 0, 0), 50);  
  delay(500);
  lightUpAllLights(strip.Color(0, 255, 0), 50);  
  delay(500);
  lightUpAllLights(strip.Color(0, 0, 0), 50);  
  
  Serial.println(0);
}


void lightUpAllLights(uint32_t color, int wait){
  strip.clear();
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}
