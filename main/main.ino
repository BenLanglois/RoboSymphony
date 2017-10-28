#define IDLE 0
#define DRIVE 1
// define pins
#define IN1 16
#define IN2 17
#define ENABLE1 20
#define LED 13

//#include "../run_files/idle.cpp"
//#include "../run_files/drive.cpp"
//#include "../run_files/sensors.cpp"

void set_motor_dir(int pin1, int pin2, int dir);
void motor_speed(int pin, int rate);


void setup() {
    //bool state = IDLE;
    // pinMode(GRAYSCALE_SENSOR, INPUT);
    // pinMode(LEFT_LINE_FOLLOWER, INPUT);
    // pinMode(RIGHT_LINE_FOLLOWER, INPUT);

    // pinMode(LEFT_MOTOR, OUTPUT);
    // pinMode(RIGHT_MOTOR, OUTPUT);
    // pinMode(SPEAKER, OUTPUT);



    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(LED, OUTPUT);
    pinMode(ENABLE1, OUTPUT);
    set_motor_dir(IN1, IN2, 1);
}

void loop() {

    if ((millis() / 1000) % 3) {
        motor_speed(ENABLE1, 255);
        digitalWrite(LED, HIGH);
    } else {
        motor_speed(ENABLE1, 63);
        digitalWrite(LED, LOW);
    }

    /*if (state == IDLE) idle();
    else if (state == DRIVE) drive();*/
}





void set_motor_dir(int pin1, int pin2, int dir) {
    if (dir) {
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
    } else {
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH);
    }
}

void motor_speed(int pin, int rate) {
    analogWrite(pin, rate);
}
