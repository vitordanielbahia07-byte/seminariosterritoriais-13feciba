#include <Servo.h>

// **********************************
// *** DECLARAÇÃO DE VARIÁVEIS E OBJETOS ***
// **********************************
Servo servoMotor;

const int SERVO_PIN = 8;

// **********************************
// *** CONSTANTES DE VELOCIDADE E SUAVIZAÇÃO ***
// **********************************
const int DELAY_MOVIMENTO = 50;   // Velocidade
const int PASSO_ANGULAR = 1;      // Suavização

int currentPosition = 90;         // Agora começa em 90°
// **********************************

void setup() {
  servoMotor.attach(SERVO_PIN);

  // inicia o servo em 90°
  servoMotor.write(currentPosition);
  delay(500);
}

void loop() {

  // 1. MOVIMENTO DE 90 → 180
  for (currentPosition = 90; currentPosition <= 180; currentPosition += PASSO_ANGULAR) {
    servoMotor.write(currentPosition);
    delay(DELAY_MOVIMENTO);
  }

  // Pausa no 180°
  delay(1000);

  // 2. MOVIMENTO DE 180 → 90
  for (currentPosition = 180; currentPosition >= 90; currentPosition -= PASSO_ANGULAR) {
    servoMotor.write(currentPosition);
    delay(DELAY_MOVIMENTO);
  }

  // Pausa no 90°
  delay(500);
}
