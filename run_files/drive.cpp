

void follow_line() {
    read_sensor(left_sensor);
    read_sensor(right_sensor);

    // Has veered left
    if (left_sensor.reading > 150 && right_sensor.reading < 200) { // Test values
        right_turn_ratio = 5; // Test value
        current_drift = LEFT;
    }
    // Has veered right
    else if (right_sensor.reading > 150 && left_sensor.reading < 200) {
        right_turn_ratio = -5;
        current_drift = RIGHT;
    }
    // Was veering left, now is straight
    else if (current_drift == LEFT) {
        current_drift = NONE;
        // Correct the turn
        right_turn_ratio = -10; // Test value
    }
    // Was veering right, now is straight
    else if (current_drift == RIGHT) {
        current_drift = NONE;
        // Correct the turn
        right_turn_ratio = 10;
    }
    // Was straight last time, still is straight
    else {
        // finish correcting (if it was correcting at all)
        if (right_turn_ratio > 0) right_turn_ratio -= 1;
        if (right_turn_ratio < 0) right_turn_ratio += 1;
    }


}
