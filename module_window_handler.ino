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

    uint16_t borderColor, bgColor, text_color; // Colors 
};



//have the windows treated as objects this time instead of just functions. makes referencing much more reliable lol

class Window {
public:
    std::string name;  // Window's name
    WindowCfg config;
    std::string content;

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
        tft.setTextSize(1); //we will need to be able to draw multiple strings to this but whatever the fuck

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
    void drawText(const char* text) //draw da text
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




//just erase the windows. probably would be cooler if it maintained some kind of background image but then we would have to just redraw THAT segment


//needs to work with object
void clear_window(Window win) {
    // Clear the window area
    tft.fillRect(win.x, win.y, win.width, win.height, BLACK);
    //Serial.println("Window cleared");
}
//change to take color var


//variables used in this function to drawscreen
// Global variables for time access
extern int currentHour;
extern int currentMinute;
extern int currentSecond;
extern float temperature;
extern int AVG_HR;

void drawcoverscreen(){
  // Print temperature
  tft.setCursor(0, 0);
  tft.setTextSize(1); // Set text size for temperature
  tft.setTextColor(0x558F); 
  tft.printf("%.0fC\n", temperature); // print temperature from a value rather than checking the sensor every frame
  
  // Print time
  tft.setCursor(14, 34); //why is it xy from the top left instead of left bottom. x= how far to right. y= how far down
  tft.setTextSize(3); 
  tft.setTextColor(0x07FF); 
  tft.printf("%02d:%02d", currentHour, currentMinute); // Print hours and minutes
  tft.setTextSize(1);
  tft.setCursor(101, 48);
  tft.printf(":%02d", currentSecond);  // Print seconds small
  

//print heart rate, bottom right

  // Debug framerate display
  //tft.setCursor(0, 64); 
 // tft.setTextSize(1); 
  //tft.setTextColor(RED); // Set text color for frame time
  // tft.printf("T:%lums", frameTime); // Commented out for now

  tft.setCursor(0, 120);
  //tft.setTextSize(1); 
  tft.setTextColor(0xb000); 
tft.printf("%d", AVG_HR); //print heart rate
}




void clearscreen(){
    // Calculate the time taken since the last update
 // frameTime = currentMillis - previousMillis;
 // previousMillis = currentMillis; // Update the last time

  // Clear the screen
  tft.fillScreen(BLACK);
  
//this is awful
}



#endif
