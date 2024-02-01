#include <Arduino.h>

const int sensorPins[] = {A0, A1, A2, A3, A4}; // ACS712 센서 핀
const int numSensors = 5; // 센서의 개수
unsigned long previousMillis = 0; // 마지막으로 센서 값을 읽은 시간
const long interval = 1; // 센서 값을 읽는 주기 (밀리초)

const float ADC_zero = 515.0; // 0A에 해당하는 ADC 값
const float ADC_max = 635.0; // 8.584A에 해당하는 ADC 값
const float current_max = 8.584; // 최대 전류 값
const float k = current_max / (ADC_max - ADC_zero); // 비례 상수

void setup() {
  Serial.begin(9600); // 시리얼 통신 시작
  for (int i = 0; i < numSensors; i++) {
    pinMode(sensorPins[i], INPUT); // 센서 핀을 입력으로 설정
  }
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    for (int i = 0; i < numSensors; i++) {
      int sensorValue = analogRead(sensorPins[i]); // 센서 값 읽기
      float current = k * (sensorValue - ADC_zero); // 실제 전류 값으로 변환
      Serial.print("Sensor ");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(current, 3); // 변환된 전류 값 출력 (소수점 셋째자리까지)
      Serial.print(" A | ");
    }
    Serial.println(); // 줄바꿈으로 센서 값들을 구분
  }
}
