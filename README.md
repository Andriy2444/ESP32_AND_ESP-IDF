# **ESP32-ESP-IDF**

## **Navigation**
- [Overview](#Overview)
- [My project](#My-project)
- [Getting Started](#Getting-Started)
- [Components Details](#Components-Details)
- [Disclaimer](#Disclaimer)
- [Contributing](#Contributing)



## **Overview**
This repository contains my ESP32 projects, developed using VS Code with the ESP-IDF extension. The projects are organized into separate folders, making it easy to switch between them.



## **My project**
- [Counter](#Counter)
- [Timer](#Timer)



# **Getting Started**

Make sure you have the following set up before using this code:



### **ESP-IDF Installed**
- Follow the official ESP-IDF setup guide.
- Install the appropriate version of ESP-IDF(v5.3.1).



### **Software Tools**
- A terminal or command-line tool (e.g., PowerShell, bash, or cmd).
- IDE or text editor (e.g., Visual Studio Code, with ESP-IDF plugin).
- Python installed on your system(3.12.8).



### **Hardware Requirements**
- ESP32 development board.
- USB cable for flashing.
- Additional components used in the project.
- Breadboard and wires if applicable.



### **Setting Up the Project**
Clone or download the repository:
```bash
git clone https://github.com/Andriy2444/ESP32_AND_ESP-IDF.git
cd ESP32_AND_ESP-IDF
```

Configure ESP-IDF environment:
```bash
. ./export.sh   # Linux/macOS  
export.bat      # Windows  
```

Configure the project:
```bash
idf.py menuconfig
```

Select the project you want to run
```bash
cd (PROJECT NAME)
```

Build and flash the code:
```bash
idf.py build  
idf.py flash 
```



# **Project Details**


## **Counter**
This is a counter that displays numbers from 0000 to 9999 and starts over, information is displayed on a 4-digit 7-segment display.


Additional components:
- [4-digit 7-segment display](#Display).
  


## **Timer**
This project implements a countdown timer with buttons to increment and decrement the displayed value. The buttons allow for normal and rapid adjustments, with the speed of changes increasing as the buttons are held down. The timer starts when the "Start" button is pressed, and it counts down from the current value of the counter.

Additional components:
- [4-digit 7-segment display](#Display).
- 3 button(INC, DEC, Start).

GPIO(you can change for yourself):

- BTN_INC GPIO_NUM_27
- BTN_DEC GPIO_NUM_14
- BTN_START GPIO_NUM_12

# **Components Details**

## **Display**
This component allows you to control a 4-digit 7-segment display([Pinout](./Pinout/4-digit-display.png)) using ESP32 and ESP-IDF. It enables you to display numbers or other information on a 4-digit 7-segment display, useful for digital clocks, counters, or numerical indicators.

### GPIO(you can change for yourself):
- SEG_A GPIO_NUM_23
- SEG_B GPIO_NUM_22
- SEG_C GPIO_NUM_21
- SEG_D GPIO_NUM_19
- SEG_E GPIO_NUM_18
- SEG_F GPIO_NUM_5
- SEG_G GPIO_NUM_4

- DIGIT_1 GPIO_NUM_33
- DIGIT_2 GPIO_NUM_32
- DIGIT_3 GPIO_NUM_25
- DIGIT_4 GPIO_NUM_26

### How to use
- Call init_gpio_display(): Initialize the GPIO pins for the display.
- Create a Task: display_task() to continuously display a number on the 4-digit display.

# **Disclaimer**
This code has been tested and verified to work with ESP-IDF version v5.3.1, Python 3.12.8 and VS Code 1.95.3.

⚠️ No guarantees are provided if you use a different versions (older or newer).
Unexpected behavior, compilation errors, or runtime issues might occur if the environment does not match the specified version.

To ensure proper functionality, please use the exact versions mentioned above.



# **Contributing**
Contributions are welcome! If you have suggestions for improvements or new features, feel free to open an issue or submit a pull request.
