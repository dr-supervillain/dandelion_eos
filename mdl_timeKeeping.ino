#ifndef mdl_timeKeeping_H
#define mdl_timeKeeping_H



// Global variables for time access
extern int currentHour;
extern int currentMinute;
extern int currentSecond;

// Function to initialize the RTC with stored time or set a default time
void initializeRTC() {
    struct tm timeinfo;
    
    // Check if time is stored in NVS
    if (preferences.isKey("time_date") && preferences.isKey("time_hour")) {
        int64_t date = preferences.getLong("time_date");
        int64_t hour = preferences.getLong("time_hour");
        
        // Decode the date
        timeinfo.tm_year = (date / 10000) - 1900;
        timeinfo.tm_mon  = ((date / 100) % 100) - 1;
        timeinfo.tm_mday = date % 100;
        
        // Decode the hour
        timeinfo.tm_hour = hour / 10000;
        timeinfo.tm_min  = (hour / 100) % 100;
        timeinfo.tm_sec  = hour % 100;
        
        time_t t = mktime(&timeinfo);
        struct timeval now = { .tv_sec = t };
        settimeofday(&now, NULL);
        
        Serial.println("Time initialized from NVS");
    } else {
        Serial.println("No time stored, setting default time");
        
        timeinfo.tm_year = 2024 - 1900;
        timeinfo.tm_mon  = 8;
        timeinfo.tm_mday = 1;
        timeinfo.tm_hour = 12;
        timeinfo.tm_min  = 0;
        timeinfo.tm_sec  = 0;
        
        time_t t = mktime(&timeinfo);
        struct timeval now = { .tv_sec = t };
        settimeofday(&now, NULL);
    }
}

// Function to update the time in NVS and global variables
void updateStoredTime() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return;
    }
    
    // Update global time variables
    currentHour = timeinfo.tm_hour;
    currentMinute = timeinfo.tm_min;
    currentSecond = timeinfo.tm_sec;
    
    // Store the date and time in NVS
    int64_t date = (int64_t)(timeinfo.tm_year + 1900) * 10000 +
                   (int64_t)(timeinfo.tm_mon + 1) * 100 +
                   (int64_t)timeinfo.tm_mday;
    preferences.putLong("time_date", date);
    
    int64_t hour = (int64_t)timeinfo.tm_hour * 10000 +
                   (int64_t)timeinfo.tm_min * 100 +
                   (int64_t)timeinfo.tm_sec;
    preferences.putLong("time_hour", hour);
    
    Serial.println("Time updated in NVS");
}

// Function to print the time
void printLocalTime() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return;
    }

    // Update global variables again (optional)
    currentHour = timeinfo.tm_hour;
    currentMinute = timeinfo.tm_min;
    currentSecond = timeinfo.tm_sec;
    
    char dateStr[20];
    strftime(dateStr, sizeof(dateStr), "%Y/%b/%d", &timeinfo);
    Serial.print("Date: ");
    Serial.println(dateStr);
    
    char time24Str[10];
    strftime(time24Str, sizeof(time24Str), "%H:%M:%S", &timeinfo);
    Serial.print("Time (24-hour): ");
    Serial.println(time24Str);
    
    char time12Str[10];
    strftime(time12Str, sizeof(time12Str), "%I:%M:%S %p", &timeinfo);
    Serial.print("Time (12-hour): ");
    Serial.println(time12Str);
}

#endif