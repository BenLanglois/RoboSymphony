#include <PID_v1.h>

#define IDLE 0
#define DRIVE 1

// define pins
// Left motor
#define IN1 14
#define IN2 15
#define ENABLE1 16
#define ENCLA 0
#define ENCLB 0
// Right motor
#define IN3 18
#define IN4 19
#define ENABLE2 17
#define ENCRA 0
#define ENCRB 0

#define LED 13

const int tics_per_rotation = 360;
const double meters_per_rotation = 0.251327412;
double target_RPM = ((double)4.5)/meters_per_rotation;

enum motor_state {
    FORWARD,
    BACK,
    COAST,
    BRAKE
};

struct motor {
    int in1, in2, en, enca, encb;
    double speed;
    int encoder_reading, last_enc_reading;
    int enca_prev, encb_prev;
    int last_enc_time;
};

typedef struct motor Motor;

/***********************

         HEATSINK

    | EN1         EN2
    | IN1 IN2 IN3 IN4

***********************/

//#include "../run_files/idle.cpp"
//#include "../run_files/drive.cpp"
//#include "../run_files/sensors.cpp"

void set_motor_state(Motor m, motor_state dir);
void set_motor_power(Motor m, int rate);
double get_motor_speed(Motor m);
void update_encoder(Motor m);
void set_motor_pins(Motor m); 

Motor left;
Motor right;

int kp = 0, ki = 0, kd = 0;

int target_speed;

double left_motor_power = 0, right_motor_power = 0;

//Create PID controllers
PID left_PID(&left.speed, &left_motor_power, &target_RPM, kp, ki, kd, DIRECT);
PID right_PID(&right.speed, &right_motor_power, &target_RPM, kp, ki, kd, DIRECT);

void setup() {
    //bool state = IDLE;
    // pinMode(GRAYSCALE_SENSOR, INPUT);
    // pinMode(LEFT_LINE_FOLLOWER, INPUT);
    // pinMode(RIGHT_LINE_FOLLOWER, INPUT);

    // pinMode(LEFT_MOTOR, OUTPUT);
    // pinMode(RIGHT_MOTOR, OUTPUT);
    // pinMode(SPEAKER, OUTPUT);

    //Setup our motors
    left.in1 = IN1;
    left.in2 = IN2;
    left.en = ENABLE1;
    left.enca = ENCLA;
    left.encb = ENCLB;
    left.speed = 0;
    left.encoder_reading = left.last_enc_reading = 0;
    left.enca_prev = left.encb_prev = LOW;
    left.last_enc_time = 0; 

    right.in1 = IN3;
    right.in2 = IN4;
    right.en = ENABLE2;
    right.enca = ENCRA;
    right.encb = ENCRB;
    right.speed = 0;
    right.encoder_reading = right.last_enc_reading = 0; 
    right.enca_prev = right.encb_prev = LOW;
    right.last_enc_time = 0;

    //Initialize Serial communcation
    Serial.begin(9600);

    //Set pin modes
    set_motor_pins(right);
    set_motor_pins(left);
    pinMode(LED, OUTPUT);

    //Set motor states
    set_motor_state(right, FORWARD);
    set_motor_state(left, FORWARD);

    //Enable PID controllers
    left_PID.SetMode(AUTOMATIC);
    right_PID.SetMode(AUTOMATIC);
}


void loop() {

    //Update the motor states
    update_encoder(left);
    update_encoder(right);
    update_motor_speed(left);
    update_motor_speed(right);

    //Calculate motor power
    left_PID.Compute();
    right_PID.Compute();

    //Update motor output power
    set_motor_power(left, left_motor_power);
    set_motor_power(right, right_motor_power);

    Serial.printf("%d\t%d\t%d\n", target_RPM, left.speed, right.speed);


    /*if ((millis() / 1000) % 3) {
        set_motor_power(right, 255);
        set_motor_power(left, 255);
        digitalWrite(LED, LOW);
    } else {
        set_motor_power(right, 100);
        set_motor_power(left, 100);
        digitalWrite(LED, HIGH);
    }*/

    /*if (state == IDLE) idle();
    else if (state == DRIVE) drive();*/
}

void set_motor_pins(Motor m) {
    pinMode(m.in1, OUTPUT);
    pinMode(m.in2, OUTPUT);
    pinMode(m.en, OUTPUT);
    pinMode(m.enca, OUTPUT);
    pinMode(m.encb, OUTPUT);
}

void set_motor_state(Motor m, motor_state dir) {
    int pin1 = m.in1;
    int pin2 = m.in2;
    if (dir == FORWARD) {
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
    } else if (dir == REVERSE) {
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH);
    } else if (dir == COAST) {
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW);
    } else if (dir == BRAKE) {
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, HIGH);
    }
}

void set_motor_power(Motor m, int rate) {
    analogWrite(m.en, rate);
}

int curr_t;
int delta_t;
int delta_r;
double speed;
void update_motor_speed(Motor m) {
    curr_t = millis();
    delta_t = curr_t - m.last_enc_time;
    m.last_enc_time = curr_t;
    delta_r = m.encoder_reading - m.last_enc_reading;
    if(delta_t != 0) speed = ((double)delta_r)/((double)delta_t);
    m.speed = speed * (double)60000 / (double)tics_per_rotation;
}

int enca_update_read;
int encb_update_read;
void update_encoder(Motor m) {
    enca_update_read = digitalRead(m.enca);
    encb_update_read = digitalRead(m.encb);
    m.last_enc_reading = m.encoder_reading;
    if(m.enca_prev == LOW && enca_update_read == HIGH) {
        m.encoder_reading += enca_update_read == HIGH ? 1 : -1;
    }
    m.enca_prev = enca_update_read;
    m.encb_prev = encb_update_read;
}
