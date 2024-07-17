#include <Arduino.h>

const int sensorPins[] = {A0, A1, A2, A3, A4}; // ACS712 센서 핀
const int numSensors = 5; // 센서의 개수
const int numSamples = 50; // 필터링에 사용할 샘플 수
unsigned long previousMillis = 0; // 마지막으로 센서 값을 읽은 시간
const long interval = 1; // 센서 값을 읽는 주기 (밀리초)

const float ADC_zero = 511.0; // 0A에 해당하는 ADC 값
float sensorOffsets[numSensors] = {0}; // 각 센서의 오프셋 저장

float sensorValues[numSensors][numSamples] = {0};
int currentIndex = 0;

void setup() {
  Serial.begin(115200); // 시리얼 통신 시작
  for (int i = 0; i < numSensors; i++) {
    pinMode(sensorPins[i], INPUT); // 센서 핀을 입력으로 설정
  }
  calibrateSensors(); // 센서 오프셋 보정 실행
}

void calibrateSensors() {
  for (int i = 0; i < numSensors; i++) {
    float sum = 0;
    for (int j = 0; j < 10; j++) {
      sum += analogRead(sensorPins[i]);
      delay(10);
    }
    sensorOffsets[i] = sum / 10 - ADC_zero; // 오프셋 계산
  }
}

float calculateMovingAverage(int sensorIndex) {
  float sum = 0.0;
  for (int i = 0; i < numSamples; i++) {
    sum += sensorValues[sensorIndex][i];
  }
  return sum / numSamples;
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    for (int i = 0; i < numSensors; i++) {
      int sensorValue = analogRead(sensorPins[i]);
      float correctedValue = sensorValue - ADC_zero - sensorOffsets[i]; // 오프셋 보정 적용
      float current = (correctedValue * 5.0 / 1024.0) / 0.066 * 1000;

      sensorValues[i][currentIndex] = current;
      float averageCurrent = calculateMovingAverage(i);

      Serial.print(averageCurrent, 0);
      Serial.print("@");
    }
    Serial.println();
    currentIndex = (currentIndex + 1) % numSamples;
  }
}


