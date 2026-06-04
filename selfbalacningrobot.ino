#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

//Motor driver 1 output wires
  #define DIR1 14
  #define STEP1 27

  //Motor driver 2 output wires
  #define DIR2 25
  #define STEP2 26


Adafruit_MPU6050 mpu;
float aBiasX;
float aBiasY;
float aBiasZ;

float gBiasX;
float gBiasY;
float gBiasZ;


void spinBothMotors(bool direction, int stepDelay) {

  digitalWrite(DIR1, direction);
  digitalWrite(DIR2, direction);

  digitalWrite(STEP1, HIGH);
  digitalWrite(STEP2, HIGH);
  delayMicroseconds(stepDelay);

  digitalWrite(STEP1, LOW);
  digitalWrite(STEP2, LOW);
  delayMicroseconds(stepDelay);
}

void spinMotor1(float gCorrectedY){ 
  if(gCorrectedY > 0.20) { 
    digitalWrite(DIR1, HIGH);

    digitalWrite(STEP1, HIGH);
    delayMicroseconds(800);
    digitalWrite(STEP1, LOW);
    delayMicroseconds(800);
  }

  else if(gCorrectedY < -0.20){
    digitalWrite(DIR1, LOW);

    digitalWrite(STEP1, HIGH);
    delayMicroseconds(800);
    digitalWrite(STEP1, LOW);
    delayMicroseconds(800);
  }
}

void spinMotor2(float gCorrectedY) { 
  if (gCorrectedY > 0.20) { 
    digitalWrite(DIR2, HIGH);

    digitalWrite(STEP2, HIGH);
    delayMicroseconds(800);
    digitalWrite(STEP2, LOW);
    delayMicroseconds(800);
  }

  else if (gCorrectedY < -0.20) {
    digitalWrite(DIR2, LOW);

    digitalWrite(STEP2, HIGH);
    delayMicroseconds(800);
    digitalWrite(STEP2, LOW);
    delayMicroseconds(800);
  }
}

void setup() {


Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  //Software error loop, sets points to 0
    //Corrected acceleration
  for(int i = 0; i < 2000; i++) { 
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    aBiasX += a.acceleration.x;
    aBiasY += a.acceleration.y;
    aBiasZ += a.acceleration.z;

  }

  aBiasX /= 2000;
  aBiasY /= 2000;
  aBiasZ /= 2000;
  
  //Corrected Rotation
  for(int i = 0; i < 2000; i++) { 
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    gBiasX += g.gyro.x;
    gBiasY += g.gyro.y;
    gBiasZ += g.gyro.z;
  }

  gBiasX /= 2000;
  gBiasY /= 2000;
  gBiasZ /= 2000;

  Serial.println("");
  
  


  //Motor driver 1
  pinMode(DIR1, OUTPUT);
  pinMode(STEP1, OUTPUT);

  //Motor driver 2
  pinMode(DIR2, OUTPUT);
  pinMode(STEP2, OUTPUT);




}

void loop() {

  sensors_event_t a, g, temp; 
  mpu.getEvent(&a, &g, &temp);

  float aCorrectedX = a.acceleration.x - aBiasX;
  float aCorrectedY = a.acceleration.y - aBiasY;
  float aCorrectedZ = a.acceleration.z;

  float gCorrectedX = g.gyro.x - gBiasX;
  float gCorrectedY = g.gyro.y - gBiasY;
  float gCorrectedZ = g.gyro.z - gBiasZ;

Serial.print("Acceleration X: ");
  Serial.print(aCorrectedX);
  Serial.print(", Y: ");
  Serial.print(aCorrectedY);
  Serial.print(", Z: ");
  Serial.print(aCorrectedZ);
  Serial.println(" m/s^2");

Serial.print("Rotation X: ");
  Serial.print(gCorrectedX);
  Serial.print(", Y: ");
  Serial.print(gCorrectedY);
  Serial.print(", Z: ");
  Serial.print(gCorrectedZ);
  Serial.println(" rad/s");

  
 
spinBothMotors(HIGH, 800);



}


