/*#define SENSOR1 9

//int sensor_value;
//unsigned long int total = 0;
//unsigned int count = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(SENSOR1, INPUT);
  Serial.begin(9600);
}

void loop() {
  //total += analogRead(SENSOR1);
  //count += 1;
  //Serial.println(total / count);
  Serial.println(analogRead(SENSOR1));
  delay(100);
}*/


void read_sensor(sensor s) {
    s.reading = analogRead(s.port);
}
