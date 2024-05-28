#include <WiFi.h>
#include <HTTPClient.h>
#include "Bounce2.h"
#include <heltec.h>                // Display

// Replace with your network credentials
const char *ssid = "IoTWiFi";
const char *password = "YourPassword";

// API endpoint and authentication token
const char *serverName = "https://xxxxx.indigodomo.net/v2/api/command";
const char *authToken = "YourAuthToken";

// JSON data to send
const char *jsonData = "{\"message\": \"indigo.actionGroup.execute\", \"objectId\": 1901754189}";

Bounce2::Button button;

void setup()
{
  Serial.begin(115200);

  Heltec.begin(true /*DisplayEnable Enable*/, true /*LoRa Disable*/, false /*Serial Enable*/);
  Heltec.display->screenRotate(ANGLE_180_DEGREE);
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Indigo Actionbutton...");
  Heltec.display->display();

  button.attach(0, INPUT_PULLUP);
  button.interval(5);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
}

void sendActionRequest()
{
  // Check Wi-Fi connection
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;

    // Specify content type and authorization headers
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", String("Bearer ") + authToken);

    // Send POST request
    int httpResponseCode = http.POST(jsonData);

    // Check the response
    if (httpResponseCode > 0)
    {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    }
    else
    {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    // End the connection
    http.end();
  }
  else
  {
    Serial.println("WiFi not connected");
  }
}


void loop()
{
    Heltec.display->clear();
    Heltec.display->drawString(0, 0, "Indigo Actionbutton");
    Heltec.display->drawString(0, 10, "Waiting for Button...");
    Heltec.display->display();

    button.update();

    if (button.pressed())
    {
      Serial.println("Button pressed!");
      Heltec.display->drawString(0, 20, "PRESSED!");
      Heltec.display->display();

      // Call the function to send the action request
      sendActionRequest();
      delay(5000); // 5 seconds delay
    }
    delay(100); // 60 seconds delay
}
