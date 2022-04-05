#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI  6
#define OLED_CLK   7
#define OLED_DC    4
#define OLED_CS    3
#define OLED_RESET 5
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// Declare buzzers
const int buzzer1 = 13;
const int buzzer2 = 12;

// Declare Back Distance sensor
const int echoPinSpate = 11;
const int trigPinSpate = 10;

// Declare Front Distance sensor
const int echoPinFata = 9;
const int trigPinFata = 8;

// The durations sent-receiv operation of the sensors 
long durationSpate, durationFata;
// The calculations of the distances resulted by the durations of each signal
int distanceSpate, distanceFata;

void setup(){
  // Display stuff
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000); // Pause for 2 seconds
  display.clearDisplay();
  // Initialize display text settings
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  pinMode(buzzer1, OUTPUT);         // Set buzzer1 as output
  pinMode(buzzer2, OUTPUT);         // Set buzzer2 as output
  pinMode(trigPinSpate, OUTPUT);    // Sets the trigPin as an Output
  pinMode(echoPinSpate, INPUT);     // Sets the echoPin as an Input
  pinMode(trigPinFata, OUTPUT);     // Sets the trigPin as an Output
  pinMode(echoPinFata, INPUT);      // Sets the echoPin as an Input
  Serial.begin(9600);               // Start serial comm for debugging

}

void loop(){
  // To clean the 10us signal
  digitalWrite(trigPinSpate, LOW);
  delayMicroseconds(2);
  
  // Set the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPinSpate, HIGH);
  delayMicroseconds(10);
  
  digitalWrite(trigPinSpate, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  durationSpate = pulseIn(echoPinSpate, HIGH);
  
  // Calculating the distance
  // The speed of sound is: 340m/s
  distanceSpate = durationSpate/2 * 0.034;
  
  // Prints the distance on the Serial Monitor
  Serial.print("DistanceSpate: ");
  Serial.println(distanceSpate);

  if(distanceSpate < 30) // Beep if distance is short enough
  {
    int beepRate = distanceSpate * 5;
    Serial.print("DistanceSpate too close\n");
//    drawDistance(distanceSpate);
    beep(buzzer1, beepRate);
  }
  else
    noTone(buzzer1);
//  drawDistance();
  
  // To clean the 10us signal
  digitalWrite(trigPinFata, LOW);
  delayMicroseconds(2);

  // Set the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPinFata, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPinFata, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  durationFata = pulseIn(echoPinFata, HIGH);

  // Calculating the distance
  // The speed of sound is: 340m/s
  distanceFata = durationFata/2 * 0.034;
  
  // Prints the distance on the Serial Monitor
  Serial.print("DistanceFata: ");
  Serial.println(distanceFata);
  
  if(distanceFata < 30)
  {
    int beepRate = distanceFata * 5;
    Serial.print("DistanceFata too close\n");
//    drawDistance(distanceFata);
    beep(buzzer2, beepRate);
  }
  else
    noTone(buzzer2);  
  drawDistance();
}

// Create a beep that can have variable beep rate
void beep(const int buzzerNO, int beepRate)
{
    tone(buzzerNO, 200);  //beep
    delay(50);            //beep duration
    noTone(buzzerNO);     //pause 
    delay(beepRate);      //pause duration
}

// Draw the distance bars for each side/sensor
void drawDistance(void) {  
  display.clearDisplay();      // Normal display draw steps
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.println(distanceFata);
  display.setCursor(40, 0);
  display.println(distanceSpate);

  uint16_t _distFata, _distSpate;
  
  _distFata = distanceFata * 2;
  _distSpate = distanceSpate * 2;
  
  // For debugging
  Serial.println("\n__________DISTSPATE");
  Serial.println(_distSpate);
  // For debugging
  Serial.println("\n__________FATA");
  Serial.println(_distFata);
  
  // Arbitrary length to represent how close or far each obstacle is from its respective sensor
  display.fillRect(0, 10                  , _distFata , 5, 1);
  display.fillRect(0, SCREEN_HEIGHT/2 + 10, _distSpate, 5, 1);

  display.display();
}
