#include <Servo.h>


// **********************************
// *** DECLARAÇÃO DE VARIÁVEIS E OBJETOS ***
// **********************************
Servo servoMotor1; // Objeto para o primeiro servo (0 -> 180)
Servo servoMotor2; // **NOVO:** Objeto para o segundo servo (180 -> 0)


// O Arduino Mega tem muitos pinos digitais.
// Você pode usar qualquer pino digital (ex: 2 a 13) ou pino PWM (com ~).
const int SERVO_PIN_1 = 2; // Pino para o Servo 1
const int SERVO_PIN_2 = 8; // **NOVO:** Pino para o Servo 2 (escolhi o 9)


// **********************************
// *** CONSTANTES DE VELOCIDADE E SUAVIZAÇÃO ***
// **********************************
// Controla a VELOCIDADE GERAL:
// 5ms = Rápido | 15ms = Médio | 30ms = Lento
const int DELAY_MOVIMENTO = 50;


// Controla a SUAVIZAÇÃO do movimento:
// 1 = Mais suave | 5 = Mais rápido, menos suave
const int PASSO_ANGULAR = 1;


int currentPosition = 0; // Variável para armazenar a posição atual
// **********************************




void setup() {
  // --- Configuração do Servo 1 ---
  servoMotor1.attach(SERVO_PIN_1);
  servoMotor1.write(currentPosition); // Começa em 0 graus


  // **NOVO: Configuração do Servo 2 ---
  servoMotor2.attach(SERVO_PIN_2);
  servoMotor2.write(180); // **IMPORTANTE:** Começa na posição 180 (oposta)
 
  delay(500); // Pausa inicial para estabilização
}




void loop() {
  // 1. MOVIMENTO DE ABERTURA:
  // Servo 1: Gira de 0 para 180 (ABERTURA)
  // Servo 2: Gira de 180 para 0 (FECHAMENTO - Sentido Inverso)
  for (currentPosition = 0; currentPosition <= 180; currentPosition += PASSO_ANGULAR) {
    // Servo 1: Posição Normal (0 -> 180)
    servoMotor1.write(currentPosition);


    // **NOVO:** Servo 2: Posição Inversa (180 -> 0)
    // A fórmula '180 - currentPosition' inverte o movimento.
    // Quando 'currentPosition' é 0, 'servoMotor2' vai para 180.
    // Quando 'currentPosition' é 180, 'servoMotor2' vai para 0.
    servoMotor2.write(180 - currentPosition);
   
    delay(DELAY_MOVIMENTO);
  }


  // Pausa na posição final (Servo 1 em 180, Servo 2 em 0)
  delay(1000);


 
  // 2. MOVIMENTO DE FECHAMENTO:
  // Servo 1: Gira de 180 para 0 (FECHAMENTO)
  // Servo 2: Gira de 0 para 180 (ABERTURA - Sentido Inverso)
  for (currentPosition = 180; currentPosition >= 0; currentPosition -= PASSO_ANGULAR) {
    // Servo 1: Posição Normal (180 -> 0)
    servoMotor1.write(currentPosition);


    // **NOVO:** Servo 2: Posição Inversa (0 -> 180)
    servoMotor2.write(180 - currentPosition);
   
    delay(DELAY_MOVIMENTO);
  }


  // Pausa na posição inicial (Servo 1 em 0, Servo 2 em 180)
  delay(500);
}