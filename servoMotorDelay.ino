#include <Servo.h>


// **********************************
// *** CONSTANTES DE PINO E LÓGICA ***
// **********************************
#define PINO_SINAL_RECONHECIMENTO 2 // Pino para receber o sinal de sucesso (LOW)
#define PINO_SERVO 9                // Pino para o servo motor


// Posições em graus para o servo motor
const int POSICAO_TRAVADA = 0;      // Posição inicial/fechada (ex: 0 graus)
const int POSICAO_ABERTA = 90;      // Posição de abertura (ex: 90 graus)
const long TEMPO_ABERTURA = 5000;   // Tempo em ms que a porta fica aberta (5 segundos)


// **********************************
// *** CONSTANTES DE MOVIMENTO SUAVE ***
// **********************************
const int PASSO_ANGULAR = 1;        // Quantidade de graus para mover por vez (Suavidade)
const int DELAY_MOVIMENTO = 50;     // Tempo (ms) entre cada passo (Velocidade)


// **********************************
// *** VARIÁVEIS DE ESTADO ***
// **********************************
Servo meuServo;
int currentServoPos = POSICAO_TRAVADA; // Rastreia a posição atual do servo


long tempoAberturaAnterior = 0; // Armazena o momento em que a porta foi aberta
long lastMovementTime = 0;      // Armazena o momento do último movimento do servo


// Variável para rastrear o estado atual do sistema
// 0: TRAVADO
// 1: ABRINDO (Movendo suavemente para POSICAO_ABERTA)
// 2: ABERTO (Esperando TEMPO_ABERTURA)
// 3: FECHANDO (Movendo suavemente para POSICAO_TRAVADA)
int estadoPorta = 0;




void setup() {
  // Configura o pino de sinal com PULLUP, ele será HIGH por padrão.
  // O sinal de sucesso (LOW) virá quando o reconhecimento ocorrer.
  pinMode(PINO_SINAL_RECONHECIMENTO, INPUT_PULLUP);


  meuServo.attach(PINO_SERVO);
  meuServo.write(POSICAO_TRAVADA);


  Serial.begin(9600);
  Serial.println("Sistema de Acesso Facial Inicializado!");
  Serial.println("Estado Atual: TRAVADO");
}




void loop() {
  // Lê o estado do pino de sinal de reconhecimento facial
  int sinalReconhecimento = digitalRead(PINO_SINAL_RECONHECIMENTO);


  // Lógica principal de controle de estado
  switch (estadoPorta) {
   
    case 0: // ESTADO: TRAVADO
      // Verifica se houve um sinal de reconhecimento (LOW, pois está com PULLUP)
      if (sinalReconhecimento == LOW) {
        Serial.println("Reconhecimento facial SUCESSO! Abrindo...");
        // Define a posição inicial para o movimento suave
        currentServoPos = meuServo.read();
        estadoPorta = 1; // Próximo estado: ABRINDO (Inicia o movimento suave)
      }
      break;


    case 1: // ESTADO: ABRINDO (Movimento suave para POSICAO_ABERTA)
      // Verifica se o tempo para o próximo passo de movimento já passou
      if (millis() - lastMovementTime >= DELAY_MOVIMENTO) {
       
        // Verifica se a posição final (ABERTA) foi atingida
        if (currentServoPos < POSICAO_ABERTA) {
          // Incrementa a posição
          currentServoPos += PASSO_ANGULAR;
          // Garante que não ultrapasse a posição final
          if (currentServoPos > POSICAO_ABERTA) {
            currentServoPos = POSICAO_ABERTA;
          }
          meuServo.write(currentServoPos);
          lastMovementTime = millis(); // Atualiza o tempo do último movimento
        }
       
        // Se a posição final foi atingida, muda o estado
        if (currentServoPos >= POSICAO_ABERTA) {
          tempoAberturaAnterior = millis(); // Marca o tempo que a porta ABRIU
          estadoPorta = 2; // Próximo estado: ABERTO
          Serial.println("Porta ABERTA. Esperando 5 segundos...");
        }
      }
      break;


    case 2: // ESTADO: ABERTO
      // Verifica se o TEMPO_ABERTURA já passou
      if (millis() - tempoAberturaAnterior >= TEMPO_ABERTURA) {
        Serial.println("Tempo esgotado. Fechando/Travando...");
        // Define a posição inicial para o movimento suave
        currentServoPos = meuServo.read();
        estadoPorta = 3; // Próximo estado: FECHANDO (Inicia o movimento suave)
      }
      break;


    case 3: // ESTADO: FECHANDO (Movimento suave para POSICAO_TRAVADA)
      // Verifica se o tempo para o próximo passo de movimento já passou
      if (millis() - lastMovementTime >= DELAY_MOVIMENTO) {
       
        // Verifica se a posição final (TRAVADA) foi atingida
        if (currentServoPos > POSICAO_TRAVADA) {
          // Decrementa a posição
          currentServoPos -= PASSO_ANGULAR;
          // Garante que não ultrapasse a posição final
          if (currentServoPos < POSICAO_TRAVADA) {
            currentServoPos = POSICAO_TRAVADA;
          }
          meuServo.write(currentServoPos);
          lastMovementTime = millis(); // Atualiza o tempo do último movimento
        }
       
        // Se a posição final foi atingida, muda o estado
        if (currentServoPos <= POSICAO_TRAVADA) {
          estadoPorta = 0; // Próximo estado: TRAVADO
          Serial.println("Porta TRAVADA.");
        }
      }
      break;


    default:
      // Caso de segurança: reverte para o estado inicial
      estadoPorta = 0;
      meuServo.write(POSICAO_TRAVADA);
      Serial.println("Estado Inválido, redefinido para TRAVADO.");
      break;
  }
}
