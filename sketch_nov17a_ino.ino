#include <Servo.h>


// **********************************
// *** DECLARAÇÃO DE VARIÁVEIS E OBJETOS ***
// **********************************
Servo servoMotor; // Cria o objeto para o servo motor


// O Arduino Mega tem muitos pinos digitais.
// Você pode usar qualquer pino digital (ex: 2 a 13) ou pino PWM (com ~).
const int SERVO_PIN = 8; // Pino digital 8 para o sinal (fio amarelo/laranja)


// **********************************
// *** CONSTANTES DE VELOCIDADE E SUAVIZAÇÃO ***
// **********************************
// Controla a VELOCIDADE GERAL:
// 5ms = Rápido | 15ms = Médio | 30ms = Lento
const int DELAY_MOVIMENTO = 50;


// Controla a SUAVIZAÇÃO do movimento:
// 1 = Mais suave | 5 = Mais rápido, menos suave
const int PASSO_ANGULAR = 1;


int currentPosition = 0; // Variável para armazenar a posição atual do servo
// **********************************




void setup() {
  // Associa o objeto 'servoMotor' ao pino de controle (SERVO_PIN).
  // Verifique se o pino 8 está livre e adequado para o seu projeto no Mega.
  servoMotor.attach(SERVO_PIN);
  // Garante que o servo comece na posição inicial (0 graus).
  servoMotor.write(currentPosition);
  delay(500); // Pausa inicial para estabilização
}




void loop() {
  // 1. MOVIMENTO DE ABERTURA: Gira de 0 para 180 graus
  for (currentPosition = 0; currentPosition <= 180; currentPosition += PASSO_ANGULAR) {
    servoMotor.write(currentPosition);
    // Atraso para controlar a velocidade da transição.
    delay(DELAY_MOVIMENTO);
  }


  // Pausa na posição final (180 graus)
  delay(1000); // Fica parado por 1 segundo


 
  // 2. MOVIMENTO DE FECHAMENTO: Gira de 180 para 0 graus
  for (currentPosition = 180; currentPosition >= 0; currentPosition -= PASSO_ANGULAR) {
    servoMotor.write(currentPosition);
    // Atraso para controlar a velocidade da transição.
    delay(DELAY_MOVIMENTO);
  }


  // Pausa na posição inicial (0 graus)
  delay(500); // Fica parado por meio segundo
}