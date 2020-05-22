#include <Adafruit_NeoPixel.h>

#define PIXEL_PIN 6  // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT 8  // Number of NeoPixels

Adafruit_NeoPixel pixels(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

int hapticPin = 2;
int bendPin = A1;

bool vibrating = false;
bool audioPlayed = false;
bool lightsOn = false;
bool messageReceived = false;

int bendVal = 0;
int bendThreshold = 340;

int savingCounter = 0;

int redValue = 0;
int greenValue = 0;
int blueValue = 0;
int tempRed = 0;
int tempGreen = 0;
int tempBlue = 0;
int arr[] = {redValue, greenValue, blueValue};

void setup() {
  Serial.begin(115200);
  pixels.begin();
  pinMode(hapticPin, OUTPUT);
  pinMode(bendPin, INPUT);
  lightUpAllLights(pixels.Color(0, 0, 0), 0);
}
void loop() {
  
  if(Serial.available()) {
    reset();
    messageReceived = true;
    audioPlayed = false;

    for(int i = 0; i < 3; i++) {
      arr[i] = Serial.read();
    }
  
    redValue = arr[0];
    greenValue = arr[1];
    blueValue = arr[2];
  }

  bendVal = analogRead(bendPin);
  if(messageReceived) {
    if(bendVal < bendThreshold) {
      audioPlayed = true;
      messageReceived = false;
      setColor(redValue, greenValue, blueValue);
      runHapticFeedback();
      
      while(bendVal < bendThreshold) {
        bendVal = analogRead(bendPin);
        // just delaying the code until audio is implemented into the code.
      }
      
      runHapticFeedback();
      setColor(0,0,0);
      reset();
    }
  } else {
    if(bendVal < bendThreshold){
      Record();
    }
  }
  
  if(!(arr[0] == 0 && arr[1] == 0 && arr[2] == 0) && !audioPlayed) {
    int iter = 0;
    int iterEnd = 50;
    int totalPulseTime = 1500;
    int iterWaitTime = 30;

    int fadeRed = (redValue/iterEnd)*2;
    int fadeGreen = (greenValue/iterEnd)*2;
    int fadeBlue = (blueValue/iterEnd)*2;
      
    while(iter < iterEnd) {

      tempRed = tempRed + fadeRed;
      tempGreen = tempGreen + fadeGreen;
      tempBlue = tempBlue + fadeBlue;

      int red = tempRed;
      int green = tempGreen;
      int blue = tempBlue;

      if(tempRed <= 0 || tempRed >= redValue) {
        if(tempRed >= 255) {
          red = 255/2;
        } else if(tempRed >= redValue) {
          red = tempRed/2;
        }
        fadeRed = -fadeRed;
      }
      
      if(tempGreen <= 0 || tempGreen >= greenValue) {
        if(tempGreen >= 255) {
          green = 255/2;
        } else if(tempGreen >= greenValue) {
          green = tempGreen/2;
        }
        fadeGreen = -fadeGreen;
      }
      
      if(tempBlue <= 0 || tempBlue >= blueValue) {
        if(tempBlue >= 255) {
          blue = 255/2;
        } else if(tempBlue >= blueValue) {
          blue = tempBlue/2;
        }
        fadeBlue = -fadeBlue;
      }

      setColor(red, green, blue);
      
      delay(iterWaitTime);
      iter++;
    }
  }
}

void Record(){
  lightsOn = false;
  Serial.println(1);
  
  hapticPin = true;
  
  while(bendVal < bendThreshold) {
    bendVal = analogRead(bendPin); //Read and save analog value from potentiometer
    digitalWrite(hapticPin, HIGH);
    delay(40);
    digitalWrite(hapticPin, LOW);

    if(!lightsOn){
      lightUpAllLights(pixels.Color(255, 0, 0), 100); 
    } else {
      lightUpAllLights(pixels.Color(0, 0, 0), 50);  
    }

    lightsOn = !lightsOn;   
    delay(500);
  }
  lightUpAllLights(pixels.Color(0, 0, 0), 0);  
  StopRecord();
}

void StopRecord(){
  lightsOn = false;

  digitalWrite(hapticPin, LOW);
  
  savingCounter = 5;
  while(savingCounter > 0){
    if(!lightsOn) {
      lightUpAllLights(pixels.Color(255, 150, 0), 50); 
    } else {
      lightUpAllLights(pixels.Color(0, 0, 0), 25);  
    }
    lightsOn = !lightsOn;   
    delay(250);
    savingCounter--;
  }

  lightUpAllLights(pixels.Color(0, 0, 0), 0);  
  lightUpAllLights(pixels.Color(0, 255, 0), 50);
  delay(500);
  lightUpAllLights(pixels.Color(0, 0, 0), 50);  
  delay(500);
  lightUpAllLights(pixels.Color(0, 255, 0), 50);  
  delay(500);
  lightUpAllLights(pixels.Color(0, 0, 0), 50);  
  
  Serial.println(0);
}

void runHapticFeedback() {
  digitalWrite(hapticPin, HIGH);
  delay(100);
  digitalWrite(hapticPin, LOW);
}

void setColor(int red, int green, int blue) {
  pixels.clear();
  for(int i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, pixels.Color(red, green, blue));
  }
  pixels.show();
}

void lightUpAllLights(uint32_t color, int wait){
  pixels.clear();
  for(int i=0; i<pixels.numPixels(); i++) { // For each pixel in strip...
    pixels.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    pixels.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void reset() {
    redValue = 0;
    greenValue = 0;
    blueValue = 0;
    tempRed = 0;
    tempGreen = 0;
    tempBlue = 0;
    setColor(redValue,greenValue, blueValue);
}

// TUVA, put your code in this here function
void selectColor() {
  // add color values here, Red at index 0, Green at index 1, and Blue at index 2.
  int color[] = {};

  // Your code

  sendMessage(color);
}

// MARIE, put your code in this here function.
// the client will need to be fixed as well before this works.
void sendMessage(color) {
  // your code
  Serial.println(color);
}