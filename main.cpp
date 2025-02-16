#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Define pins
#define TEMP_SENSOR_PIN A1        //for temperature sensor 
#define HUMIDITY_PIN A0        // Potentiometer for humidity
#define SOIL_MOISTURE_PIN A2     // for soil moisture sensor 
#define PIR_SENSOR_PIN 2         // for PIR sensor 
#define BUZZER_PIN 4             // for Piezo buzzer 
#define LIGHT_SENSOR_PIN A3      // for photoresistor 
#define LED_PIN1 7              // LED connected to simulate grow light
#define LED_PIN2 10               //LED connected to potentiometer
#define DC_MOTOR_PIN1 8          // Motor driver pin 1
#define SERVO1_PIN 5             // Ventilation servo 1
#define SERVO2_PIN 6           // Ventilation servo 2


// creating obejects
LiquidCrystal_I2C lcd(0x27, 16, 2); 
Servo vent1;                       
Servo vent2;                        

void setup() {
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  
  // Initialize pins
  pinMode(TEMP_SENSOR_PIN, INPUT);
  pinMode(HUMIDITY_PIN, INPUT);
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(PIR_SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  pinMode(LED_PIN1, OUTPUT);
  pinMode(DC_MOTOR_PIN1, OUTPUT);

  
  // Attach servos
  vent1.attach(SERVO1_PIN);
  vent2.attach(SERVO2_PIN);
  vent1.write(90); // Initial position of the ventilations
  vent2.write(90); 

  //for displaying welcome message
  lcd.setCursor(0, 0);
  lcd.print("Smart Greenhouse");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read temperature
  int tempValue = analogRead(TEMP_SENSOR_PIN);
  float temperature = tempValue / 10.0; // Converting to temperature value 

  // Read humidity from potentiometer
  int humValue = analogRead(HUMIDITY_PIN);
  float humidity = map(humValue, 0, 1023, 0, 100); // Converting to percentage

  // Read soil moisture
  int soilMoisture = analogRead(SOIL_MOISTURE_PIN);

  // Read light level
  int lightLevel = analogRead(LIGHT_SENSOR_PIN);
 

  // Read PIR sensor
  int pirState = digitalRead(PIR_SENSOR_PIN);

  // Motion detection
  if (pirState == HIGH) {
    digitalWrite(BUZZER_PIN, HIGH); //when motion is detected
    delay(1000); // Buzzer ON duration
    digitalWrite(BUZZER_PIN, LOW);
  }

  // Grow light control 
  if (lightLevel < 500) { // Low light condition
    digitalWrite(LED_PIN1, HIGH); // Turn on led (grow light)
  } else {
    digitalWrite(LED_PIN1, LOW); // Turn off led (grow light)
  }
  
  // Humidity Control
  if (humidity < 50) { // Low humidity condition
    analogWrite(LED_PIN2, 255); // Turn on led(fogger) 
  } else {
    analogWrite(LED_PIN2, 0); // Turn off led(fogger) 
  }
    
  // Ventilation control
  if (temperature > 30.0) { // High temperature threshold
    vent1.write(90); // Open vent 1
    vent2.write(90); // Open vent 2
  } else {
    vent1.write(0); // Close vent 1
    vent2.write(0); // Close vent 2
  }

  // Water pump control
  if (soilMoisture < 400) { // Low soil moisture threshold
    digitalWrite(DC_MOTOR_PIN1, HIGH); // when the soil moiture is low the motor will pump water
    
  } else {
    digitalWrite(DC_MOTOR_PIN1, LOW);
   
  }

  // Displaying values of temperature and humidity on lcd
   
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(temperature);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Hum:");
  lcd.print(humidity);
  lcd.print("%");

  delay(2000); //  2 seconds delay before updating
  lcd.clear();
}

