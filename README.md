> [!CAUTION]
> This project is created for educational purposes only. Use it responsibly and never deploy it on machines without explicit permission.

# Keyloggy
A Linux keylogger written in C++ (with no external libraries) that reads raw keyboard input directly from the hardware device file and logs keystrokes in a human-readable format. This project shows low-level system programming and Linux input subsystem handling.

## How It Works
In Linux, we know everything is a file - including your keyboard. The kernel exposes your keyboard input through a device files located at `/dev/input/event*`. Keyloggy reads these raw input events directly (bypassing the windowing system completely) and translates hardware keycodes into readable text.

The program:
1. Opens the keyboard device file for reading from `/dev/input/`
2. Reads `input_event` structures from the kernel
3. Maintains the order of currently-held keys and modifiers
4. Translates hardware codes to human-readable key names
5. Logs combinations (e.g., `Ctrl+Shift+A`) while preserving the press order
6. Writes all output to `logs.txt`

## Features
- **Direct kernel-level capture**: Reads raw input events, not OS-level keyboard APIs
- **Human-readable output**: Converts keycodes to actual key names and characters
- **All keys support**: 
  - Standard keys (letters, numbers, symbols)
  - Function keys (F1-F24)
  - Navigation keys (arrows, home, end, etc.)
  - Numpad keys
  - Media keys
- **Modifier**: Correctly handles Shift, Ctrl, Alt, and Win/Super keys
- **Handles Combination of keys**: Captures multi-key shortcuts in order (e.g., `Ctrl+Shift+Esc`)
- **Shift mapping**: Automatically logs shifted characters (e.g., `;` → `:` with Shift)
- **Hold detection**: Differentiates between single presses and held keys

## Limitations
- **No OS-level remap support**: As we are reading directly from the hardware device, any custom key remapping done at the OS level (X11, Wayland, etc.) is bypassed. Only physical key presses are logged.
- **Root privileges required**: Accessing `/dev/input/` device files requires sudo, so read the code first
- **Device detection**: The program doesn't automatically detect the keyboard device. You must specify the correct `/dev/input/event*` path explicitly.
- **CAPS Lock not fully handled**: Currently doesn't handle Caps Lock states

## Prerequisites
- Linux operating system (kernel 2.6+)
- C++ compiler with C++11 support or later
- Root/sudo access for running the program

## Installation & Building

```bash
# Clone the repository
git clone <repository-url>
cd keyloggy

# Compile
g++ -std=c++11 -o keyloggy keyloggy.cpp

# Executing the program
./keyloggy <file-path>

# Read the log file
cat logs.txt
