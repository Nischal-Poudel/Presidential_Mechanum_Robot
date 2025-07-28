#include <Arduino.h>

//----------------- Front Left Motor Pins -----------------
#define FRONT_LEFT_F 19  // Was: B_IN3 - Forward Trigger
#define FRONT_LEFT_R 21  // Was: B_IN4 - Reverse Trigger
#define FRONT_LEFT_EN 23 // Was: B_ENB (Front Left Motor Enable/PWM)

//----------------- Front Right Motor Pins -----------------
#define FRONT_RIGHT_F 33  // Was: B_IN2
#define FRONT_RIGHT_R 32  // Was: B_IN1
#define FRONT_RIGHT_EN 22 // Was: B_ENA (Front Right Motor Enable/PWM)

//----------------- Rear Left Motor Pins -----------------
#define REAR_LEFT_F 14  // Was: B_IN1 - Forward Trigger
#define REAR_LEFT_R 27  // Was: B_IN2 - Reverse Trigger
#define REAR_LEFT_EN 25 // Was: B_EN1 (Rear Left Motor Enable/PWM)

//----------------- Rear Right Motor Pins -----------------
#define REAR_RIGHT_F 5  // Was: B_IN3 -Forward Trigger
#define REAR_RIGHT_R 13   // Was: B_IN4 -Reverse Trigger
#define REAR_RIGHT_EN 26 // Was: B_EN2 (Rear Right Motor Enable/PWM)

// ===================== PWM Channel Definitions =====================
#define PWM_FRONT_LEFT_CH 0  // Was: PWM_A_EN1_CH
#define PWM_FRONT_RIGHT_CH 1 // Was: PWM_A_EN2_CH
#define PWM_REAR_LEFT_CH 2   // Was: PWM_B_EN1_CH
#define PWM_REAR_RIGHT_CH 4  // Was: PWM_B_EN2_CH

#define PWM_FREQ 5000
#define LEDC_TIMER_12_BIT 12 // 12-bit PWM (0-4095)

// ===================== Motor Control Functions =====================

// Set direction for a motor using its Forward/Reverse pins
void setMotor(int forwardPin, int reversePin, bool forward)
{
    if (forward)
    {
        digitalWrite(forwardPin, LOW);
        digitalWrite(reversePin, HIGH);
    }
    else
    {
        digitalWrite(forwardPin, HIGH);
        digitalWrite(reversePin, LOW);
    }
}

// Stop a motor (both pins HIGH)
void stopMotor(int forwardPin, int reversePin)
{
    digitalWrite(forwardPin, HIGH);
    digitalWrite(reversePin, HIGH);
}

// Set PWM speed for a motor (0-4095 for 12-bit)
void setMotorSpeed(int pwmChannel, uint32_t speed)
{
    ledcWrite(pwmChannel, speed);
}

// ===================== Independent Motor Control =====================

// Front Left Motor control
void setFrontLeft(bool forward, uint32_t speed)
{
    setMotor(FRONT_LEFT_F, FRONT_LEFT_R, forward);
    setMotorSpeed(PWM_FRONT_LEFT_CH, speed);
}

// Front Right Motor control
void setFrontRight(bool forward, uint32_t speed)
{
    setMotor(FRONT_RIGHT_F, FRONT_RIGHT_R, forward);
    setMotorSpeed(PWM_FRONT_RIGHT_CH, speed);
}

// Rear Left Motor control
void setRearLeft(bool forward, uint32_t speed)
{
    setMotor(REAR_LEFT_F, REAR_LEFT_R, forward);
    setMotorSpeed(PWM_REAR_LEFT_CH, speed);
}

// Rear Right Motor control
void setRearRight(bool forward, uint32_t speed)
{
    setMotor(REAR_RIGHT_F, REAR_RIGHT_R, forward);
    setMotorSpeed(PWM_REAR_RIGHT_CH, speed);
}

// ===================== Group Motor Control (for convenience) =====================

void moveForward(uint32_t speed = 0)
{
    setFrontLeft(true, speed);
    setFrontRight(true, speed);
    setRearLeft(true, speed);
    setRearRight(true, speed);
}

void moveBackward(uint32_t speed = 0)
{
    setFrontLeft(false, speed);
    setFrontRight(false, speed);
    setRearLeft(false, speed);
    setRearRight(false, speed);
}

void turnLeft(uint32_t speed = 0)
{
    setFrontLeft(false, speed);
    setRearLeft(false, speed);
    setFrontRight(true, speed);
    setRearRight(true, speed);
}

void turnRight(uint32_t speed = 0)
{
    setFrontLeft(true, speed);
    setRearLeft(true, speed);
    setFrontRight(false, speed);
    setRearRight(false, speed);
}

void stopAll()
{
    stopMotor(FRONT_LEFT_F, FRONT_LEFT_R);
    stopMotor(FRONT_RIGHT_F, FRONT_RIGHT_R);
    stopMotor(REAR_LEFT_F, REAR_LEFT_R);
    stopMotor(REAR_RIGHT_F, REAR_RIGHT_R);

    setMotorSpeed(PWM_FRONT_LEFT_CH, 0);
    setMotorSpeed(PWM_FRONT_RIGHT_CH, 0);
    setMotorSpeed(PWM_REAR_LEFT_CH, 0);
    setMotorSpeed(PWM_REAR_RIGHT_CH, 0);
}

void enableDrivers()
{
    setMotorSpeed(PWM_FRONT_LEFT_CH, 4095);
    setMotorSpeed(PWM_FRONT_RIGHT_CH, 4095);
    setMotorSpeed(PWM_REAR_LEFT_CH, 4095);
    setMotorSpeed(PWM_REAR_RIGHT_CH, 4095);
}

void disableDrivers()
{
    setMotorSpeed(PWM_FRONT_LEFT_CH, 0);
    setMotorSpeed(PWM_FRONT_RIGHT_CH, 0);
    setMotorSpeed(PWM_REAR_LEFT_CH, 0);
    setMotorSpeed(PWM_REAR_RIGHT_CH, 0);
}

// ===================== Initialization =====================

void iniMotorDriver()
{
    pinMode(FRONT_LEFT_F, OUTPUT_OPEN_DRAIN);  // Was: A_IN1
    pinMode(FRONT_LEFT_R, OUTPUT_OPEN_DRAIN);  // Was: A_IN2
    pinMode(FRONT_RIGHT_F, OUTPUT_OPEN_DRAIN); // Was: A_IN3
    pinMode(FRONT_RIGHT_R, OUTPUT_OPEN_DRAIN); // Was: A_IN4
    pinMode(REAR_LEFT_F, OUTPUT_OPEN_DRAIN);   // Was: B_IN1
    pinMode(REAR_LEFT_R, OUTPUT_OPEN_DRAIN);   // Was: B_IN2
    pinMode(REAR_RIGHT_F, OUTPUT_OPEN_DRAIN);  // Was: B_IN3
    pinMode(REAR_RIGHT_R, OUTPUT_OPEN_DRAIN);  // Was: B_IN4

    pinMode(FRONT_LEFT_EN, OUTPUT);  // Was: A_EN1
    pinMode(FRONT_RIGHT_EN, OUTPUT); // Was: A_EN2
    pinMode(REAR_LEFT_EN, OUTPUT);   // Was: B_EN1
    pinMode(REAR_RIGHT_EN, OUTPUT);  // Was: B_EN2

    ledcSetup(PWM_FRONT_LEFT_CH, PWM_FREQ, LEDC_TIMER_12_BIT);
    ledcAttachPin(FRONT_LEFT_EN, PWM_FRONT_LEFT_CH);

    ledcSetup(PWM_FRONT_RIGHT_CH, PWM_FREQ, LEDC_TIMER_12_BIT);
    ledcAttachPin(FRONT_RIGHT_EN, PWM_FRONT_RIGHT_CH);

    ledcSetup(PWM_REAR_LEFT_CH, PWM_FREQ, LEDC_TIMER_12_BIT);
    ledcAttachPin(REAR_LEFT_EN, PWM_REAR_LEFT_CH);

    ledcSetup(PWM_REAR_RIGHT_CH, PWM_FREQ, LEDC_TIMER_12_BIT);
    ledcAttachPin(REAR_RIGHT_EN, PWM_REAR_RIGHT_CH);

    stopAll();
}
