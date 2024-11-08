// module_window_handler.h

#ifndef MODULE_WINDOW_HANDLER_H
#define MODULE_WINDOW_HANDLER_H







// Window structure to hold window properties
struct Window {
    int x;
    int y;
    int width;
    int height;
    uint16_t borderColor;
};

// Function declarations
void draw_window(Window win); //fun fact you WILL have to number each new window you make. like "win0,win1,win2" in each time you call this function
void clear_window(Window win);

//draw windows where there's just text inside for now, then when we get later on in the future we can worry about content like images and everything else in our miserable fucking lives
void draw_window(Window win, const char* text) {
    // Clear the window interior
    tft.fillRect(win.x, win.y, win.width, win.height, BLACK);

    // Draw the window outline
    tft.drawRect(win.x, win.y, win.width, win.height, win.borderColor);

    // Set text properties
    tft.setTextColor(WHITE); //TODO: EXPOSE this so we can draw in colors
    tft.setTextSize(1);

    // Calculate character width and height
    int charWidth = 6; // Approximate width of each character (font dependent)
    int charHeight = 8; // Height of each character

    // Split text into words
    String strText = String(text);
    int cursorX = win.x + 2; // Padding from window edge
    int cursorY = win.y + 2; // Padding from window edge



    // Break text into words
    int wordStart = 0;
    for (int i = 0; i <= strText.length(); i++) {
        if (strText[i] == ' ' || strText[i] == '\0') {
            String word = strText.substring(wordStart, i);
            int wordWidth = word.length() * charWidth;

            // Check if the word fits in the current line
            if (cursorX + wordWidth > win.x + win.width - 2) {
                // Move to the next line
                cursorX = win.x + 2;
                cursorY += charHeight;

                // Check if there's room for another line
                if (cursorY + charHeight > win.y + win.height - 2) {
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


//just erase the windows. probably would be cooler if it maintained some kind of background image but then we would have to just redraw THAT segment

void clear_window(Window win) {
    // Clear the window area
    tft.fillRect(win.x, win.y, win.width, win.height, BLACK);
    Serial.println("Window cleared");
}


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
  tft.printf(":%02d", currentSecond);  // Print seconds properly
  

//print heart rate, bottom right


  // Debug framerate display
  tft.setCursor(0, 64); 
  tft.setTextSize(1); 
  tft.setTextColor(RED); // Set text color for frame time
  // tft.printf("T:%lums", frameTime); // Commented out for now

  tft.setCursor(0, 120);
  tft.setTextSize(1); 
  tft.setTextColor(0xb000); 
tft.printf("%d", AVG_HR); //print heart rate
}




void clearscreen(){
    // Calculate the time taken since the last update
 // frameTime = currentMillis - previousMillis;
 // previousMillis = currentMillis; // Update the last time

  // Clear the screen
  tft.fillScreen(BLACK);

}

#endif
