// DEFINE THESE!!!
#define LEFT_SENSOR 1
#define RIGHT_SENSOR 2
#define NOTE_SENSOR 3


sensor left_sensor = {LEFT_SENSOR, 0};
sensor right_sensor = {RIGHT_SENSOR, 0};
sensor note_sensor = {NOTE_SENSOR, 0};



typedef struct {
    int port, reading;
} sensor;

void read_sensor(sensor s) {
    s.reading = analogRead(s.port);
}
