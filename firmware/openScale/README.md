OpenScale Project

Introduction

This project utilizes the HX711 load cell amplifier to accurately measure weight. It's designed to be used with the OpenScale board and integrates temperature readings and a user interface for calibration and taring. This document covers the setup necessary to use the HX711 library within the OpenScale project.

Prerequisites

Before you begin, ensure you have the following installed:

Arduino IDE or Particle Workbench
Any necessary drivers for your development board
Installation

Setting Up the HX711 Library
There are two ways to set up the HX711 library for this project:

1. Using Arduino Library Manager (Recommended)

To install the HX711 library through the Arduino Library Manager:

Open the Arduino IDE.
Navigate to Sketch > Include Library > Manage Libraries...
In the Library Manager, enter "HX711" in the search box.
Find the HX711 library by Bogdan Necula and click Install.
2. Manually Adding the Library

If you prefer to manually add the library:

Download the HX711 library from GitHub.
Unzip and place the library in the lib/ directory of your project:

OpenScaleProject/
└── lib/
    └── HX711/
        ├── HX711.h
        └── HX711.cpp
Ensure this path is reflected in your project's include statements.


Project Structure

OpenScaleProject/
│
├── src/
│   ├── OpenScale.ino
│   ├── system.ino
│   └── temperature.ino
│
└── lib/
    └── HX711/
        ├── HX711.h
        └── HX711.cpp
Usage

After installing the HX711 library, you can include it in your sketches like this:

cpp

#include <HX711.h>

Initialize the scale and calibrate using the instructions in system.ino.
Use menus.ino to navigate through calibration, taring, and weighing options.


License

Specify the license under which your project is made available. (e.g., MIT, GPL, etc.)