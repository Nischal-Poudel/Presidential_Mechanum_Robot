#include <Arduino.h>

// Example pin definitions (update as needed)
#define A_IN1 13 // Motor Driver A, Motor 1 (Front Left)
#define A_IN2 5  // Motor Driver A, Motor 1 (Front Left)
#define A_IN3 14 // Motor Driver A, Motor 2 (Front Right)
#define A_IN4 27 // Motor Driver A, Motor 2 (Front Right)
#define A_EN1 26 // Enable pin for Motor Driver A, Motor 1
#define A_EN2 25 // Enable pin for Motor Driver A, Motor 2

#define B_IN1 32 // Motor Driver B, Motor 1 (Rear Left)
#define B_IN2 33 // Motor Driver B, Motor 1 (Rear Left)
#define B_IN3 21 // Motor Driver B, Motor 2 (Rear Right)
#define B_IN4 19 // Motor Driver B, Motor 2 (Rear Right)
#define B_EN1 22 // Enable pin for Motor Driver B, Motor 1
#define B_EN2 23 // Enable pin for Motor Driver B, Motor 2

// PWM channels for ESP32 (must be unique per pin)
#define PWM_A_EN1_CH 0
#define PWM_A_EN2_CH 1
#define PWM_B_EN1_CH 2
#define PWM_B_EN2_CH 3

#define PWM_FREQ 5000
#define LEDC_TIMER_12_BIT 12

void setMotor(int in1, int in2, bool forward)
{
    if (forward)
    {
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
    }
    else
    {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
    }
}

void stopMotor(int in1, int in2)
{
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
}

// Analog (PWM) speed control for each enable pin
void setMotorSpeed(int enPin, int pwmChannel, uint32_t speed)
{
    ledcWrite(pwmChannel, speed); // speed: 0-255
}

void moveForward(uint32_t speed = 0)
{
    setMotor(A_IN1, A_IN2, true); // Front Left
    setMotor(A_IN3, A_IN4, true); // Front Right
    setMotor(B_IN1, B_IN2, true); // Rear Left
    setMotor(B_IN3, B_IN4, true); // Rear Right

    setMotorSpeed(A_EN1, PWM_A_EN1_CH, speed);
    setMotorSpeed(A_EN2, PWM_A_EN2_CH, speed);
    setMotorSpeed(B_EN1, PWM_B_EN1_CH, speed);
    setMotorSpeed(B_EN2, PWM_B_EN2_CH, speed);
}

void moveBackward(uint32_t speed = 0)
{
    setMotor(A_IN1, A_IN2, false);
    setMotor(A_IN3, A_IN4, false);
    setMotor(B_IN1, B_IN2, false);
    setMotor(B_IN3, B_IN4, false);

    setMotorSpeed(A_EN1, PWM_A_EN1_CH, speed);
    setMotorSpeed(A_EN2, PWM_A_EN2_CH, speed);
    setMotorSpeed(B_EN1, PWM_B_EN1_CH, speed);
    setMotorSpeed(B_EN2, PWM_B_EN2_CH, speed);
}

void turnLeft(uint32_t speed = 0)
{
    setMotor(A_IN1, A_IN2, false); // Front Left reverse
    setMotor(A_IN3, A_IN4, true);  // Front Right forward
    setMotor(B_IN1, B_IN2, false); // Rear Left reverse
    setMotor(B_IN3, B_IN4, true);  // Rear Right forward

    setMotorSpeed(A_EN1, PWM_A_EN1_CH, speed);
    setMotorSpeed(A_EN2, PWM_A_EN2_CH, speed);
    setMotorSpeed(B_EN1, PWM_B_EN1_CH, speed);
    setMotorSpeed(B_EN2, PWM_B_EN2_CH, speed);
}

void turnRight(uint32_t speed = 0)
{
    setMotor(A_IN1, A_IN2, true);  // Front Left forward
    setMotor(A_IN3, A_IN4, false); // Front Right reverse
    setMotor(B_IN1, B_IN2, true);  // Rear Left forward
    setMotor(B_IN3, B_IN4, false); // Rear Right reverse

    setMotorSpeed(A_EN1, PWM_A_EN1_CH, speed);
    setMotorSpeed(A_EN2, PWM_A_EN2_CH, speed);
    setMotorSpeed(B_EN1, PWM_B_EN1_CH, speed);
    setMotorSpeed(B_EN2, PWM_B_EN2_CH, speed);
}

void stopAll()
{
    stopMotor(A_IN1, A_IN2);
    stopMotor(A_IN3, A_IN4);
    stopMotor(B_IN1, B_IN2);
    stopMotor(B_IN3, B_IN4);

    setMotorSpeed(A_EN1, PWM_A_EN1_CH, 0);
    setMotorSpeed(A_EN2, PWM_A_EN2_CH, 0);
    setMotorSpeed(B_EN1, PWM_B_EN1_CH, 0);
    setMotorSpeed(B_EN2, PWM_B_EN2_CH, 0);
}

void enableDrivers()
{
    // PWM always enabled, but you can set speed to max
    setMotorSpeed(A_EN1, PWM_A_EN1_CH, 255);
    setMotorSpeed(A_EN2, PWM_A_EN2_CH, 255);
    setMotorSpeed(B_EN1, PWM_B_EN1_CH, 255);
    setMotorSpeed(B_EN2, PWM_B_EN2_CH, 255);
}

void disableDrivers()
{
    setMotorSpeed(A_EN1, PWM_A_EN1_CH, 0);
    setMotorSpeed(A_EN2, PWM_A_EN2_CH, 0);
    setMotorSpeed(B_EN1, PWM_B_EN1_CH, 0);
    setMotorSpeed(B_EN2, PWM_B_EN2_CH, 0);
}

void iniMotorDriver()
{
    pinMode(A_IN1, OUTPUT_OPEN_DRAIN);
    pinMode(A_IN2, OUTPUT_OPEN_DRAIN);
    pinMode(A_IN3, OUTPUT_OPEN_DRAIN);
    pinMode(A_IN4, OUTPUT_OPEN_DRAIN);

    pinMode(B_IN1, OUTPUT_OPEN_DRAIN);
    pinMode(B_IN2, OUTPUT_OPEN_DRAIN);
    pinMode(B_IN3, OUTPUT_OPEN_DRAIN);
    pinMode(B_IN4, OUTPUT_OPEN_DRAIN);

    pinMode(A_EN1, OUTPUT);
    pinMode(A_EN2, OUTPUT);
    pinMode(B_EN1, OUTPUT);
    pinMode(B_EN2, OUTPUT);

    // Setup PWM channels for enable pins
    ledcSetup(PWM_A_EN1_CH, PWM_FREQ, LEDC_TIMER_12_BIT);
    ledcAttachPin(A_EN1, PWM_A_EN1_CH);

    ledcSetup(PWM_A_EN2_CH, PWM_FREQ, LEDC_TIMER_12_BIT);
    ledcAttachPin(A_EN2, PWM_A_EN2_CH);

    ledcSetup(PWM_B_EN1_CH, PWM_FREQ, LEDC_TIMER_12_BIT);
    ledcAttachPin(B_EN1, PWM_B_EN1_CH);

    ledcSetup(PWM_B_EN2_CH, PWM_FREQ, LEDC_TIMER_12_BIT);
    ledcAttachPin(B_EN2, PWM_B_EN2_CH);

    enableDrivers();
    stopAll();
}

// Set direction and speed for left side motors independently
void setLeftSide(bool forward, uint32_t speed)
{
    // Front Left
    setMotor(A_IN1, A_IN2, forward);
    setMotorSpeed(A_EN1, PWM_A_EN1_CH, speed);

    // Rear Left
    setMotor(B_IN1, B_IN2, forward);
    setMotorSpeed(B_EN1, PWM_B_EN1_CH, speed);
}

// Set direction and speed for right side motors independently
void setRightSide(bool forward, uint32_t speed)
{
    // Front Right
    setMotor(A_IN3, A_IN4, forward);
    setMotorSpeed(A_EN2, PWM_A_EN2_CH, speed);

    // Rear Right
    setMotor(B_IN3, B_IN4, forward);
    setMotorSpeed(B_EN2, PWM_B_EN2_CH, speed);
}

// Example: driveMotor('f', 200) for forward at speed 200
void driveMotor(char input, uint32_t speed = 0)
{
    char cmd = input;
    switch (cmd)
    {
    case 'f':
        moveForward(speed);
        Serial.println("Forward");
        break;
    case 'b':
        moveBackward(speed);
        Serial.println("Backward");
        break;
    case 'l':
        turnLeft(speed);
        Serial.println("Left");
        break;
    case 'r':
        turnRight(speed);
        Serial.println("Right");
        break;
    case 's':
        stopAll();
        Serial.println("Stop");
        break;
    case 'e':
        enableDrivers();
        Serial.println("Drivers enabled");
        break;
    case 'd':
        disableDrivers();
        Serial.println("Drivers disabled");
        break;
    default:
        Serial.println("Unknown command");
        break;
    }
}
