const int lMotFwd = 4;  // pin 9 nối với motor trái
const int rMotFwd = 3; // pin 9 nối với motor phải
const int lMotRev = 5; // pin 9 nối với motor trái
const int rMotRev = 2; // pin 9 nối với motor phải
const int enA = 10;
const int enB = 9;

int rightSpeed;
int leftSpeed;

unsigned long previousTime = 0;
unsigned long currentTime;
float elapsedTime;



int P, I, D;
float Kp = 3.1, Kd = 0.28 , Ki = 5;
float PID;
int curr_error, prev_error;
int base_speed = 40;  // tốc độ gốc

void setup() {
  
  pinMode(A0, INPUT);  //phai nhât
  pinMode(A1, INPUT);  //phai
  pinMode(A2, INPUT);  // giữa
  pinMode(A3, INPUT);  // trai
  pinMode(A4, INPUT);  // trai nhất
  
  Serial.begin(9600);
  
  
  pinMode(lMotFwd, OUTPUT); 
  pinMode(rMotFwd, OUTPUT);
  pinMode(lMotRev, OUTPUT);  
  pinMode(rMotRev, OUTPUT); 
  pinMode(enA, OUTPUT);  
  pinMode(enB, OUTPUT); 
}


void moveForward(float PID) {
  leftSpeed = base_speed - PID;
  rightSpeed = base_speed + PID;

  digitalWrite(lMotFwd, HIGH);
  digitalWrite(rMotFwd, HIGH);
  digitalWrite(rMotRev, LOW);
  digitalWrite(lMotRev, LOW);


  analogWrite(enA, rightSpeed);
  analogWrite(enB, leftSpeed);
}


void Stop() {
  digitalWrite(lMotFwd, LOW);
  digitalWrite(rMotFwd, LOW);
  digitalWrite(lMotRev, LOW);
  digitalWrite(rMotRev, LOW);
  analogWrite(enA, 0);
  analogWrite(enB, 0); 
  delay(10000);
}

int readError() {
  int sensor1 = digitalRead(A0); 
  int sensor2 = digitalRead(A1); 
  int sensor3 = digitalRead(A2); 
  int sensor4 = digitalRead(A3);  
  int sensor5 = digitalRead(A4); 

  if (sensor1 == LOW && sensor2 == HIGH && sensor3 == HIGH && sensor4 == HIGH && sensor5 == HIGH) {
    return -4;
  } else if (sensor1 == LOW && sensor2 == LOW && sensor3 == HIGH && sensor4 == HIGH && sensor5 == HIGH) {
    return -3;
  } else if (sensor1 == HIGH && sensor2 == LOW && sensor3 == HIGH && sensor4 == HIGH && sensor5 == HIGH) {
    return -2;
  } else if (sensor1 == HIGH && sensor2 == LOW && sensor3 == LOW && sensor4 == HIGH && sensor5 == HIGH) {
    return -1;
  } else if (sensor1 == HIGH && sensor2 == HIGH && sensor3 == LOW && sensor4 == HIGH && sensor5 == HIGH) {
    return 0;
  } else if (sensor1 == HIGH && sensor2 == HIGH && sensor3 == LOW && sensor4 == LOW && sensor5 == HIGH) {
    return 1;
  } else if (sensor1 == HIGH && sensor2 == HIGH && sensor3 == HIGH && sensor4 == LOW && sensor5 == HIGH) {
    return 2;
  } else if (sensor1 == HIGH && sensor2 == HIGH && sensor3 == HIGH && sensor4 == LOW && sensor5 == LOW) {
    return 3;
  } else if (sensor1 == HIGH && sensor2 == HIGH && sensor3 == HIGH && sensor4 == HIGH && sensor5 == LOW) {
    return 4;
  }else if(sensor1 == LOW && sensor2 == LOW && sensor3 == LOW && sensor4 == LOW && sensor5 == LOW){
    Stop();
    return 0;
  } else {
    return 0;
  }
}

float calculatePID() {
  currentTime = millis();
  elapsedTime = (currentTime - previousTime) / 1000.0;  // Convert to seconds
  previousTime = currentTime;

  curr_error = readError();
  P = curr_error;
  I += curr_error * elapsedTime;  // Integral term with time factor
  D = (curr_error - prev_error) / elapsedTime;  // Derivative term with time factor
  prev_error = curr_error;



  return P * Kp + I * Ki + D * Kd;
}

void loop() {
  PID = calculatePID();
  delay(10);
  //Serial.println(leftSpeed);
  //Serial.println(rightSpeed);
  moveForward(PID);
}