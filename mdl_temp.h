#ifndef mdl_temp_H
#define mdl_temp_H

//NO USEFUL CODE HERE. LIBRARY TOO SMALL FOR ME TO GIVE A SHIT SO IT'S DONE IN MAIN

/*
{
  private:
    TMP102 sensor0;  // Create an instance of the SparkFun TMP102 class
    float temperature;
};


class TMP102 {
  public:
    TMP102() {}
    
    bool initSensor() {
      Wire.begin(); // Join I2C Bus

      // Initialize sensor (returns true on success, false on failure)
      if (!sensor0.begin()) {
        Serial.println("Cannot connect to TMP102.");
        return false;
      }
      
      Serial.println("Connected to TMP102!");
      delay(100);

      // Configure the sensor settings
      sensor0.setFault(0);               // Trigger alarm immediately
      sensor0.setAlertPolarity(1);        // Active HIGH
      sensor0.setAlertMode(0);            // Comparator Mode
      sensor0.setConversionRate(2);       // 4Hz conversion rate
      sensor0.setExtendedMode(0);         // 12-bit temperature range
      sensor0.setHighTempF(85.0);         // Set T_HIGH in Fahrenheit
      sensor0.setLowTempF(84.0);          // Set T_LOW in Fahrenheit
      
      return true;
    }

    void updateTemperature() {
      // Wake up sensor and take reading
      sensor0.wakeup();
      float tempF = sensor0.readTempF();  // Read temperature in Fahrenheit
      temperature = tempF;                // Store it in the class variable
      sensor0.sleep();                    // Put sensor back to sleep
    }

    float getTemperature() {
      return temperature; // Return the last updated temperature
    }

    // Optional: A function to declare alert pin on pin 34
    /*
    void setupAlertPin() {
      pinMode(34, INPUT);  // Set pin 34 as input for alert
    }
    */




#endif
