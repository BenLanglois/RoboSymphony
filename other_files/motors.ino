

/*enum motor_state {
  FORWARD,
  BACK,
  COAST,
  BRAKE
};*/

//void set_motor_state(Motor m, motor_state dir);




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
