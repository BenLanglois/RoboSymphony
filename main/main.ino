#define IDLE 0
#define DRIVE 1
// define pins

#include "../run_files/idle.cpp"
#include "../run_files/drive.cpp"
#include "../run_files/sensors.cpp"

void setup() {
    bool state = IDLE;
    // pinMode(GRAYSCALE_SENSOR, INPUT);
    // pinMode(LEFT_LINE_FOLLOWER, INPUT);
    // pinMode(RIGHT_LINE_FOLLOWER, INPUT);

    // pinMode(LEFT_MOTOR, OUTPUT);
    // pinMode(RIGHT_MOTOR, OUTPUT);
    // pinMode(SPEAKER, OUTPUT);
}

void loop() {
    if (state == IDLE) idle();
    else if (state == DRIVE) drive();
}
