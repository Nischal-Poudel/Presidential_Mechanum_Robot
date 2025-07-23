#include <Arduino.h>
#include <IBusBM.h>
#include <serialdrive.h>

/*
  Display channel value updates and warn if any channel value is abnormal.
  Detect receiver disconnection using Serial2 timeout.
*/

IBusBM IBus; // IBus object

void fadeMove(void (*moveFunc)(uint8_t), const char *name)
{
  Serial.printf("Moving %s with fading speed...\n", name);
  // Fade up
  for (uint8_t speed = 0; speed <= 255; speed += 5)
  {
    moveFunc(speed);
    delay(10);
  }
  // Fade down
  for (int speed = 255; speed >= 0; speed -= 5)
  {
    moveFunc(speed);
    delay(10);
  }
  stopAll();
  delay(500);
}

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 18, 17); // RX2, TX2
  IBus.begin(Serial2, 1);
  iniMotorDriver();
  Serial.println("Start IBus Channel Scanner");
}

void loop()
{
  fadeMove(moveForward, "forward");
  fadeMove(moveBackward, "backward");
  fadeMove(turnLeft, "left");
  fadeMove(turnRight, "right");
  delay(1000);
}
// void loop()
// {
//   static int prevValues[16] = {0};
//   int values[16];
//   bool anyChanged = false;
//   static bool wasConnected = false;
//   static unsigned long lastDataTime = 0;
//   const unsigned long timeoutMs = 500;

//   // Check for incoming data on Serial2
//   if (Serial2.available())
//   {
//     lastDataTime = millis();
//   }

//   bool isConnected = (millis() - lastDataTime) < timeoutMs;

//   if (isConnected && !wasConnected)
//   {
//     Serial.println("Transmitter connected!");
//     wasConnected = true;
//   }
//   else if (!isConnected && wasConnected)
//   {
//     Serial.println("Transmitter disconnected!");
//     wasConnected = false;
//   }

//   // Only process channels if connected
//   if (isConnected)
//   {
//     for (int i = 0; i < 8; i++)
//     {
//       values[i] = IBus.readChannel(i);

//       // Check for abnormal values
//       if (values[i] == 0 || values[i] == 65535 || values[i] < 900 || values[i] > 2100)
//       {
//         Serial.printf("WARNING: Channel %2d abnormal value: %d\n", i + 1, values[i]);
//       }

//       // Only print if value changed
//       if (values[i] != prevValues[i])
//       {
//         Serial.printf("Channel %2d updated: %d\n", i + 1, values[i]);
//         prevValues[i] = values[i];
//         anyChanged = true;
//       }
//     }

//     if (anyChanged)
//       Serial.println("----------------------");
//   }

//   delay(10);
// }
