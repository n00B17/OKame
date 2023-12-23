#ifndef minikame_h
#define minikame_h

//#include <Servo.h>
#include <ESP32Servo.h>
#include <Octosnake.h>

class MiniKame{
public:
    void init();
    void run(float steps, int period);
    void walk(float steps, int period);
    void walkBack(float steps, int period);
    void omniWalk(float steps, int T, bool side, float turn_factor);
    //void backward(float steps, int period);
    void turnL(float steps, int period);
    void turnR(float steps, int period);
    void moonwalkL(float steps, int period);
    void dance(float steps, int period);
    void upDown(float steps, int period);
    void pushUp(float steps, int period);
    void hello();
    void jump();
    void home();
    void zero();
    void frontBack(float steps, int period);

    void lowarmUp();
    void uparmUp();
    void clawClose();
    void lowarmDown();
    void uparmDown();
    void clawOpen();

    void setServo(int id, float target);
    void setArm(int id, float target);
    void reverseServo(int id);
    float getServo(int id);
    float getArm(int id);
    void moveServos(int time, float target[8]);
    void moveArm(int time, float target[3]);

private:
    Oscillator oscillator[8];
    Oscillator arm_oscillator[8];
    Servo servo[8];
    Servo arm[3];
    int board_pins[8];
    int arm_pins[3];
    int trim[8];
    int arm_trim[3];
    bool reverse[8];
    bool arm_reverse[3];
    unsigned long _init_time;
    unsigned long _final_time;
    unsigned long _partial_time;
    float _increment[8];
    float _arm_increment[3];
    float _servo_position[8];
    float _arm_position[3];
    
    int armTest[3] ; 

    int angToUsec(float value);
    void execute(float steps, int period[8], int amplitude[8], int offset[8], int phase[8]);
};

#endif
