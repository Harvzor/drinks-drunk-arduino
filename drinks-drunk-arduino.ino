#include "heltec.h"
#include "WiFi.h"

#include <ArduinoJson.h>

#include "env.h"

void WIFISetUp(void)
{
    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.disconnect(true);
    
    delay(1000);
    
    WiFi.mode(WIFI_STA);
    WiFi.setAutoConnect(true);
    WiFi.begin(wifi_ssid, wifi_password);
    
    delay(100);

    byte count = 0;
    while (WiFi.status() != WL_CONNECTED && count < 10)
    {
        count++;

        delay(500);

        Heltec.display -> drawString(0, 0, "Connecting...");
        Heltec.display -> display();
    }

    Heltec.display -> clear();

    if (WiFi.status() == WL_CONNECTED)
    {
        Heltec.display -> drawString(0, 0, "Connecting...OK.");
        Heltec.display -> display();
    }
    else
    {
        Heltec.display -> clear();
        Heltec.display -> drawString(0, 0, "Connecting...Failed");
        Heltec.display -> display();
    }

    Heltec.display -> drawString(0, 10, "WIFI Setup done");
    Heltec.display -> display();

    delay(500);
}

void WIFIScan(void)
{
    Heltec.display -> drawString(0, 20, "Scan start...");
    Heltec.display -> display();

    int n = WiFi.scanNetworks();
    Heltec.display -> drawString(0, 30, "Scan done");
    Heltec.display -> display();
    delay(500);
    Heltec.display -> clear();

    if (n == 0)
    {
        Heltec.display -> clear();
        Heltec.display -> drawString(0, 0, "no network found");
        Heltec.display -> display();
        while(1);
    }
    else
    {
        Serial.print(n);
        Heltec.display -> drawString(0, 0, (String)n);
        Heltec.display -> drawString(14, 0, "networks found:");
        Heltec.display -> display();
        delay(500);

        for (int i = 0; i < n; ++i) {
        // Print SSID and RSSI for each network found
            Heltec.display -> drawString(0, (i+1)*9,(String)(i + 1));
            Heltec.display -> drawString(6, (i+1)*9, ":");
            Heltec.display -> drawString(12,(i+1)*9, (String)(WiFi.SSID(i)));
            Heltec.display -> drawString(90,(i+1)*9, " (");
            Heltec.display -> drawString(98,(i+1)*9, (String)(WiFi.RSSI(i)));
            Heltec.display -> drawString(114,(i+1)*9, ")");
            //                        display.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
            delay(10);
        }
    }

    Heltec.display -> display();
    delay(800);
    Heltec.display -> clear();
}


void setup()
{
    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);

    Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/, true /*Serial Enable*/);

    Heltec.display -> clear();
    Heltec.display -> drawString(0, 0, "Starting...");
    Heltec.display -> display();
    
    delay(300);
    
    Heltec.display -> clear();
    
    WIFISetUp();
}

void displayDrinks(JsonArray drinksArray) {
    int i = 0;

    Heltec.display -> clear();

    for (JsonVariant v : drinksArray) {
        // Serial.println(v["name"].as<const char*>());

        Heltec.display -> drawString(0, i * 10, v["name"].as<const char*>());

        i++;
    }

    Heltec.display -> display();
}

void loop()
{
    delay(5000);

    JsonArray drinksArray = getDrinks();
    displayDrinks(drinksArray);
}
