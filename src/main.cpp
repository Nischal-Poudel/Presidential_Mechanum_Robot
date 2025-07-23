#include <Arduino.h>
#include <IBusBM.h>
#include <serialdrive.h>

/*
  Display channel value updates and warn if any channel value is abnormal.
  Detect receiver disconnection using Serial2 timeout.
*/

IBusBM IBus; // IBus object

#define STOP 0
#define ForwardDirection 1
#define BackwardDirection 2

int overalldirection = ForwardDirection; // Default direction is forward

#define Channel1 0
#define Channel2 1
#define Channel3 2
#define Channel4 3
#define Channel5 4
#define Channel6 5
#define Channel7 6
#define Channel8 7

void setup()
{
    Serial.begin(115200);
    Serial2.begin(115200, SERIAL_8N1, 18, 17); // RX2, TX2
    IBus.begin(Serial2, 1);
    iniMotorDriver(); // Initialize motor driver
    enableDrivers();  // Enable motor drivers
    Serial.println("Start IBus Channel Scanner");
}

void loop()
{
    static int prevValues[16] = {0};
    int values[16];

    for (int i = 0; i < 8; i++)
    {
        values[i] = IBus.readChannel(i);
        if (values[i] == 0 || values[i] == 65535 || values[i] < 900 || values[i] > 2100)
            Serial.printf("WARNING: Channel %2d abnormal value: %d\n", i + 1, values[i]);
        if (values[i] != prevValues[i])
        {
            Serial.printf("Channel %2d updated: %d\n", i + 1, values[i]);
            prevValues[i] = values[i];
        }
    }

    // Tank mixing logic
    int throttle = map(values[Channel2], 900, 2100, -4095, 4095); // Forward/backward
    int steering = map(values[Channel4], 900, 2100, -4095, 4095); // Left/right

    int leftPWM = throttle + steering;
    int rightPWM = throttle - steering;

    // Constrain to valid PWM range
    leftPWM = constrain(leftPWM, -4095, 4095);
    rightPWM = constrain(rightPWM, -4095, 4095);

    // Set direction and speed for each side
    setLeftSide(leftPWM >= 0, abs(leftPWM));
    setRightSide(rightPWM >= 0, abs(rightPWM));
}
