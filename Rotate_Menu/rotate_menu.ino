#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

// Button pins
#define BUTTON_NAVIGATE 32  // Button to navigate through menu
#define BUTTON_SELECT 33    // Button to select/enter

// Create display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Menu states
enum MenuState {
  MAIN_MENU,
  SENSOR_DATA,
  NETWORK_STATUS,
  DEVICE_CONFIG
};

// Current menu state
MenuState currentState = MAIN_MENU;
int menuSelection = 0;
int maxMenuItems = 3;

// Button debounce variables
unsigned long lastNavPress = 0;
unsigned long lastSelPress = 0;
const unsigned long debounceDelay = 200;

// Demo data for submenus
float temperature = 25.5;
float humidity = 65.2;
bool wifiConnected = true;
String deviceID = "ESP32-001";

void setup() {
  Serial.begin(115200);
  
  // Initialize buttons
  pinMode(BUTTON_NAVIGATE, INPUT_PULLUP);
  pinMode(BUTTON_SELECT, INPUT_PULLUP);
  
  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  // Clear display buffer
  display.clearDisplay();
  display.display();
  
  // Set text properties
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  Serial.println("Neurai IoT Menu System Started");
}

void loop() {
  // Handle button inputs
  handleButtons();
  
  // Update display based on current state
  updateDisplay();
  
  delay(10);
}

void handleButtons() {
  // Navigate button - cycles through menu items
  if (digitalRead(BUTTON_NAVIGATE) == LOW) {
    if (millis() - lastNavPress > debounceDelay) {
      lastNavPress = millis();
      
      if (currentState == MAIN_MENU) {
        menuSelection++;
        if (menuSelection >= maxMenuItems) {
          menuSelection = 0;  // Cycle back to first item
        }
      }
    }
  }
  
  // Select button - enters menu item or goes back
  if (digitalRead(BUTTON_SELECT) == LOW) {
    if (millis() - lastSelPress > debounceDelay) {
      lastSelPress = millis();
      
      handleSelection();
    }
  }
}

void handleSelection() {
  switch (currentState) {
    case MAIN_MENU:
      // Enter selected submenu
      switch (menuSelection) {
        case 0:
          currentState = SENSOR_DATA;
          break;
        case 1:
          currentState = NETWORK_STATUS;
          break;
        case 2:
          currentState = DEVICE_CONFIG;
          break;
      }
      break;
      
    case SENSOR_DATA:
    case NETWORK_STATUS:
    case DEVICE_CONFIG:
      // Go back to main menu
      currentState = MAIN_MENU;
      break;
  }
}

void updateDisplay() {
  display.clearDisplay();
  
  switch (currentState) {
    case MAIN_MENU:
      drawMainMenu();
      break;
    case SENSOR_DATA:
      drawSensorData();
      break;
    case NETWORK_STATUS:
      drawNetworkStatus();
      break;
    case DEVICE_CONFIG:
      drawDeviceConfig();
      break;
  }
  
  display.display();
}

void drawMainMenu() {
  // Draw title
  display.setTextSize(2);
  display.setCursor(10, 0);
  display.print("Neurai IoT");
  
  // Draw menu items
  display.setTextSize(1);
  
  // Option 1: Sensor Data
  display.setCursor(10, 25);
  if (menuSelection == 0) display.print("> ");
  else display.print("  ");
  display.print("Sensor Data");
  
  // Option 2: Network Status
  display.setCursor(10, 35);
  if (menuSelection == 1) display.print("> ");
  else display.print("  ");
  display.print("Network Status");
  
  // Option 3: Device Config
  display.setCursor(10, 45);
  if (menuSelection == 2) display.print("> ");
  else display.print("  ");
  display.print("Device Config");
  
  // Instructions
  display.setTextSize(1);
  display.setCursor(0, 56);
  display.print("NAV:Cycle SEL:Enter");
}

void drawSensorData() {
  // Title
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("=== SENSOR DATA ===");
  
  // Sensor readings
  display.setCursor(0, 15);
  display.print("Temperature: ");
  display.print(temperature);
  display.print(" C");
  
  display.setCursor(0, 25);
  display.print("Humidity: ");
  display.print(humidity);
  display.print(" %");
  
  display.setCursor(0, 35);
  display.print("Light: ");
  display.print("850 lux");
  
  display.setCursor(0, 45);
  display.print("Pressure: ");
  display.print("1013 hPa");
  
  // Back option
  display.setCursor(0, 56);
  display.print("SEL: Back to Menu");
}

void drawNetworkStatus() {
  // Title
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("=== NETWORK STATUS ===");
  
  // Network info
  display.setCursor(0, 15);
  display.print("WiFi: ");
  display.print(wifiConnected ? "Connected" : "Disconnected");
  
  display.setCursor(0, 25);
  display.print("SSID: Neurai_IoT");
  
  display.setCursor(0, 35);
  display.print("IP: 192.168.1.100");
  
  display.setCursor(0, 45);
  display.print("MQTT: Connected");
  
  // Back option
  display.setCursor(0, 56);
  display.print("SEL: Back to Menu");
}

void drawDeviceConfig() {
  // Title
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("=== DEVICE CONFIG ===");
  
  // Device info
  display.setCursor(0, 15);
  display.print("Device ID: ");
  display.print(deviceID);
  
  display.setCursor(0, 25);
  display.print("Firmware: v1.0.3");
  
  display.setCursor(0, 35);
  display.print("Uptime: 2d 14h 32m");
  
  display.setCursor(0, 45);
  display.print("Free RAM: 45.2 KB");
  
  // Back option
  display.setCursor(0, 56);
  display.print("SEL: Back to Menu");
}

// Optional: Add function to update sensor data periodically
void updateSensorData() {
  // Simulate sensor readings with random variations
  temperature += random(-10, 11) / 10.0;
  humidity += random(-20, 21) / 10.0;
  
  // Keep values in reasonable ranges
  if (temperature < 20) temperature = 20;
  if (temperature > 30) temperature = 30;
  if (humidity < 40) humidity = 40;
  if (humidity > 80) humidity = 80;
}
