// Este código controla a posição de um Servo Motor usando um teclado matricial 4x4,
// E exibe a tecla pressionada em um Display LCD 16x2, tudo sem bibliotecas.

// --- Configuração dos Pinos do LCD (Modo 4-bit) ---
// Note: As conexões do potenciômetro (V0) para contraste e o pino R/W (GND) devem ser feitas manualmente.
const int rs = 11; // Register Select (RS)
const int en = 12; // Enable (E)
const int d4 = 13; // Data 4
const int d5 = A0; // Data 5 (usando pino analógico como digital)
const int d6 = A1; // Data 6 (usando pino analógico como digital)
const int d7 = A2; // Data 7 (usando pino analógico como digital)

// --- Configuração dos Pinos do Servo Motor ---
const int pinoServo = 9; // Pino digital para o sinal do Servo Motor

// --- Configuração do Teclado Matricial 4x4 ---
const byte LINHAS = 4;
const byte COLUNAS = 4;

// Mapeamento dos caracteres no teclado
char teclas[LINHAS][COLUNAS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Pinos do Arduino conectados às LINHAS (configurados como SAÍDAS)
byte pinosLinhas[LINHAS] = {2, 3, 4, 5};

// Pinos do Arduino conectados às COLUNAS (configurados como ENTRADAS c/ PULL-UP)
byte pinosColunas[COLUNAS] = {6, 7, 8, 10};

// --- Variáveis de Controle do Servo ---
#define SERVO_PERIODO_US 20000 // Período de 20ms
volatile int duracaoPulsoUS = 1500;   // Posição atual do pulso (começa em 90 graus)
volatile int targetPulsoUS = 1500;    // NOVO: Posição final desejada (o que a tecla define)
const int passoServoUS = 5;           // NOVO: Quantos microssegundos o pulso muda por ciclo.
                                      // Diminuir este valor deixa o movimento mais lento.
unsigned long ultimoPulsoTempo = 0;


// --- FUNÇÕES DE CONTROLE MANUAL DO LCD 16x2 (Modo 4-bit) ---

// Envia o pulso de HABILITAÇÃO (Enable)
// Tempo de espera reduzido para 40us (padrão para a maioria das instruções)
void pulseEnable() {
  digitalWrite(en, LOW);
  delayMicroseconds(1);
  digitalWrite(en, HIGH); // Pulso HIGH
  delayMicroseconds(1);   // Mantém HIGH por um breve período
  digitalWrite(en, LOW);  // Pulso LOW
  // Espera o tempo de execução da instrução (37us min.)
  delayMicroseconds(40); 
}

// Escreve um nibble (4 bits) nos pinos D4-D7
void write4bits(byte value) {
  digitalWrite(d4, (value >> 0) & 0x01);
  digitalWrite(d5, (value >> 1) & 0x01);
  digitalWrite(d6, (value >> 2) & 0x01);
  digitalWrite(d7, (value >> 3) & 0x01);
}

// Envia um byte (comando ou dado) para o LCD
void sendByte(byte value, boolean mode) {
  // Configura RS (mode = false para comando, true para dado)
  digitalWrite(rs, mode);

  // Envia o nibble HIGH (bits 7 a 4)
  write4bits(value >> 4);
  pulseEnable();

  // Envia o nibble LOW (bits 3 a 0)
  write4bits(value);
  pulseEnable();
}

// Envia um COMANDO para o LCD
void lcdCommand(byte command) {
  sendByte(command, LOW);
  // CORREÇÃO: Adiciona espera extra para comandos longos (Clear e Home)
  if (command == 0x01 || command == 0x02) {
    delay(2); // Espera 2ms (necessário 1.52ms)
  }
}

// Envia um DADO (caractere) para o LCD
void lcdData(byte data) {
  sendByte(data, HIGH);
}

// Inicializa o LCD no modo 4-bit
void lcdInit() {
  // Configura todos os pinos do LCD como SAÍDA
  pinMode(rs, OUTPUT);
  pinMode(en, OUTPUT);
  pinMode(d4, OUTPUT);
  pinMode(d5, OUTPUT);
  pinMode(d6, OUTPUT);
  pinMode(d7, OUTPUT);

  // Sequência de inicialização para modo 4-bit (Mais robusta)
  digitalWrite(rs, LOW);
  digitalWrite(en, LOW);
  delay(50); // Espera o LCD inicializar

  // Tenta 1
  write4bits(0x03); 
  pulseEnable(); 
  delayMicroseconds(4500); // Espera mais de 4.1ms

  // Tenta 2
  write4bits(0x03); 
  pulseEnable(); 
  delayMicroseconds(150); // Espera mais de 100us

  // Tenta 3
  write4bits(0x03); 
  pulseEnable(); 

  // Configura para modo 4-bit
  write4bits(0x02); 
  pulseEnable();

  // Funções Set (agora com lcdCommand, que inclui o delay de 40us)
  
  // Função Set: 4-bit, 2 Linhas, Font 5x8
  lcdCommand(0x28);
  
  // Display ON, Cursor OFF, Blink OFF
  lcdCommand(0x0C);
  
  // Clear Display (O delay de 2ms está dentro de lcdCommand)
  lcdCommand(0x01);
  
  // Entry Mode Set: Incrementa endereço, sem shift de display
  lcdCommand(0x06);
}

// Posiciona o cursor
void lcdSetCursor(byte col, byte row) {
  // Endereços: Linha 0: 0x00-0x0F, Linha 1: 0x40-0x4F
  int address = (row == 0) ? 0x80 + col : 0xC0 + col;
  lcdCommand(address);
}

// Imprime uma string no LCD
void lcdPrintString(const char *str) {
  while (*str) {
    lcdData(*str++);
  }
}


// --- FUNÇÃO DE VARREDURA MANUAL DO TECLADO ---
char scanKeypad() {
  // O código de varredura permanece o mesmo
  for (byte i = 0; i < LINHAS; i++) {
    pinMode(pinosLinhas[i], OUTPUT);
    digitalWrite(pinosLinhas[i], HIGH);
  }

  for (byte i = 0; i < COLUNAS; i++) {
    pinMode(pinosColunas[i], INPUT_PULLUP);
  }

  for (byte linha = 0; linha < LINHAS; linha++) {
    digitalWrite(pinosLinhas[linha], LOW);
    for (byte coluna = 0; coluna < COLUNAS; coluna++) {
      if (digitalRead(pinosColunas[coluna]) == LOW) {
        digitalWrite(pinosLinhas[linha], HIGH);
        // Debouncing
        delay(50); 
        return teclas[linha][coluna];
      }
    }
    digitalWrite(pinosLinhas[linha], HIGH);
  }

  return '\0'; 
}


void setup() {
  Serial.begin(9600);
  Serial.println("Controle Manual de Servo Motor com Teclado 4x4 e LCD");

  // Configura o pino do servo como SAÍDA
  pinMode(pinoServo, OUTPUT);

  // 1. Inicializa o LCD
  lcdInit();
  
  // 2. Mensagem inicial no LCD
  lcdSetCursor(0, 0);
  lcdPrintString("Servo Manual Ctl");
  lcdSetCursor(0, 1);
  lcdPrintString("Tecla: ");
}


void loop() {
  unsigned long tempoAtual = micros();

  // 1. Geração do Pulso PWM (Simulação do Servo.h)
  // O pulso PWM usa a duracaoPulsoUS atual, que é ajustada lentamente abaixo.
  if (tempoAtual - ultimoPulsoTempo >= SERVO_PERIODO_US) {
    digitalWrite(pinoServo, HIGH);
    delayMicroseconds(duracaoPulsoUS);
    digitalWrite(pinoServo, LOW);

    ultimoPulsoTempo += SERVO_PERIODO_US;
    
    if (tempoAtual - ultimoPulsoTempo >= SERVO_PERIODO_US) {
        ultimoPulsoTempo = tempoAtual;
    }
  }
  
  // 2. Rampa de Movimento Lento (O que faz o servo girar mais devagar)
  if (duracaoPulsoUS < targetPulsoUS) {
    // Se a posição atual for menor que a desejada, aumenta em um passo
    duracaoPulsoUS += passoServoUS;
    // Evita ultrapassar o alvo
    if (duracaoPulsoUS > targetPulsoUS) duracaoPulsoUS = targetPulsoUS;
  } else if (duracaoPulsoUS > targetPulsoUS) {
    // Se a posição atual for maior que a desejada, diminui em um passo
    duracaoPulsoUS -= passoServoUS;
    // Evita ir abaixo do alvo
    if (duracaoPulsoUS < targetPulsoUS) duracaoPulsoUS = targetPulsoUS;
  }
  // Fim da lógica de rampa

  // 3. Leitura do Teclado
  char tecla = scanKeypad();

  if (tecla != '\0') {
    Serial.print("Tecla Pressionada: ");
    Serial.println(tecla);
    
    // FUNÇÃO PARA EXIBIR A TECLA:
    lcdSetCursor(7, 1);
    lcdData(tecla);
    lcdPrintString(" "); 

    // Mapeia a tecla para uma nova DURAÇÃO DE PULSO ALVO (targetPulsoUS)
    switch (tecla) {
      case '1':
        targetPulsoUS = 500; // Posição alvo
        lcdSetCursor(10, 1);
        lcdPrintString("0 Deg ");
        Serial.println("Servo -> 0 graus");
        break;
        
      case '2':
        targetPulsoUS = 1000; // Posição alvo
        lcdSetCursor(10, 1);
        lcdPrintString("45 Deg");
        Serial.println("Servo -> 45 graus");
        break;
        
      case '3':
        targetPulsoUS = 1500; // Posição alvo
        lcdSetCursor(10, 1);
        lcdPrintString("90 Deg");
        Serial.println("Servo -> 90 graus");
        break;
        
      case '4':
        targetPulsoUS = 2500; // Posição alvo
        lcdSetCursor(10, 1);
        lcdPrintString("180 Deg");
        Serial.println("Servo -> 180 graus");
        break;

      default:
        // Caso outras teclas sejam pressionadas
        lcdSetCursor(10, 1);
        lcdPrintString("N/A   ");
        Serial.println("Tecla sem função definida para o servo.");
        break;
    }
    
    // Atraso para anti-repique do teclado
    delay(100);
  }
}