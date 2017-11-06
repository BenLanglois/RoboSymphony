// https://www.arduino.cc/en/Tutorial/toneMelody

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
