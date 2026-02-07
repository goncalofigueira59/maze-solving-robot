/*
 * File:   MegaBot_V2.1
 * Author: GF
 *
 * Created on 11 de Dezembro de 2024, 23:57
 */

#include <xc.h>
//#include <stdio.h> // Comentado, pois não está sendo utilizado no código

////////////////////////////// SENSORIZAÇÃO ///////////////////////////
// Define os sensores e os respectivos pinos
#define SENSOR_1 PORTBbits.RB1
#define SENSOR_2 PORTBbits.RB2
#define SENSOR_3 PORTBbits.RB3
#define SENSOR_4 PORTBbits.RB4
#define SENSOR_5 PORTBbits.RB5
#define SENSOR_6 PORTBbits.RB6
#define SENSOR_7 PORTBbits.RB7

///////////////////// ATUADORES ////////////////////////////////////
// Definição de controle do sentido de rotação dos motores
#define MOTORA_DIRECAO_PR LATAbits.LATA1 // Motor A no sentido PR (horário)
#define MOTORA_DIRECAO_CPR LATAbits.LATA2 // Motor A no sentido CPR (anti-horário)
#define MOTORB_DIRECAO_PR LATAbits.LATA3 // Motor B no sentido PR
#define MOTORB_DIRECAO_CPR LATAbits.LATA4 // Motor B no sentido CPR

#define PR 1   // Define o sentido PR (horário)
#define CPR 0  // Define o sentido CPR (anti-horário)

// Controle PWM para os motores
#define MOTOR_A_HB CCPR2L            // Registrador PWM Motor A (8 bits mais significativos)
#define MOTOR_A_LB CCP2CONbits.DC2B  // Registrador PWM Motor A (2 bits menos significativos)
#define MOTOR_B_HB CCPR1L            // Registrador PWM Motor B (8 bits mais significativos)
#define MOTOR_B_LB CCP1CONbits.DC1B  // Registrador PWM Motor B (2 bits menos significativos)

// Valores pré-definidos para controle de velocidade
#define PWM_NORMAL 180  // Velocidade normal (mínimo 100, máximo 255)
#define PWM_CURVA 200   // Velocidade para curvas
#define PWM_PARAR 0     // Velocidade para parar os motores

//////////////////// CONFIGURAÇÕES ////////////////////
// Configuração da comunicação série
void configurarSERIAL() {
  TXSTAbits.TXEN = 1;  // Habilita a transmissão
  TXSTAbits.SYNC = 0;  // Configuração para comunicação assíncrona
  TXSTAbits.BRGH = 0;  // Configura o baud rate como baixo
  TXSTAbits.TX9 = 0;   // Comunicação de 8 bits

  RCSTAbits.SPEN = 1;  // Habilita o módulo série
  RCSTAbits.CREN = 1;  // Habilita a receção contínua
  RCSTAbits.RX9 = 0;   // Comunicação de 8 bits

  BAUDCONbits.BRG16 = 0; // Configuração de 8 bits para o baud rate

  SPBRGH = 0;           // Configura baud rate (parte alta)
  SPBRG = 77;           // Configura baud rate (parte baixa)
}

// Configuração de interrupções
void configurarINTERRUPT() {
  GIE = 1;   // Habilita interrupções globais
  PEIE = 1;  // Habilita interrupções periféricas
  TXIE = 1;  // Habilita interrupção de transmissão série
  RCIE = 1;  // Habilita interrupção de recepção série
}

// Configuração dos pinos
void configurarPIN() {
  TRISB = 0xFF;             // Configura todos os pinos do PORTB como entrada
  INTCON2bits.RBPU = 1;     // Desativa os pull-ups internos no PORTB
  ADCON1bits.PCFG = 0b1111; // Configura todos os pinos como digitais

  TRISAbits.RA1 = 0; // Configura os pinos do Motor A e B como saída
  TRISAbits.RA2 = 0;
  TRISAbits.RA3 = 0;
  TRISAbits.RA4 = 0;

  TRISCbits.RC6 = 1; // Configura pinos RC6 e RC7 como entrada para comunicação série
  TRISCbits.RC7 = 1;
}

// Configuração do PWM
void configurarPWM() {
  TRISCbits.RC1 = 0;          // Configura pinos RC1 e RC2 como saída para PWM
  TRISCbits.RC2 = 0;

  CCP1CONbits.CCP1M = 0b1100; // Configura CCP1 no modo PWM
  CCP2CONbits.CCP2M = 0b1100; // Configura CCP2 no modo PWM

  T2CON = 0b00000110;         // Configura Timer2 para PWM com prescaler 1:16
  PR2 = 74;                   // Define o período do PWM
}

// Controle de velocidade dos motores usando PWM
void PWM_MOTORES(unsigned int PWMA, unsigned int PWMB) {
  MOTOR_A_HB = (PWMA >> 2) & 0xFF; // Define os 8 bits mais significativos para Motor A
  MOTOR_A_LB = PWMA & 0x03;        // Define os 2 bits menos significativos para Motor A
  MOTOR_B_HB = (((int)(PWMB * 0.98)) >> 2) & 0xFF; // Ajusta o PWM para Motor B
  MOTOR_B_LB = ((int)(PWMB * 0.98)) & 0x03;
}

// Controle da direção e velocidade dos motores
void controloMOTOR(unsigned int PWMMOTORA, char direcaoA, unsigned int PWMMOTORB, char direcaoB) {
  // Controle da direção do Motor A
  if (direcaoA) {
    MOTORA_DIRECAO_CPR = 0;
    MOTORA_DIRECAO_PR = 1;
  } else {
    MOTORA_DIRECAO_PR = 0;
    MOTORA_DIRECAO_CPR = 1;
  }

  // Controle da direção do Motor B
  if (direcaoB) {
    MOTORB_DIRECAO_CPR = 0;
    MOTORB_DIRECAO_PR = 1;
  } else {
    MOTORB_DIRECAO_PR = 0;
    MOTORB_DIRECAO_CPR = 1;
  }

  PWM_MOTORES(PWMMOTORA, PWMMOTORB); // Ajusta a velocidade dos motores
}

// Movimentos do robô
void frente() {
  controloMOTOR(PWM_NORMAL, PR, PWM_NORMAL, PR); // Move para frente
}

void direita() {
  controloMOTOR(PWM_NORMAL, PR, PWM_CURVA, PR); // Vira à direita
}

void esquerda() {
  controloMOTOR(PWM_CURVA, PR, PWM_NORMAL, PR); // Vira à esquerda
}

void parar() {
  controloMOTOR(PWM_PARAR, PR, PWM_PARAR, PR); // Para o movimento
}

// Função principal
void main(void) {
  configurarPIN();          // Configura os pinos
  configurarPWM();          // Configura o PWM
  configurarSERIAL();       // Configura a comunicação série
  configurarINTERRUPT();    // Configura as interrupções

  while (1) {
    // Envia os estados dos sensores pela comunicação série
    TXREG = SENSOR_1 ? '1' : '0';
    while (!TXSTAbits.TRMT);
    TXREG = SENSOR_2 ? '1' : '0';
    while (!TXSTAbits.TRMT);
    TXREG = SENSOR_3 ? '1' : '0';
    while (!TXSTAbits.TRMT);
    TXREG = SENSOR_4 ? '1' : '0';
    while (!TXSTAbits.TRMT);
    TXREG = SENSOR_5 ? '1' : '0';
    while (!TXSTAbits.TRMT);
    TXREG = SENSOR_6 ? '1' : '0';
    while (!TXSTAbits.TRMT);
    TXREG = SENSOR_7 ? '1' : '0';

    while (!TXSTAbits.TRMT); //envia um espaço
    TXREG = ' ';

    // Lógica de controle do robô com base nos sensores
    if (!(SENSOR_2) && !(SENSOR_5)) {
      frente(); // Move para frente
      TXREG = 'F';
    } else if (!(SENSOR_2) && (SENSOR_5)) {
      esquerda(); // Vira à esquerda
      TXREG = 'E';
    } else if ((SENSOR_2) && !(SENSOR_5)) {
      direita(); // Vira à direita
      TXREG = 'D';
    } else if ((SENSOR_2) && (SENSOR_5)) {
      parar(); // Para o robô
      TXREG = 'P';
    }
  }
}
