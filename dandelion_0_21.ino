#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include "esp_system.h"
#include "esp_pm.h"
#include "esp_wifi.h"
#include "esp_bt.h"
#include "esp_sleep.h"
#include "esp_system.h" // For device ID
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "HardwareSerial.h" 
#include <Wire.h>
#include <time.h>
#include <stdio.h>
#include <SPI.h>  // Include the SPI library
#define SPI_CLK_SPEED 56000000 // 56 MHz. very high! may increase power use


// Pin definitions for spi
#define SCLK_PIN 18
#define MOSI_PIN 23
#define MISO_PIN  19 
#define DC_PIN   12
#define CS_PIN   13
#define RST_PIN  14





#include <Preferences.h> // Library for handling NVS non volitile storage
Preferences preferences;

//wireless
#include <WiFi.h>
#include "esp_bt.h"
//#include "esp_bt_main.h"   // Required for esp_bluedroid functions
#include "esp_bt_device.h" // Optional if you need Bluetooth device functions

//sensors
#include <SparkFunTMP102.h>

//heartrate
#include "MAX30105.h"
#include "heartRate.h"
#include "spo2_algorithm.h"//aw lawd
MAX30105 particleSensor;

///////gyro imu thing
//#include "quaternionFilters.h"
#include "FastIMU.h"
#include "mdl_accelerometer.ino"
#include "Madgwick.h"
#define IMU_ADDRESS 0x68    //0x68 is the imu adress, if it needs to be fixed do that later
//#define PERFORM_CALIBRATION //Comment to disable startup calibration
MPU9250 IMU;               //Change to the name of any supported IMU!
calData calib = { 0 };  //Calibration data
AccelData IMUAccel;    //Sensor data
GyroData IMUGyro;
MagData IMUMag;
Madgwick filter;



#include <Adafruit_Sensor.h>


//wireles comunication
#include <RadioLib.h>

/*
// CC1101 has the following connections:
// CS pin:    10
// GDO0 pin:  2
// RST pin:   unused
// GDO2 pin:  3 (optional)

CC1101 radio1 = new Module(10, 2, RADIOLIB_NC, 3); //uncomment this line for config purposes
*/


//define pins
#define SDA_PIN 27
#define SCL_PIN 26

// Include basic functions
#include "module_math_render_base.ino"
#include "module_window_handler.ino"
#include "mdl_pwr_mngr.ino"
#include "mdl_timeKeeping.ino"  //the clock is ticking
#include "mdl_diagnosticTools.ino"




//hardware. declare sensors and set settings
#include <FatFs.h> //hardware library used in a lot of things
FatFs fs;


#include "mdl_temp.h"
// Create an instance of the TMP102 class from SparkFun library
TMP102 sensor0;




#define I2Cclock 400000
//math functions misc i guess
#include <vector>
#include <stdint.h> // For int8_t
#define uS_TO_S_FACTOR 1000000ULL /* Conversion factor for micro seconds to seconds */
 int currentHour = 0;
 int currentMinute = 0;
 int currentSecond = 0;
 float temperature = 69;





//less important stuff 
extern unsigned long previousMillis = 0; // Stores the last time the frame time was updated
extern unsigned long frameTime = 0;      // Variable to hold the frame time


//declare the task functions for multicore programs
void taskDrawScreen(void *pvParameters); //render the screen using known variables
void taskUpdateSensors(void *pvParameters); //update the sensors 
void taskUpdateHeartRate(void *pvParameters);









// Setup!!!
void setup() {


//config wifi and bluetooth to be off
  WiFi.mode(WIFI_OFF);  // Turn off Wi-Fi
  WiFi.disconnect(true); // Ensure it stays off and disconnected
  //  esp_bluedroid_disable(); // Disable Bluetooth stack
 // esp_bluedroid_deinit();  // De-initialize Bluetooth stack
  btStop();                // Turn off Bluetooth
  

  // Initialize SPI
  SPI.begin();
  SPI.begin(SCLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
  SPI.beginTransaction(SPISettings(40000000, MSBFIRST, SPI_MODE0));  // 40 MHz SPI clock speed
SPI.endTransaction();


 Serial.begin(115200); 
  Wire.begin(SDA_PIN, SCL_PIN); // Initialize I2C with specified pins
  // Start display
 screen_on();  // Call to initialize the screen with proper SPI settings





   if (!fs.begin(CS_PIN)) {  // Use the CS pin defined (e.g., pin 13)
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");

    // Open the root directory
  File dir = fs.open("/");  // Open the root directory
  if (dir) {
    // Iterate through all files in the directory
    while (true) {
      File file = dir.openNextFile();  // Get the next file
      if (!file) {
        break;  // Break the loop if no more files
      }
      Serial.print("File: ");
      Serial.println(file.name());  // Print the file name
      file.close();  // Close the file
    }
    dir.close();  // Close the directory
  } else {
    Serial.println("Error opening root directory.");
  }


//run a little scan of the files, take this out later




  delay(1);
  Wire.begin(SDA_PIN, SCL_PIN); // Initialize I2C
  screen_on();
  screen_startup(); // Start the screen up
  tft.fillScreen(BLACK); // Erase it
  set_orientation(0);

  // Print debug devices identified to this watch with a status as OK or not
  tft.setTextSize(1); 
  tft.setTextColor(PEACH);

  // void deviceCheck();
  tft.setCursor(0, 0); 
   tft.setTextSize(1); 
  tft.printf("DANDELION OS");
    tft.setCursor(0, 64);
      tft.setTextSize(1); 
  tft.printf("ATERNUM TECHNOLOGIES");


    // Initialize RTC with stored or default time
    initializeRTC();
  
    // Print the current local time
    printLocalTime();


//initialize stupid sensors here
HRsensorSetup();

//stupid temp sensor dbg
  // Initialize TMP102 sensor
  if (!sensor0.begin()) {
    Serial.println("Failed to initialize temperature sensor.");
    while (1);  // Halt execution if the sensor fails to initialize
  }



 delay(800);
  tft.fillScreen(BLACK);

  tft.setTextColor(WHITE);
  scanI2C();
  delay(1);
  scanSPI();
  delay(800);
  tft.fillScreen(BLACK);


//preboot phase done, move to standard tasks

 // Create task for drawing to the screen (Core 0)
  xTaskCreatePinnedToCore(taskDrawScreen,"Draw Screen",10000,NULL,1,NULL,0);

  // Create task for updating sensors and clock (Core 1)
  xTaskCreatePinnedToCore(
    taskUpdateSensors, // Function to be called (updating sensors)
    "Update Sensors",  // Name of the task (for debugging)
    10000,             // Stack size (words)
    NULL,              // Parameters (none)
    1,                 // Priority (1)
    NULL,              // Task handle (none)
    1                  // Run on Core 1
  );





xTaskCreatePinnedToCore(taskUpdateHeartRate,"HRSENSOR",2048,NULL,1,NULL,1);






//setup ends here
}


void loop() {
  //nothing for now
}

// Task for drawing to the screen (Core 0)
void taskDrawScreen(void *pvParameters) {
  (void)pvParameters; // Avoid unused parameter warning

  while (true) {
    // Clear screen and draw content
    clearscreen();  // Clear the screen
    drawcoverscreen();  // Draw the updated screen content

    vTaskDelay(100 / portTICK_PERIOD_MS);  // Delay for 100ms before next update
  }
}

// Task for updating sensors and clock (Core 1)
void taskUpdateSensors(void *pvParameters) {
  (void)pvParameters; // Avoid unused parameter warning

  while (true) {
    // Update sensor data
    sensor0.wakeup();
    temperature = sensor0.readTempC();  // Read temperature in Celsius
    sensor0.sleep();  // Put sensor back to sleep

    // Update time
    updateStoredTime();  // Function to update time (currentHour, currentMinute, currentSecond)
   updateheartrate();
    vTaskDelay(500 / portTICK_PERIOD_MS);  // Delay for 500ms before next update
  }
}

// Task for updating heart rate
void taskUpdateHeartRate(void *pvParameters) {
  (void)pvParameters;  // Avoid unused parameter warning

  while (true) {
    // Update sensor data
    updateheartrate(); // Make sure this function is defined elsewhere
    vTaskDelay(10 / portTICK_PERIOD_MS);  // Delay for 10ms before next update. this is stupidly fast
  }
}
