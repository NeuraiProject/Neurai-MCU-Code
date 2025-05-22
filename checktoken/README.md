
# ESP32 Token Balance Checker

This code demonstrates how to periodically query a blockchain RPC endpoint from an ESP32, parse the JSON response, and set a flag (`CHECK`) based on the token balance. It is designed for easy configuration and integration in the Arduino IDE.

---

## Table of Contents

1. [Overview](#overview)  
2. [Features](#features)  
3. [Hardware Requirements](#hardware-requirements)  
4. [Software Requirements](#software-requirements)  
5. [Configuration](#configuration)  
6. [Usage](#usage)  
7. [Code Structure](#code-structure)  
8. [How It Works](#how-it-works)  
9. [Adjusting the Poll Interval](#adjusting-the-poll-interval)  
10. [License](#license)

---

## Overview

The ESP32 Token Balance Checker connects to a Wi-Fi network, issues an HTTPS POST to a specified RPC endpoint, and extracts the balance of a given token for a specified address. If the balance is **greater than or equal to 1**, it sets the global variable `CHECK = 1`; otherwise, `CHECK = 0`. Results are printed to the serial console.

---

## Features

- **HTTPS RPC call** using `WiFiClientSecure` + `HTTPClient`  
- **JSON parsing** with [ArduinoJson](https://arduinojson.org/)  
- Configurable parameters at the top of the sketch  
- Status flag (`TOKEN`) for conditional logic in your application  
- Serial debug output  

---

## Hardware Requirements

- ESP32 development board (e.g., ESP32-DevKitC)  
- USB-A to Micro-USB (or USB-C) cable  
- Internet connection (Wi-Fi)

---

## Software Requirements

- [Arduino IDE](https://www.arduino.cc/en/software) (version 1.8.13 or later)  
- ESP32 board support installed in Arduino IDE  
- Libraries:
  - **WiFi** (built-in)  
  - **HTTPClient** (built-in)  
  - **ArduinoJson** (v6.x)

---

## Configuration

At the top of `TokenBalanceChecker.ino`, modify the following constants:

```cpp
// Wi-Fi credentials
const char* WIFI_SSID  = "YOUR_SSID";
const char* WIFI_PASS  = "YOUR_PASSWORD";

// RPC endpoint and query parameters
const char* RPC_URL    = "https://rpc-main.neurai.top/rpc";
const char* ADDRESS    = "ADDRESS";   // Blockchain address to query
const char* TOKEN_NAME = "TOKEN";        // Token symbol in JSON result
````

---

## Usage

1. Open the Arduino IDE and install the ESP32 board package (Tools → Board → Boards Manager → search for “esp32”).
2. Install **ArduinoJson** via Library Manager (Sketch → Include Library → Manage Libraries → search “ArduinoJson”).
3. Copy the provided sketch into a new `.ino` file (e.g., `TokenBalanceChecker.ino`).
4. Configure your Wi-Fi credentials and RPC parameters.
5. Select your ESP32 board and the correct COM port under **Tools**.
6. Upload the sketch.
7. Open the Serial Monitor at 115200 baud to view status messages.

---

## Code Structure

```text
TokenBalanceChecker.ino
├── Includes and Libraries
│   └── WiFi.h, HTTPClient.h, ArduinoJson.h
├── User-configurable parameters
│   └── WIFI_SSID, WIFI_PASS, RPC_URL, ADDRESS, TOKEN_NAME
├── Global Variables
│   └── int CHECK
├── setup()
│   └── Initialize Serial, connect to Wi-Fi
└── loop()
    ├── Perform HTTPS POST
    ├── Parse JSON response
    ├── Extract token balance
    ├── Update CHECK flag
    └── Delay before next iteration
```

---

## How It Works

1. **Wi-Fi Connection**

   * `WiFi.begin(ssid, pass)` → wait until `WL_CONNECTED`.

2. **Secure HTTP Request**

   * `WiFiClientSecure client; client.setInsecure();`
   * `HTTPClient http; http.begin(client, RPC_URL);`
   * Build JSON payload:

     ```json
     {
       "method": "listassetbalancesbyaddress",
       "params": ["ADDRESS"]
     }
     ```
   * `http.POST(payload)` → read `httpCode`.

3. **JSON Parsing**

   * `DynamicJsonDocument doc(capacity);`
   * `deserializeJson(doc, response);`
   * Access `doc["result"][TOKEN_NAME]` → `double balance`.

4. **Flag Update**

   ```cpp
   if (balance >= 1.0) {
     CHECK = 1;
   } else {
     CHECK = 0;
   }
   ```

   * Print `balance` and `CHECK` to Serial.

---

## Adjusting the Poll Interval

By default, the loop pauses for 30 seconds:

```cpp
delay(30000);
```

* **To poll every minute**, use `delay(60000);`
* **To poll every 10 seconds**, use `delay(10000);`

For non-blocking timing, consider using `millis()` instead of `delay()`.

---

## License

This code is released under the MIT License. Feel free to adapt and integrate it into your own projects.

```text
MIT License
```

---

**Author:** Asimov
**Last Updated:** May 22, 2025

