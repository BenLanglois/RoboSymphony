struct Motor {
  int in1, in2, en, enca, encb;
  double speed;
  int encoder_reading, last_enc_reading;
  int enca_prev;
  unsigned long last_enc_time;
};

struct sensor {
    int port, reading;
};
