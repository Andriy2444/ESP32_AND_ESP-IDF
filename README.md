# **ESP32-ESP-IDF**


## **Overview**
This repository contains my ESP32 projects, developed using VS Code with the ESP-IDF extension. The projects are organized into separate folders, making it easy to switch between them.


## **My project**
- Counter


# **Getting Started**

Make sure you have the following set up before using this code:


## **ESP-IDF Installed**
- Follow the official ESP-IDF setup guide
- Install the appropriate version of ESP-IDF(v5.3.1)


## **Software Tools**
- A terminal or command-line tool (e.g., PowerShell, bash, or cmd).
- IDE or text editor (e.g., Visual Studio Code, with ESP-IDF plugin if preferred).
- Python installed on your system(3.12.8).


## **Setting Up the Project**
Clone or download the repository:
```bash
git clone https://github.com/Andriy2444/ESP32_AND_ESP-IDF.git
cd ESP32_AND_ESP-ID
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

Build and flash the code:
```bash
idf.py build  
idf.py flash 
```






## **Dependencies**
- ESP-IDF: The official development framework for ESP32.
- CMake: Build system generator.
  
## **Contributing**
Contributions are welcome! If you have suggestions for improvements or new features, feel free to open an issue or submit a pull request.
