// https://www.arduino.cc/en/Tutorial/toneMelody

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
