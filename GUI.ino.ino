#include <Wire.h>
#include <U8g2lib.h>
#include <WiFi.h>
#include <time.h>

// OLED Display setup
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

// WiFi credentials
const char* ssid = "TMNL-30AEB1";
const char* password = "GKFPH45UYMCNC3GM";

// Netherlands timezone settings
const long gmtOffset_sec = 3600;      // Netherlands base time (GMT+1)
const int daylightOffset_sec = 3600;  // Automatic daylight saving adjustment (+1 hour when active)

//pin assignments
#define BUTTON_PIN 32           // Button 
#define ROTARY_ENCODER_A_PIN 26 // CLK 
#define ROTARY_ENCODER_B_PIN 27 // DT 

// Screen states
enum ScreenState {
  HOME_SCREEN,
  MAIN_MENU,
  HEART_RATE_SCREEN,
  STEPS_SCREEN,
  BODY_TEMP_SCREEN,
  PLACEHOLDER_SCREEN
};

// State variables
ScreenState currentScreen = HOME_SCREEN;
int menuPosition = 0;
bool buttonPressed = false;
bool wifiConnected = false;

// Encoder tracking variables
int lastEncA = 0;

// Time variables
struct tm timeinfo;

// Placeholders sensor data
int heartRate = 72;
int stepCount = 1234;
float bodyTemp = 36.5;

void setup() {
  Serial.begin(115200);
  
  // Initialize OLED
  u8g2.begin();
  u8g2.setContrast(255);
  
  // Initialize pins
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(ROTARY_ENCODER_A_PIN, INPUT_PULLUP);
  pinMode(ROTARY_ENCODER_B_PIN, INPUT_PULLUP);
  
  // Get initial state
  lastEncA = digitalRead(ROTARY_ENCODER_A_PIN);
  
  // Connect to WiFi and sync time
  connectToWiFi();
  
  Serial.println("ESP32 Smartwatch GUI Ready");
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    wifiConnected = true;
    
    // Configure time with timezone
    configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org", "time.nist.gov");
    Serial.println("Getting time from NTP server...");
    
    // Wait for time to be set
    int timeAttempts = 0;
    while (!getLocalTime(&timeinfo) && timeAttempts < 10) {
      delay(1000);
      Serial.print(".");
      timeAttempts++;
    }
    
    if (timeAttempts < 10) {
      Serial.println("\nTime synchronized!");
    } else {
      Serial.println("\nFailed to get time");
    }
  } else {
    Serial.println("\nWiFi connection failed");
    wifiConnected = false;
  }
}

void drawHomeScreen() {
  char timeStr[9];
  
  if (wifiConnected && getLocalTime(&timeinfo)) {
    sprintf(timeStr, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  } else {
    sprintf(timeStr, "12:30:00"); // Fallback time
  }
  
  u8g2.clearBuffer();
  u8g2.drawFrame(0, 0, 128, 64);
  u8g2.setFont(u8g2_font_6x10_tf);
  
  // title
  const char* title = "Home Screen";
  int titleWidth = u8g2.getStrWidth(title);
  int titleX = (128 - titleWidth) / 2;
  u8g2.drawStr(titleX, 12, title);
  u8g2.drawLine(0, 15, 128, 15);
  
  // clock display
  u8g2.setFont(u8g2_font_profont22_tf);
  int timeWidth = u8g2.getStrWidth(timeStr);
  int timeX = (128 - timeWidth) / 2;
  u8g2.drawStr(timeX, 40, timeStr);
  
  // instruction text
  u8g2.setFont(u8g2_font_6x10_tf);
  const char* instruction = "Press for menu";
  int instructionWidth = u8g2.getStrWidth(instruction);
  int instructionX = (128 - instructionWidth) / 2;
  u8g2.drawStr(instructionX, 60, instruction);
  
  u8g2.sendBuffer();
}

void drawMainMenu() {
  u8g2.clearBuffer();
  u8g2.drawFrame(0, 0, 128, 64);
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(40, 12, "Main Menu");
  u8g2.drawLine(0, 15, 128, 15);
  
  //  menu items 
  const char* menuItems[] = {"Heart Rate", "Steps", "Body Temp", "Option 4", "Back to Home"};
  const int numMenuItems = sizeof(menuItems) / sizeof(menuItems[0]); // Calculate number of items automatically
  
  for (int i = 0; i < numMenuItems; i++) {
    int y = 25 + i * 7;  // Spacing adjusts based on number of items
    
    // Highlight selected menu item
    if (i == menuPosition) {
      u8g2.drawBox(0, y-6, 128, 8);
      u8g2.setDrawColor(0); // Draw in black on white background
    } else {
      u8g2.setDrawColor(1); // Draw in white on black background
    }
    
    u8g2.drawStr(10, y, menuItems[i]);
    u8g2.setDrawColor(1); // Reset to normal
  }
  
  u8g2.sendBuffer();
}

void drawHeartRateScreen() {
  u8g2.clearBuffer();
  u8g2.drawFrame(0, 0, 128, 64);
  u8g2.setFont(u8g2_font_6x10_tf);
  
  // Center the title
  const char* title = "Heart Rate";
  int titleWidth = u8g2.getStrWidth(title);
  int titleX = (128 - titleWidth) / 2;  // Calculate center position
  u8g2.drawStr(titleX, 12, title);
  u8g2.drawLine(0, 15, 128, 15);
  
  //  heart rate display
  char hrStr[10];
  sprintf(hrStr, "%d", heartRate);
  u8g2.setFont(u8g2_font_profont22_tf);
  
  // centering
  int numWidth = u8g2.getStrWidth(hrStr);
  u8g2.setFont(u8g2_font_6x10_tf);
  int bpmWidth = u8g2.getStrWidth("BPM");
  int totalWidth = numWidth + bpmWidth + 5; // 5 pixels spacing between number and BPM
  int startX = (128 - totalWidth) / 2;
  
  // Draw the heart rate number
  u8g2.setFont(u8g2_font_profont22_tf);
  u8g2.drawStr(startX, 40, hrStr);
  
  // Draw "BPM" next to the number
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(startX + numWidth + 5, 40, "BPM");
  
  // Center the status message based on heart rate value
  const char* statusMsg;
  if (heartRate >= 60 && heartRate <= 100) {
    statusMsg = "Normal Range";
  } else if (heartRate < 60) {
    statusMsg = "Low Rate";
  } else {
    statusMsg = "High Rate";
  }
  
  int statusWidth = u8g2.getStrWidth(statusMsg);
  int statusX = (128 - statusWidth) / 2;
  u8g2.drawStr(statusX, 55, statusMsg);
  
  // Center the back instruction
  const char* backMsg = "Press to go back";
  int backWidth = u8g2.getStrWidth(backMsg);
  int backX = (128 - backWidth) / 2;
  u8g2.drawStr(backX, 63, backMsg);
  
  u8g2.sendBuffer();
}

void drawStepsScreen() {
  u8g2.clearBuffer();
  u8g2.drawFrame(0, 0, 128, 64);
  u8g2.setFont(u8g2_font_6x10_tf);
  
  // Center the title
  const char* title = "Steps";
  int titleWidth = u8g2.getStrWidth(title);
  int titleX = (128 - titleWidth) / 2;
  u8g2.drawStr(titleX, 12, title);
  u8g2.drawLine(0, 15, 128, 15);
  
  // steps display 
  char stepsStr[10];
  sprintf(stepsStr, "%d", stepCount);
  u8g2.setFont(u8g2_font_profont17_tf);
  int stepsWidth = u8g2.getStrWidth(stepsStr);
  int stepsX = (128 - stepsWidth) / 2;
  u8g2.drawStr(stepsX, 35, stepsStr);
  
  // Center the description text
  u8g2.setFont(u8g2_font_6x10_tf);
  const char* description = "Today's Count";
  int descWidth = u8g2.getStrWidth(description);
  int descX = (128 - descWidth) / 2;
  u8g2.drawStr(descX, 45, description);
  
  // Center the back instruction
  const char* backMsg = "Press to go back";
  int backWidth = u8g2.getStrWidth(backMsg);
  int backX = (128 - backWidth) / 2;
  u8g2.drawStr(backX, 63, backMsg);
  
  u8g2.sendBuffer();
}

void drawBodyTempScreen() {
  u8g2.clearBuffer();
  u8g2.drawFrame(0, 0, 128, 64);
  u8g2.setFont(u8g2_font_6x10_tf);
  
  // Center the title
  const char* title = "Body Temp";
  int titleWidth = u8g2.getStrWidth(title);
  int titleX = (128 - titleWidth) / 2;
  u8g2.drawStr(titleX, 12, title);
  u8g2.drawLine(0, 15, 128, 15);
  
  //temperature display
  char tempStr[10];
  sprintf(tempStr, "%.1fC", bodyTemp); 
  u8g2.setFont(u8g2_font_profont17_tf);
  int tempWidth = u8g2.getStrWidth(tempStr);
  int tempX = (128 - tempWidth) / 2;
  u8g2.drawStr(tempX, 35, tempStr);
  
  // Center the status message based on temperature value
  u8g2.setFont(u8g2_font_6x10_tf);
  const char* statusMsg;
  if (bodyTemp >= 36.1 && bodyTemp <= 37.2) {
    statusMsg = "Normal";
  } else {
    statusMsg = "Check temp";
  }
  
  int statusWidth = u8g2.getStrWidth(statusMsg);
  int statusX = (128 - statusWidth) / 2;
  u8g2.drawStr(statusX, 45, statusMsg);
  
  // Center the back instruction
  const char* backMsg = "Press to go back";
  int backWidth = u8g2.getStrWidth(backMsg);
  int backX = (128 - backWidth) / 2;
  u8g2.drawStr(backX, 63, backMsg);
  
  u8g2.sendBuffer();
}

void drawPlaceholderScreen() {
  u8g2.clearBuffer();
  u8g2.drawFrame(0, 0, 128, 64);
  u8g2.setFont(u8g2_font_6x10_tf);
  
  // Center the title
  const char* title = "Option 4";
  int titleWidth = u8g2.getStrWidth(title);
  int titleX = (128 - titleWidth) / 2;
  u8g2.drawStr(titleX, 12, title);
  u8g2.drawLine(0, 15, 128, 15);
  
  // Center the placeholder messages
  const char* msg1 = "Coming Soon...";
  int msg1Width = u8g2.getStrWidth(msg1);
  int msg1X = (128 - msg1Width) / 2;
  u8g2.drawStr(msg1X, 35, msg1);
  
  const char* msg2 = "Feature TBD";
  int msg2Width = u8g2.getStrWidth(msg2);
  int msg2X = (128 - msg2Width) / 2;
  u8g2.drawStr(msg2X, 45, msg2);
  
  // Center the back instruction
  const char* backMsg = "Press to go back";
  int backWidth = u8g2.getStrWidth(backMsg);
  int backX = (128 - backWidth) / 2;
  u8g2.drawStr(backX, 63, backMsg);
  
  u8g2.sendBuffer();
}

void checkButton() {
  if (digitalRead(BUTTON_PIN) == LOW && !buttonPressed) {
    delay(50); // Debounce
    if (digitalRead(BUTTON_PIN) == LOW) {
      buttonPressed = true;
      
      switch (currentScreen) {
        case HOME_SCREEN:
          currentScreen = MAIN_MENU;
          menuPosition = 0;
          Serial.println("Switched to Main Menu");
          break;
          
        case MAIN_MENU:
          // Handle menu selection
          switch (menuPosition) {
            case 0: // Heart Rate
              currentScreen = HEART_RATE_SCREEN;
              Serial.println("Switched to Heart Rate Screen");
              break;
            case 1: // Steps
              currentScreen = STEPS_SCREEN;
              Serial.println("Switched to Steps Screen");
              break;
            case 2: // Body Temp
              currentScreen = BODY_TEMP_SCREEN;
              Serial.println("Switched to Body Temp Screen");
              break;
            case 3: // Placeholder
              currentScreen = PLACEHOLDER_SCREEN;
              Serial.println("Switched to Placeholder Screen");
              break;
            case 4: // Back to Home
              currentScreen = HOME_SCREEN;
              Serial.println("Returned to Home Screen");
              break;
          }
          break;
          
        case HEART_RATE_SCREEN:
        case STEPS_SCREEN:
        case BODY_TEMP_SCREEN:
        case PLACEHOLDER_SCREEN:
          currentScreen = MAIN_MENU;
          Serial.println("Returned to Main Menu");
          break;
      }
    }
  } else if (digitalRead(BUTTON_PIN) == HIGH) {
    buttonPressed = false;
  }
}

void checkRotation() {
  // Only process rotation when in menu screen
  if (currentScreen == MAIN_MENU) {
    // Calculate menu size dynamically 
    const char* menuItems[] = {"Heart Rate", "Steps", "Body Temp", "Option 4", "Back to Home"};
    const int numMenuItems = sizeof(menuItems) / sizeof(menuItems[0]);
    
    int currentEncA = digitalRead(ROTARY_ENCODER_A_PIN);
    int currentEncB = digitalRead(ROTARY_ENCODER_B_PIN);
    
    // Detect falling edge on CLK (pin A)
    if (lastEncA == HIGH && currentEncA == LOW) {
      
      if (currentEncB == HIGH) {
        // Rotate RIGHT (clockwise) tp move DOWN in menu
        menuPosition++;
        if (menuPosition >= numMenuItems) menuPosition = 0; // Wrap to top 
        Serial.print("RIGHT rotation - Menu position: ");
        Serial.println(menuPosition);
        
      } else {
        // Rotate LEFT (counter-clockwise) to move UP in menu
        menuPosition--;
        if (menuPosition < 0) menuPosition = numMenuItems - 1; // Wrap to bottom 
        Serial.print("LEFT rotation - Menu position: ");
        Serial.println(menuPosition);
      }
    }
    
    lastEncA = currentEncA;
  }
}

void loop() {
  checkButton();
  checkRotation();
  
  // Draw appropriate screen
  switch (currentScreen) {
    case HOME_SCREEN:
      drawHomeScreen();
      break;
    case MAIN_MENU:
      drawMainMenu();
      break;
    case HEART_RATE_SCREEN:
      drawHeartRateScreen();
      break;
    case STEPS_SCREEN:
      drawStepsScreen();
      break;
    case BODY_TEMP_SCREEN:
      drawBodyTempScreen();
      break;
    case PLACEHOLDER_SCREEN:
      drawPlaceholderScreen();
      break;
  }
  
  delay(10);
}