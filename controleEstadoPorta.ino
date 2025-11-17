  // Lê o estado do pino de sinal de reconhecimento facial
  int sinalReconhecimento = digitalRead(PINO_SINAL_RECONHECIMENTO);


  // Lógica principal de controle de estado
  switch (estadoPorta) {
    case 0: // ESTADO: TRAVADO
      // Verifica se houve um sinal de reconhecimento (LOW, pois está com PULLUP)
      if (sinalReconhecimento == LOW) {
        // Sinal recebido, iniciar a abertura
        Serial.println("Reconhecimento facial SUCESSO! Abrindo...");
        meuServo.write(POSICAO_ABERTA);
        tempoAberturaAnterior = millis(); // Marca o tempo de abertura
        estadoPorta = 1; // Próximo estado: Abrindo (curta duração, apenas para transição visual/servo)
      }
      break;


    case 1: // ESTADO: ABRINDO
      // Neste simples caso, a abertura é instantânea com servo.
      // Mudamos imediatamente para o estado ABERTO e esperamos o TEMPO_ABERTURA.
      estadoPorta = 2;
      Serial.println("Porta ABERTA. Esperando 5 segundos...");
      break;


    case 2: // ESTADO: ABERTO
      // Verifica se o TEMPO_ABERTURA já passou
      if (millis() - tempoAberturaAnterior >= TEMPO_ABERTURA) {
        // Tempo esgotado, fechar a porta
        Serial.println("Tempo esgotado. Fechando/Travando...");
        meuServo.write(POSICAO_TRAVADA);
        estadoPorta = 3; // Próximo estado: Fechando (curta duração)
      }
      break;


    case 3: // ESTADO: FECHANDO
      // A transição é instantânea (servo fecha). Volta ao estado travado.
      estadoPorta = 0;
      Serial.println("Porta TRAVADA.");
      break;
#include <Servo.h>


#define PINO_SINAL_RECONHECIMENTO 2 // COM -> Porta Digital 2
#define PINO_SERVO 9 // Servo -> Porta Digital 9


Servo meuServo; // Cria o objeto servo


// Posições em graus para o servo motor
const int POSICAO_TRAVADA = 0; // Posição inicial (ex: 0 graus)
const int POSICAO_ABERTA = 90; // Posição de abertura (ex: 90 graus)
const long TEMPO_ABERTURA = 5000; // Tempo em milissegundos que a porta fica aberta (5 segundos)


long tempoAberturaAnterior = 0; // Armazena o momento em que a porta foi aberta


// Variável para rastrear o estado atual do sistema
// 0: Travado (Padrão)
// 1: Abrindo
// 2: Aberto
// 3: Fechando
int estadoPorta = 0;


void setup() {
  // Configura a Porta 2 com PULLUP INTERNO.
  // Isso garante que o pino esteja em HIGH por padrão (quando o relé está aberto).
  // O sinal de reconhecimento (LOW) virá quando o relé fechar.
  pinMode(PINO_SINAL_RECONHECIMENTO, INPUT_PULLUP);


  meuServo.attach(PINO_SERVO); // Conecta o objeto servo ao pino 9
  meuServo.write(POSICAO_TRAVADA); // Define a posição inicial/travada


  Serial.begin(9600); // Para debug
  Serial.println("Sistema de Acesso Facial Inicializado!");
  Serial.println("Estado Atual: TRAVADO");
}


void loop() {


    default:
      // Caso de segurança
      estadoPorta = 0;
      meuServo.write(POSICAO_TRAVADA);
      Serial.println("Estado Inválido, redefinido para TRAVADO.");
      break;
  }


  // Pequeno delay para evitar debounce excessivo e sobrecarga na CPU (opcional)
  delay(10);
}