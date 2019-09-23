#define WAIT 1
#define ROTATE 2
#define POST_STOP 3
#define STOP 9

int val1 = 0, pre1 = 0, diff1 = 0, pdiff1 = 0;
int val2 = 0, pre2 = 0, diff2 = 0, pdiff2 = 0;
int switch1 = 0, pre_switch1 = 0;
boolean is_start = false;
int threshold = 800;
int gyaku_start = 0, jun_start = 0;
unsigned long start_time = 0;
int mode = STOP;

char str;


void setup() {

  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(7, INPUT_PULLUP);
  Serial.begin(9600);

}

void loop() {

  if (Serial.available() > 0) {
    //データの読み込み
    //str = Serial.readStringUntil('\r');
    str = Serial.read();
    Serial.println(str);
    if (str == 'a') {
      mode = WAIT;
      start_time = millis();
    } else if (str == 'b') {
      mode = POST_STOP;
    } else if (str == 'c') {
      mode = STOP;
    }
  }

  if (mode == WAIT) {
    is_start = isStart();
    Serial.println(is_start);
    if (is_start) {
      mode = ROTATE;
      start_time = millis();
      Serial.println("Start to rotate.");
    }
  } else if (mode == ROTATE) {
    // モータ駆動
    driveMotor(true);
    Serial.println(millis() - start_time);

    if (millis() - start_time > 10000) {
      Serial.println("auto stop.");
      mode = STOP;
    }
  } else if (mode == POST_STOP) {
    // モータ駆動
    driveMotor(false);

  } else {
    stopMotor();
    Serial.println("stop!");
  }

  delay(100);

}

void stopMotor()
{
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  analogWrite(5, 0);
}

void driveMotor(boolean flag)
{
  if (flag == true) {
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
  }
  else {
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
  }

  analogWrite(5, 250);
}

boolean isStart()
{
  boolean dst = false;

  switch1 = digitalRead(7);
  if (pre_switch1 == 0 && switch1 == 1) {
    dst = true;
  }
  pre_switch1 = switch1;

  return dst;
}
