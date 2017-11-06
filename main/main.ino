#include <PID_v1.h>
#include <assert.h>
#include <math.h>
using namespace std;

#include "structs.h"





// Speaker
int dir = 1, extension = 0;

void update_speaker(int increment) {
  extension += dir * increment;
  if (extension >= 255) {
    dir = -1;
    extension = 255;
  }
  else if (extension <= 0) {
    dir = 1;
    extension = 0;
  }

  PORTD = extension;
}



// Motors

// Both motors
#define IN_1_3 14
#define IN_2_4 15
// Left motor
#define ENABLE1 17
#define ENCLA 9
#define ENCLB 10
// Right motor
#define ENABLE2 16
#define ENCRA 11
#define ENCRB 12



Motor left;
Motor right;

void set_motor_pins(Motor m) {
  pinMode(m.en, OUTPUT);
  pinMode(m.enca, INPUT);
  pinMode(m.encb, INPUT);
}

void set_motor_power(Motor m, int rate) {
  analogWrite(m.en, rate);
}



// Sine Wave

int sin_table[65];


void calculate_sin_table() {
    for (int i = 0; i <= 64; i++)
    if (i == 64) sin_table[i] = 127;
    else sin_table[i] = (int)(128 * sin(M_PI * i / 128));
}

int custom_sin(int theta) {
    assert(0 <= theta && theta < 256);
    if (0 <= theta && theta < 64) return 128 + sin_table[theta];
    else if (64 <= theta && theta < 128) return 128 + sin_table[128 - theta];
    else if (128 <= theta && theta < 192) return 128 - sin_table[theta - 128];
    else if (192 <= theta && theta < 256) return 128 - sin_table[256 - theta];
    assert(0);
}



// Encoder
void update_encoder(Motor *m) {
  int enca_update_read = digitalRead(m->enca);
  if (m->enca_prev == LOW && enca_update_read == HIGH) {
    if (digitalRead(m->encb) == HIGH) m->encoder_reading++;
    else m->encoder_reading--;
  }
  m->enca_prev = enca_update_read;
}


// Sensors

sensor left_sensor = {A5, 0};
sensor right_sensor = {A7, 0};
sensor note_sensor = {A6, 0};


void read_sensor(sensor *s) {
    s->reading = analogRead(s->port);
}


// Follow Line

enum drift {
  RIGHT,
  LEFT,
  NONE
};

int right_turn_ratio = 0;
drift current_drift = NONE;

void follow_line() {
    read_sensor(&left_sensor);
    read_sensor(&right_sensor);

    // Has veered left
    if (left_sensor.reading > 250 /*&& right_sensor.reading < 200*/) { // Test values
        right_turn_ratio = 10; // Test value
        current_drift = LEFT;
    }
    // Has veered right
    else if (right_sensor.reading < 350 /*&& left_sensor.reading < 200*/) {
        right_turn_ratio = -5;
        current_drift = RIGHT;
    }
    /*
    // Was veering left, now is straight
    else if (current_drift == LEFT) {
        current_drift = NONE;
        // Correct the turn
        right_turn_ratio = -5; // Test value
    }
    // Was veering right, now is straight
    else if (current_drift == RIGHT) {
        current_drift = NONE;
        // Correct the turn
        right_turn_ratio = 5;
    }
    // Was straight last time, still is straight
    else {
        // finish correcting (if it was correcting at all)
        if (right_turn_ratio > 0) right_turn_ratio -= 1;
        if (right_turn_ratio < 0) right_turn_ratio += 1;
    }
  */
  else right_turn_ratio = 0;

}



// PID

const int tics_per_rotation = 360;
const double meters_per_rotation = 0.251327412;
double target_RPM = ((double)4.5) / meters_per_rotation;

int curr_t, delta_t, delta_r;
double speed;

int kp = 0, ki = 0, kd = 0;
int target_speed;
double left_motor_power = 0, right_motor_power = 0;

//Create PID controllers
PID left_PID(&left.speed, &left_motor_power, &target_RPM, kp, ki, kd, DIRECT);
PID right_PID(&right.speed, &right_motor_power, &target_RPM, kp, ki, kd, DIRECT);

void update_motor_speed(Motor m) {
  curr_t = millis();
  delta_t = curr_t - m.last_enc_time;
  m.last_enc_time = curr_t;
  delta_r = m.encoder_reading - m.last_enc_reading;
  if (delta_t != 0) speed = ((double)delta_r) / ((double)delta_t);
  m.speed = speed * (double)60000 / (double)tics_per_rotation;
}

















#define LED 13

void setup() {
  //bool state = IDLE;
  pinMode(note_sensor.port, INPUT);
  pinMode(left_sensor.port, INPUT);
  pinMode(right_sensor.port, INPUT);

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
  pinMode(left_sensor.port, INPUT);
  pinMode(right_sensor.port, INPUT);
  pinMode(note_sensor.port, INPUT);
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
  /*
  //Update the encoders
  update_encoder(&left);
  update_encoder(&right);
  */
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
  follow_line();

  set_motor_power(left, 35 + right_turn_ratio);
  set_motor_power(right, 30 - right_turn_ratio);

  Serial.printf("%d, %d, %d, %d\n", 250, left_sensor.reading, 350, right_sensor.reading);


  delay(50);
  /*if ((millis() / 1000) % 2) {
    digitalWrite(LED, LOW);
  } else {
    digitalWrite(LED, HIGH);
  }*/

  
}
