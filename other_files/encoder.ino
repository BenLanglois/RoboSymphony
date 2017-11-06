#include "definitions.h"


void update_encoder(Motor *m) {
  int enca_update_read = digitalRead(m->enca);
  if (m->enca_prev == LOW && enca_update_read == HIGH) {
    if (digitalRead(m->encb) == HIGH) m->encoder_reading++;
    else m->encoder_reading--;
  }
  m->enca_prev = enca_update_read;
}
