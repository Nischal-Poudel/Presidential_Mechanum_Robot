#include <Arduino.h>
#include <serialdrive.h>

void fadeMotor(void (*motorFunc)(bool, uint32_t), const char *name)
{
    // Forward fade
    Serial.printf("%s FORWARD fade up\n", name);
    for (uint16_t speed = 0; speed <= 4095; speed += 64)
    {
        motorFunc(true, speed);
        delay(10);
    }
    Serial.printf("%s FORWARD fade down\n", name);
    for (int speed = 4095; speed >= 0; speed -= 64)
    {
        motorFunc(true, speed);
        delay(10);
    }
    stopAll();
    delay(2500);

    // Reverse fade
    Serial.printf("%s REVERSE fade up\n", name);
    for (uint16_t speed = 0; speed <= 4095; speed += 64)
    {
        motorFunc(false, speed);
        delay(10);
    }
    Serial.printf("%s REVERSE fade down\n", name);
    for (int speed = 4095; speed >= 0; speed -= 64)
    {
        motorFunc(false, speed);
        delay(10);
    }
    stopAll();
    delay(1500);
}

void setup()
{
    Serial.begin(115200);
    iniMotorDriver();
    Serial.println("Testing each motor individually with fade (forward and reverse)...");
    delay(1000);
}

void loop()
{
    fadeMotor(setFrontLeft, "Front Left");
    delay(2000);
    fadeMotor(setFrontRight, "Front Right");
    delay(2000);
    fadeMotor(setRearLeft, "Rear Left");
    delay(2000);
    fadeMotor(setRearRight, "Rear Right");
    delay(2000);
}