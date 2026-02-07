/*
 * File:   MegaBot_V0.1
 * Author: GF
 *
 * Created on 11 de Dezembro de 2024, 23:57
 */

#include <xc.h>

//////////////////////////////SENSORIZAÇÃO///////////////////////////
// Definição dos sensores e os pinos correspondentes
// Invertendo a lógica com '!'
#define SENSOR_1 !PORTBbits.RB1
#define SENSOR_2 !PORTBbits.RB2
#define SENSOR_3 !PORTBbits.RB3
#define SENSOR_4 !PORTBbits.RB4
#define SENSOR_5 !PORTBbits.RB5
#define SENSOR_6 !PORTBbits.RB6
#define SENSOR_7 !PORTBbits.RB7

/////////////////////ATUADORES////////////////////////////////////
// Controle do sentido de rotação dos motores
#define MOTORA_DIRECAO_PR LATAbits.LATA1
#define MOTORA_DIRECAO_CPR LATAbits.LATA2
#define MOTORB_DIRECAO_PR LATAbits.LATA3
#define MOTORB_DIRECAO_CPR LATAbits.LATA4

#define PR 1   // Sentido horário
#define CPR 0  // Sentido anti-horário

// Definição de registros para controle PWM dos motores
// MOTOR A (esquerdo)
#define MOTOR_A_HB CCPR2L            // 8 bits mais significativos do PWM do Motor A
#define MOTOR_A_LB CCP2CONbits.DC2B  // 2 bits menos significativos do PWM do Motor A

// MOTOR B (direito)
#define MOTOR_B_HB CCPR1L            // 8 bits mais significativos do PWM do Motor B
#define MOTOR_B_LB CCP1CONbits.DC1B  // 2 bits menos significativos do PWM do Motor B

// Definição de valores de PWM para diferentes movimentos
#define PWM_NORMAL 180  // Velocidade normal (min 100, max 255) (Motor sem carga)
#define PWM_PARAR 0     // Parar os motores
#define PWM_CURVA 180   // Velocidade em curva

// Configura os pinos de entrada/saída
void configurarPIN() {
  TRISB = 0xFF;              // Todos os pinos do PORTB como entrada (sensores)
  INTCON2bits.RBPU = 1;      // Desabilita pull-ups internos no PORTB
  ADCON1bits.PCFG = 0b1111;  // Configura todos os pinos como digitais
  TRISAbits.RA1 = 0;         // Motores configurados como saída
  TRISAbits.RA2 = 0;
  TRISAbits.RA3 = 0;
  TRISAbits.RA4 = 0;
}

// Configura o PWM para os motores
void configurarPWM() {
  TRISCbits.RC1 = 0;  // Pinos RC1 e RC2 como saída
  TRISCbits.RC2 = 0;

  CCP1CONbits.CCP1M = 0b1100;  // Configura CCP1 no modo PWM
  CCP2CONbits.CCP2M = 0b1100;  // Configura CCP2 no modo PWM

  T2CON = 0b00000110;  // Configura Timer2 para o PWM
  PR2 = 74;            // Define o período do PWM
}

// Ajusta a velocidade dos motores usando PWM
void PWM_MOTORES(unsigned int PWMA, unsigned int PWMB) {
  MOTOR_A_HB = (PWMA >> 2) & 0xFF;                  // Define os 8 bits mais significativos para o Motor A
  MOTOR_A_LB = PWMA & 0x03;                         // Define os 2 bits menos significativos para o Motor A
  MOTOR_B_HB = (((int)(PWMB * 0.98)) >> 2) & 0xFF;  // Define PWM ajustado para o Motor B
  MOTOR_B_LB = ((int)(PWMB * 0.98)) & 0x03;
}

// Controla a direção e velocidade dos motores
void controloMOTOR(unsigned int PWMMOTORA, char direcaoA, unsigned int PWMMOTORB, char direcaoB) {
  // Define a direção do Motor A
  if (direcaoA) {
    MOTORA_DIRECAO_CPR = 0;
    MOTORA_DIRECAO_PR = 1;
  } else {
    MOTORA_DIRECAO_PR = 0;
    MOTORA_DIRECAO_CPR = 1;
  }

  // Define a direção do Motor B
  if (direcaoB) {
    MOTORB_DIRECAO_CPR = 0;
    MOTORB_DIRECAO_PR = 1;
  } else {
    MOTORB_DIRECAO_PR = 0;
    MOTORB_DIRECAO_CPR = 1;
  }

  // Define a velocidade dos motores
  PWM_MOTORES(PWMMOTORA, PWMMOTORB);
}

// Movimentos predefinidos para o robô
void frente() {
  controloMOTOR(PWM_NORMAL, PR, PWM_NORMAL, PR);
}

void direita() {
  controloMOTOR(PWM_CURVA, CPR, PWM_CURVA, PR);
}

void esquerda() {
  controloMOTOR(PWM_CURVA, PR, PWM_CURVA, CPR);
}

void meiavolta() {
  controloMOTOR(PWM_NORMAL, PR, PWM_NORMAL, CPR);
}

void parar() {
  controloMOTOR(PWM_PARAR, PR, PWM_PARAR, PR);
}


void main(void) {

  // chama a função para configura os pinos e o PWM
  configurarPIN();
  configurarPWM();

  while (1) {
    // Caso todos os sensores estejam ativados, para o robô
    if (SENSOR_1 && SENSOR_2 && SENSOR_3 && SENSOR_4 && SENSOR_5 && SENSOR_6 && SENSOR_7) {
      parar();
    }
    // Caso apenas SENSOR_1 esteja ativado, vira para a esquerda
    else if (SENSOR_1) {
      esquerda();
    }
    // Caso SENSOR_3 esteja ativado e SENSOR_1 não, segue em frente
    else if (!SENSOR_1 && SENSOR_3) {
      frente();
    }
    // Caso SENSOR_6 esteja ativado e os anteriores não, vira para a direita
    else if (!SENSOR_1 && !SENSOR_3 && SENSOR_6) {
      direita();
    }
    // Caso SENSOR_4 esteja ativado e os demais não, faz meia-volta
    else if (!SENSOR_1 && !SENSOR_3 && SENSOR_4 && !SENSOR_6) {
      meiavolta();
    }
  }
}
