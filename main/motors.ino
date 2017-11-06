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

/*enum motor_state {
  FORWARD,
  BACK,
  COAST,
  BRAKE
};*/

//void set_motor_state(Motor m, motor_state dir);

typedef struct {
  int in1, in2, en, enca, encb;
  double speed;
  int encoder_reading, last_enc_reading;
  int enca_prev;
  int last_enc_time;
} Motor;



void set_motor_pins(Motor m) {
  pinMode(m.en, OUTPUT);
  pinMode(m.enca, INPUT);
  pinMode(m.encb, INPUT);
}

void set_motor_power(Motor m, int rate) {
  analogWrite(m.en, rate);
}


/***********************

         HEATSINK

    | EN1         EN2
    | IN1 IN2 IN3 IN4

***********************/



