//file system reader. 

/*
features intended:

-read write create destroy files: txt,png,jpg,gif, bmp, etc [mp4decode will be an issue really, cpu speed issue, mp3 will need this device to have output for speakers]
-

-read pseudoprograms from a file, load them into memory and execute via terminal [need to add terminal too]

-

*/


//disk load function enum types
enum class Disk_LoadOption {
    Settings,
    Unknown
};

enum class Settings_type{
    general,
    visual, //visual: screen update rate
    audio,//audio: we have no audio
    power, //sleep mode, and so on. also tweak cpu speed
    hardware, //tweak cpu speed and other things
    datalogging, //data logging-hr/btemp/ambienttemp/colisions [will add other sensors later for cyberdeck things]
    deviceLink, //linking this to your phone and computer
    macros,
    misc
 
//i'll need to add new categories later but this is fine for now :3

};






//generalized disk load function that switches based on what we input
void Disk_load




//open settings file
#include "ff.h"
#include <cstring>

void updateSetting(const char* filename, const char* variable_name, const char* value) {
    FIL file;
    FIL tempFile;
    FRESULT result = f_open(&file, filename, FA_READ);
    FRESULT tempResult = f_open(&tempFile, "temp.csv", FA_WRITE | FA_CREATE_ALWAYS);

    if (result != FR_OK || tempResult != FR_OK) {
        printf("Failed to open settings file or create temp file.\n");
        return;
    }

    bool found = false;
    char line[128];

    // Read through each line in the existing file
    while (f_gets(line, sizeof(line), &file)) {
        char* current_variable = strtok(line, ",");
        char* current_value = strtok(nullptr, ",");

        if (current_variable && strcmp(current_variable, variable_name) == 0) {
            // Replace the line with the new value
            f_printf(&tempFile, "%s,%s\n", variable_name, value);
            found = true;
        } else if (current_variable && current_value) {
            // Write the unchanged line to the temp file
            f_printf(&tempFile, "%s,%s\n", current_variable, current_value);
        }
    }

    // If the variable was not found, add it as a new line
    if (!found) {
        f_printf(&tempFile, "%s,%s\n", variable_name, value);
    }

    f_close(&file);
    f_close(&tempFile);

    // Replace the original file with the temp file
    f_unlink(filename);                    // Delete the original file
    f_rename("temp.csv", filename);        // Rename temp file to original filename
    printf("Settings updated.\n");
}


