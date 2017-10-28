unsigned long start_time[] = {0, 0}, stop_time[] = {0, 0}; // When the motors should start and stop pulsing
int current_time;
int side_pin[] = {LEFT_MOTOR, RIGHT_MOTOR};

void drive() { // run by main.c every loop

}

void set_motor_dir(pin1, pin2, dir) {
    if (dir) {
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
    } else {
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH);
    }
}

void motor_speed(pin, speed) {
    analogWrite(pin, speed);
}

/*void check_motors(int speed) { // checks when motors should be stopped or started
    current_time = millis()
    for (bool side = 0; side <= 1; side ++) { // For left and right motor
        if (current_time >= start_time[side]) { // Start the pulse
            digitalWrite(side_pin[side], HIGH);
            motor_pwm(side, speed);
        }
        if (current_time >= stop_time[side]) // Stop the pulse
            digitalWrite(side_pin[side], LOW);
    }
}

void motor_pwm(bool side, int speed) { // sets stop and start times for motors
    // speed is between 0 and 5 inclusive
    current_time = millis();
    stop_time[side] = current_time + speed;
    start_time[side] = current_time + 5;
}*/


// float turn_direction: turn left/right, determined by PID

// float speed

// Determine individual wheel's speed from turn_direction and speed
