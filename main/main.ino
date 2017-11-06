#include <PID_v1.h>
#include <assert.h>
#include <math.h>
using namespace std;

#define LED 13

const int tics_per_rotation = 360;
const double meters_per_rotation = 0.251327412;
double target_RPM = ((double)4.5) / meters_per_rotation;


/*
void set_motor_power(Motor m, int rate);
double get_motor_speed(Motor m);
void update_encoder(Motor m);
void set_motor_pins(Motor m);
*/


int curr_t, delta_t, delta_r;
double speed;


int kp = 0, ki = 0, kd = 0;
int target_speed;
double left_motor_power = 0, right_motor_power = 0;

//Create PID controllers
PID left_PID(&left.speed, &left_motor_power, &target_RPM, kp, ki, kd, DIRECT);
PID right_PID(&right.speed, &right_motor_power, &target_RPM, kp, ki, kd, DIRECT);

void update_motor_speed(Motor m);


void setup() {
  //bool state = IDLE;
  // pinMode(GRAYSCALE_SENSOR, INPUT);
  // pinMode(LEFT_LINE_FOLLOWER, INPUT);
  // pinMode(RIGHT_LINE_FOLLOWER, INPUT);

  // pinMode(LEFT_MOTOR, OUTPUT);
  // pinMode(RIGHT_MOTOR, OUTPUT);
  // pinMode(SPEAKER, OUTPUT);

  //Setup our motors
  left.en = ENABLE1;
  left.enca = ENCLA;
  left.encb = ENCLB;
  left.speed = 0;
  left.encoder_reading = left.last_enc_reading = 0;
  left.enca_prev = LOW;
  left.last_enc_time = 0;

  right.en = ENABLE2;
  right.enca = ENCRA;
  right.encb = ENCRB;
  right.speed = 0;
  right.encoder_reading = right.last_enc_reading = 0;
  right.enca_prev = LOW;
  right.last_enc_time = 0;

  //Initialize Serial communcation
  Serial.begin(9600);

  //Set pin modes
  set_motor_pins(right);
  set_motor_pins(left);
  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);
  pinMode(NOTE_SENSOR, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(IN_1_3, OUTPUT);
  pinMode(IN_2_4, OUTPUT);

  //Set motor states
  digitalWrite(IN_1_3, LOW);
  digitalWrite(IN_2_4, HIGH);

  //Enable PID controllers
  //left_PID.SetMode(AUTOMATIC);
  //right_PID.SetMode(AUTOMATIC);
}


void loop() {

  //Update the motor states
  update_encoder(&left);
  update_encoder(&right);
  /*update_motor_speed(left);
    update_motor_speed(right);

    // Update sensor readings
    //read_sensor(note_sensor);

    //Calculate motor power
    left_PID.Compute();
    right_PID.Compute();

    follow_line();

    //Update motor output power
    set_motor_power(left, left_motor_power - right_turn_ratio);
    set_motor_power(right, right_motor_power + right_turn_ratio);

    Serial.printf("Target: %d\tLeft: %d\tRight: %d\n", target_RPM, left.speed, right.speed);
  */

  if ((millis() / 1000) % 2) {
    digitalWrite(LED, LOW);
  } else {
    digitalWrite(LED, HIGH);
  }
}



void update_motor_speed(Motor m) {
  curr_t = millis();
  delta_t = curr_t - m.last_enc_time;
  m.last_enc_time = curr_t;
  delta_r = m.encoder_reading - m.last_enc_reading;
  if (delta_t != 0) speed = ((double)delta_r) / ((double)delta_t);
  m.speed = speed * (double)60000 / (double)tics_per_rotation;
}

