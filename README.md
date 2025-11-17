# Seminários Territoriais FECIBA 2025 - 13ª Edição  
## Projeto de Automação com Reconhecimento Facial e Servo Motores

Este repositório reúne os códigos desenvolvidos para o projeto apresentado nos **Seminários Territoriais da FECIBA 2025 (13ª edição)**.  
O objetivo é demonstrar a integração entre **sistemas de reconhecimento facial (Harkivison)** e **controle de servo motores**, aplicados em protótipos de automação, como portas inteligentes.

---

## 📂 Estrutura do Repositório

### 1. `controleEstadoPorta.ino`
- **Função:** Controla o estado da porta com base no sinal recebido do sistema de reconhecimento facial (Harkivison).  
- **Características:**
  - Recebe sinal via pino digital com `INPUT_PULLUP`.  
  - Implementa uma **máquina de estados**:
    - `0` → Porta Travada  
    - `1` → Abrindo  
    - `2` → Aberta (aguarda 5 segundos)  
    - `3` → Fechando  
  - O servo abre e fecha a porta automaticamente após o tempo configurado.  
- **Aplicação:** Código central para controle de acesso facial.

---

### 2. `servoMotor1.ino`
- **Função:** Controla um único servo motor com **movimento suave e temporizado**.  
- **Características:**
  - Movimento de abertura (0° → 180°) e fechamento (180° → 0°).  
  - Utiliza variáveis de **delay** e **passo angular** para suavizar o movimento.  
- **Aplicação:** Testes de velocidade e suavização do servo motor.

---

### 3. `servoMotor2.ino`
- **Função:** Controla **dois servo motores simultaneamente**, com movimentos inversos.  
- **Características:**
  - Servo 1 abre (0° → 180°) enquanto Servo 2 fecha (180° → 0°).  
  - Movimento sincronizado e inverso, útil para mecanismos coordenados.  
- **Aplicação:** Cenários em que dois servos precisam trabalhar em conjunto, como portas duplas.

---

### 4. `servoMotorDelay.ino`
- **Função:** Integra a lógica de **delay do servo motor** com o **controle de reconhecimento facial (Harkivison)**.  
- **Características:**
  - Usa o pino digital **2** para receber o sinal facial.  
  - Servo motor conectado ao pino **9**, com posições definidas:
    - `POSICAO_TRAVADA = 0°`  
    - `POSICAO_ABERTA = 90°`  
  - Implementa **movimento incremental**:
    - `PASSO_ANGULAR = 1°` por vez.  
    - `DELAY_MOVIMENTO = 50 ms` entre cada passo.  
  - Máquina de estados:
    - `0` → Travado  
    - `1` → Abrindo (suave até 90°)  
    - `2` → Aberto (aguarda 5 segundos)  
    - `3` → Fechando (suave até 0°)  
  - Inclui **tratamento de segurança**: qualquer estado inválido retorna para `TRAVADO`.  
- **Aplicação:** Versão final e integrada do projeto, ideal para demonstração prática.

---

### 5. `senhaLCD.ino`
- **Função:** Controla a posição de um **servo motor** usando um **teclado matricial 4x4** e exibe a tecla pressionada em um **LCD 16x2**, sem uso de bibliotecas externas.  
- **Características:**
  - Implementa controle manual do LCD em **modo 4-bit** (funções próprias para enviar comandos e dados).  
  - Varredura manual do teclado matricial para identificar teclas pressionadas.  
  - Mapeamento de teclas para posições específicas do servo:
    - `1` → 0°  
    - `2` → 45°  
    - `3` → 90°  
    - `4` → 180°  
  - Movimento suave do servo implementado via **rampa de PWM manual** (sem `Servo.h`).  
  - Exibe no LCD a tecla pressionada e a posição correspondente do servo.  
- **Aplicação:** Demonstração de interface homem-máquina (IHM), permitindo controle direto do servo via teclado e feedback visual no LCD.

---

## 📊 Fluxograma da Máquina de Estados

```text
          +-------------------+
          |   Estado 0        |
          |   TRAVADO         |
          +-------------------+
                   |
                   | sinal facial LOW
                   v
          +-------------------+
          |   Estado 1        |
          |   ABRINDO         |
          +-------------------+
                   |
                   | servo chegou a 90°
                   v
          +-------------------+
          |   Estado 2        |
          |   ABERTO          |
          +-------------------+
                   |
                   | tempo >= 5s
                   v
          +-------------------+
          |   Estado 3        |
          |   FECHANDO        |
          +-------------------+
                   |
                   | servo chegou a 0°
                   v
          +-------------------+
          |   Estado 0        |
          |   TRAVADO         |
          +-------------------+
```

---

## 🚀 Objetivo do Projeto
O projeto busca demonstrar como **visão computacional** e **automação mecânica** podem ser integradas em soluções práticas.  
Nos Seminários Territoriais da FECIBA, este protótipo exemplifica aplicações em **segurança, acessibilidade e inovação tecnológica**.

---

## 📌 Observações
- Certifique-se de ter a biblioteca **Servo.h** instalada no Arduino IDE.  
- O sistema **Harkivison** deve estar configurado corretamente para enviar o sinal de reconhecimento.  
- Recomenda-se iniciar os testes com os códigos individuais e depois avançar para a versão integrada.  

---

## 👥 Créditos
Projeto desenvolvido para os **Seminários Territoriais FECIBA 2025 - 13ª edição**.  
Equipe responsável: Daniel Bahia Cruz de Deus e Isabel Fernandes Conceição

Orientadores: Prof° Gilberto Monteiro e Prof° Lucas Freitas 

Colégio Estadual de Aplicação Anísio Teixeira

Instituto Anísio Teixeira
```
