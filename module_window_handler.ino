// module_window_handler.h

#ifndef MODULE_WINDOW_HANDLER_H
#define MODULE_WINDOW_HANDLER_H


//todo: automatically store a registry of all known windows
//need to remove it from the list and from collection because it doesn't exist
//we'll also have to use a destructor to clean this object up!



// Window structure to hold window properties
struct WindowCfg {
    int x, y;          // Position
    int width, height; // Size
    bool auto_alignment=0,wrap=1; // Align text centrally or not,wrap text
    int textsize=1; //default text size inside this window
    uint16_t borderColor, bgColor, text_color; // Colors 
};



//have the windows treated as objects this time instead of just functions. makes referencing much more reliable lol

class Window {
public:
    std::string name;  // Window's name
    WindowCfg config; //take config
    std::string content; //take string off this


    // Constructor
    Window(const std::string& windowName, const WindowCfg& cfg, const std::string& initialContent = "")
        : name(windowName), config(cfg), content(initialContent) {}

    // Draws the window with its content
    void draw() {
        // Clear the window interior
        tft.fillRect(config.x, config.y, config.width, config.height, config.bgColor);

        // Draw the window outline
        tft.drawRect(config.x, config.y, config.width, config.height, config.borderColor);

        // Set text properties
        tft.setTextColor(config.text_color);
        tft.setTextSize(config.textsize); //we will need to be able to draw multiple strings to this. I'd really like dynamic size set and multi string support

        // Render content as word-wrapped text
        drawText(content.c_str());
    }


//clear the window
void clear() {
    tft.fillRect(config.x, config.y, config.width, config.height, config.bgColor);
}





    // Updates the window content and redraws it
    void updateContent(const std::string& newContent) {
        content = newContent;
        draw(); // Re-draw with updated content
    }



private:
    void drawText(const char* text) //draw da text. this should be scaled to text size but i'll add that later
    { 
        int charWidth = 6;   // Approximate width of each character 
        int charHeight = 8;  // Height of each character

        // Break text into words
        String strText = String(text);
        int cursorX = config.x + 2; // Padding
        int cursorY = config.y + 2;

        int wordStart = 0;
        for (int i = 0; i <= strText.length(); i++) {
            if (strText[i] == ' ' || strText[i] == '\0') {
                String word = strText.substring(wordStart, i);
                int wordWidth = word.length() * charWidth;

                // Check if the word fits in the current line
                if (cursorX + wordWidth > config.x + config.width - 2) {
                    // Move to the next line
                    cursorX = config.x + 2;
                    cursorY += charHeight;

                    // Check if there's room for another line
                    if (cursorY + charHeight > config.y + config.height - 2) {
                        // No more room, text overflows
                        break;
                    }
                }

                // Print the word
                tft.setCursor(cursorX, cursorY);
                tft.print(word);

                // Move the cursor to the end of the word
                cursorX += wordWidth + charWidth; // Add space between words

                // Update wordStart to the next word
                wordStart = i + 1;
            }
        }

        Serial.println("Window drawn with text");
    }
};

//variables used in this function to drawscreen
// Global variables for time access
extern int currentHour;
extern int currentMinute;
extern int currentSecond;
extern float temperature;
extern int AVG_HR;

//define a little window group we can set for the lock screen
//quick,dirty,effective
#define defaultWinGroup_lockscreen \
// Predefine windows for the lock screen
Window timeWindow("Time", WindowCfg{14, 34, 100, 40, false, true, 2, 0xFFFF, 0x0000, 0x07FF}, "00:00");
Window tempWindow("Temperature", WindowCfg{10, 0, 100, 30, false, true, 1, 0xFFFF, 0x0000, 0x558F}, "23C");
Window heartRateWindow("Heart Rate", WindowCfg{100, 120, 50, 30, false, true, 1, 0xFFFF, 0x0000, 0xB000}, "72");

void updateLockScreen() {
    heartRateWindow.updateContent(std::to_string(AVG_HR)); // Update heart rate window
    tempWindow.updateContent(std::to_string(temperature)); // Update temperature window

    // Format time as "hh:mm:ss" byconvert time into hhmmss
    std::string timeString = 
        (currentHour < 10 ? "0" : "") + std::to_string(currentHour) + ":" +  // Ensure 2 digits for hour
        (currentMinute < 10 ? "0" : "") + std::to_string(currentMinute) + ":" +  // Ensure 2 digits for minute
        (currentSecond < 10 ? "0" : "") + std::to_string(currentSecond);  // Ensure 2 digits for second
    timeWindow.updateContent(timeString); // Update time window
}


/*
void updateLockscreenTime(int hour, int minute, int second) {
    std::string timeStr = (hour < 10 ? "0" : "") + std::to_string(hour) + ":" + (minute < 10 ? "0" : "") + std::to_string(minute);
    timeWindow.updateContent(timeStr); // Update time window

    std::string secondStr = (second < 10 ? "0" : "") + std::to_string(second);
    heartRateWindow.updateContent(secondStr); // Update heart rate window (using seconds for simplicity)
}

void updateLockscreenTemperature(float temperature) {
    tempWindow.updateContent(std::to_string((int)temperature) + "C"); // Update temperature window
}
*/





#endif
