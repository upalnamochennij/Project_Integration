//
// Created by bloom on 18.05.2025.
//
#include "OLEDscreen.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C //< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

OLEDscreen::OLEDscreen() : oled_display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET){
}


void OLEDscreen::initComponent(){
}
