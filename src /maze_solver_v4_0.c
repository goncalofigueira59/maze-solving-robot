/*
//=========================================================
//============= I D E N T I F I C A   O =================

> Projeto: Robot Solicionador de Labirinto Seguidor de Linha
> Ambito/Objetivo: Projeto Tematico em Eletrnica e Instrumentacao e Concurso MicroRato Edicao 2024/2025, by U.A.
> Robot: MegaBot v4.0
> Autor: Goncalo Figueira, 109438

//=========================================================

INFORMACOES DO PROJETO:

Sensores:
Sensor 1 = pino digital 1
Sensor 2 = pino digital 2
Sensor 3 = pino digital 3
Sensor 4 = pino digital 4
Sensor 5 = pino digital 5
Sensor 6 = pino digital 6
Sensor 7 = pino digital 7

Posio dos Sensores:

      7
          3
  1   2       5   6
          4

Botes:
Botao Modo Aprendizagem = pino digital 8
Botao Modo Otimizado = pino digital 9

Motores:
Motor Esquerdo Avana = pino digital 10
Motor Esquerdo Regride = pino digital 11
Motor Direito Avana = pino digital 12
Motor Direito Regride = pino digital 13

Buzzer = pino digital 0

SDA Lcd 128x64 - Pino Analgico A4
SCL Lcd 128x64 - Pino Analgico A5

Material Eletrico utilizado:

1x PIC18F26K22
1x Oscilador de Cristal HC-49S (20MHz) : Responsvel pelo Clock do PIC
2x Condensador cermico 27pF : Responsveis pelo funcionamento mais suave do Oscilador
1x Condensador cermico 100nF : Responsvel pela alimentao mais suave do PIC
2x Motor Coreless (Mini Coreless HM Motor DC 3-5V - 8520 CW + CCW) : Motor esquerdo e direito
  - Dependendo da corrente (sendo 0.15A a corrente mxima de alimentao) :
      - Se for alimentado a 5V, atinge uma velocidade giratoria de 50000rpm.
      - Se for alimentado a 3V, atinge uma velocidade giratoria de 35000 a 37000 rpm.
  - Dimenses :
    - Ncleo de 8,5mm de largura por 20mm de comprimento
    - Eixo de 5mm de comprimento por 1mm de dimetro
1x Boto On/Off : Corte Geral da Alimentao
1x Bateria de Ltio Recarregvel (JIANGQI - 18650) : Alimentao do robot
  - 5V - 5.8V, 11.2Ah/11200mAh 
  - Conector de alimentao XH2.54-2P 
  - Conector de Carregamento DC3.5*1.3mm
1x Modulo de 6 Sensores IR : Sensores 1, 2, 3, 4, 5 e 6
1x Modulo de sensor IR : Sensor 7
3x Push Buttons
  - SW1 : Reset PIC
  - SW2 : Boto Modo de Aprendizagem
  - SW3 : Boto Modo de execuo do Caminho Otimizado
1x Resistencia 390 Ohm : Pull-up ao boto de reset do PIC
1x Modulo Mini L298N DC Motor Driver : Ponte H, para controlar os motores
  - Alimentao 2-10VDC
  - Velocidade de Rotao ajustvel atravs do PWM
  - Circuito de Proteo Trmica Embutido (TSD)
  - Capaz de fornecer at 1.5A a cada sada
  - O sinal de entrada tem que ser entre 1.8-7VDC
  - O driver  controlado por um CI MX1508
1x LCD 128x64 (Modelo 12864): Interface de Comunicao
  - Ecra OLED
  - Ecra de 0.96'' 
  - Comunicacao I2C
  - Tensao de Alimentacao 3-5VDC
  - Dimenses : 128x64
*/

                                                                                                                                                              //Verificar se falta declarar variveis
String caminho;  // String usada para gravar o caminho percorrido
String estado_sensores;

bool modo_aprendizagem;
bool modo_otimizado;

bool buzzer = 0;

int sensor1 = 1;
int sensor2 = 2;
int sensor3 = 3;
int sensor4 = 4;
int sensor5 = 5;
int sensor6 = 6;
int sensor7 = 7;

int motor_esq_avanca = 10;
int motor_esq_recua = 11;
int motor_dir_avanca = 12;
int motor_dir_recua = 13;

void setup() {
                                                                                                                                                                      //CONFIGURAR LCD e I/Os
  for(int i = 1, i < 10, i++) {    // Define os pinos 1 a 9 como entradas para os sensores e botes
    pinMode(i, INPUT);
  }
  
  for(int z = 10, z < 14, z++) {   // Define os pinos 10 a 13 como sadas para os motores
    pinMode(z, OUTPUT);
  }
  pinMode(0, OUTPUT);              // Define o pino 0 como sada para o Buzzer 
}

void loop() {

//=========================================================
//========= M O D O  D E  A P R E N D I Z A G E M =========
if(digitalRead(8)) {
  modo_aprendizagem = 1;
  modo_otimizado = 0; 
                                                                                                                                    // Resetar a informao existente na varivel caminho_otimizado, 
  }                                                                                                                                 // caso o robot se perca e tenha de reiniciar o trajeto
//=========================================================
//==== M O D O  D E  C A M I N H O  O T I M I Z A D O =====
if(digitalRead(9)) {
  modo_aprendizagem = 0;  
  modo_otimizado = 1; }
//=========================================================
//=================== S E N S O R E S =====================
/*
Neste bloco, o nivel lgico dos sensores (1 ou 0)  colocado 
numa varivel de nome "estado_sensores", conforme o lugar que 
eles ocupam, dispostos da esquerda para a direita. Para mais
tarde atravs da combinao momentnea dos mesmos, se consiga
identificar as junes.
*/
//==================== S E N S O R  1 =====================

  if(digitalRead(1)) {            
    estado_sensores[0] = "1";         // 1 X X X X X X
                                                                                                                                       //LCD muda posio do sensor 1 para 1
    }
  else {                              // 0 X X X X X X
    estado_sensores[0] = "0"; 
                                                                                                                                         //LCD muda posio do sensor 1 para 0
    }       

//==================== S E N S O R  2 =====================
  if(digitalRead(2)) {
    estado_sensores[1] = "1";         // X 1 X X X X X  
                                                                                                                                       //LCD muda posio do sensor 2 para 1
    }
  else {                                      // X 0 X X X X X
    estado_sensores[1] = "0";                                                                                              
                                                                                                                                       //LCD muda posio do sensor 2 para 0
    }    

//==================== S E N S O R  3 =====================

  if(digitalRead(3)) {
    estado_sensores.charAt(2) = "1";         // X X 1 X X X X  
                                                                                                                                        //LCD muda posio do sensor 3 para 1
    }
  else {                                      // X X 0 X X X X
    estado_sensores.charAt(2) = "0";
                                                                                                                                          //LCD muda posio do sensor 3 para 0
    }         

//==================== S E N S O R  4 =====================

  if(digitalRead(4)) {
    estado_sensores.charAt(3) = "1";        // X X X 1 X X X  
                                                                                                                                         //LCD muda posio do sensor 4 para 1
    }
  else {                                      // X X X 0 X X X
    estado_sensores.charAt(3) = "0"; 
                                                                                                                                        //LCD muda posio do sensor 4 para 0
    }         

//==================== S E N S O R  5 =====================

  if(digitalRead(5)) {
    estado_sensores.charAt(4) = "1";         // X X X X 1 X X
                                                                                                                                        //LCD muda posio do sensor 5 para 1
    }         
  else {                                      // X X X X 0 X X
    estado_sensores.charAt(4) = "0"; 
                                                                                                                                        //LCD muda posio do sensor 5 para 0
    }         

//==================== S E N S O R  6 =====================

  if(digitalRead(6)) {
    estado_sensores.charAt(5) = "1";         // X X X X X 1 X
                                                                                                                                      //LCD muda posio do sensor 6 para 1
    }         
  else {                                      // X X X X X 0 X
    estado_sensores.charAt(5) = "0"; 
                                                                                                                                      //LCD muda posio do sensor 6 para 0
    }         

//==================== S E N S O R  7 =====================

  if(digitalRead(7)) {
    estado_sensores.charAt(6) = "1";         // X X X X X X 1
                                                                                                                                    //LCD muda posio do sensor 7 para 1
    }   
  else {                                      // X X X X X X 0
    estado_sensores.charAt(6) = "0"; 
                                                                                                                                    //LCD muda posio do sensor 7 para 0
    }         

//=========================================================

  while (modo_aprendizagem == 1) {
                                                                                                                                                  //LCD exibe ("Modo de Aprendizagem")

    switch (estado_sensores) {

      case "0011000" :                        // ANDAR PARA A FRENTE (0011000)
        caminho += "F";
        frente();
        break;

      case "0010000" :                        // COMEAR CIRCUITO/SAIR DO BECO SEM SADA (0010000)
        caminho += "F";
        frente();
        break;

      case "1111110" :                        // CRUZAMENTO (1111110)
        caminho += "E";
        esquerda();
        break;

      case "1111111" :                        // FINAL DO PERCURSO (1111111)
        caminho += "P";
        parar();
                                                                                                                                                                //Buzzer (3xBIP)
                                                                                                                                                  //LCD diz "Chegou ao fim do Percurso"
        break;

      case "0011110":                         // T DIREITA (0011110)
        caminho += "D";
        direita();
        break;
      
      case "0001110" :                        // CURVA DIREITA (0001110)
        caminho += "D";
        direita();
        break;

      case "1101000" :                        // CURVA ESQUERDA (1101000)
        caminho += "E";
        esquerda();
        break;

      /*
      case "1101110" :                        // T VERTICAL (1101110)
        caminho += "E";
        esquerda();
        break;
      
      > NUNCA SE VAI CONCRETIZAR PORQUE AO CHEGAR AO T VERTICAL 
      ELE  RECONHECIDO COMO UM CRUZAMENTO PRIMEIRO.
      */

      case 1111000 :                        // T ESQUERDA (1111000)
        
        if(caminho[0] == "D") {                                                                                                               // REVER SE ISTO VAI SER UTIL NA OTIMIZAO DO CAMINHO OU NO
          direita(); }

        if(caminho[0] == "E") {
          esquerda(); }

        if(caminho[0] == "F") {
          frente(); }  

        caminho += "E";
        esquerda();
        break;

      case "0001000" :                       // BECO SEM SADA (0001000)
        digitalWrite(motor_esq_avanca, HIGH);
        digitalWrite(motor_dir_avanca, HIGH);   //AVANA 1cm PARA VERIFICAR SE NO SE TRATA DE UM CRUZAMENTO
        delay(100);                                                                                                        // VERIFICAR SE 1/10s CORRESPONDE A 1CM // VERIFICAO PARA SABER SE NO  UM CRUZAMENTO
        digitalWrite(motor_esq_avanca, LOW);
        digitalWrite(motor_dir_avanca, LOW);
        //delay(1000);                                                                                                                                   //VERIFICAR SE ESTE DELAY  NECESSRIO NA PRTICA
       
        if (estado_sensores == "0000000") {                                                                                       /*Dupla verificao para ter a certeza que no  outro tipo de juno*/
          caminho += "G";                                                                                                                                 //PERCEBER SE NA PRTICA PRECISO DA DUPLA VERIFICAO REALMENTE
          girar_180graus();
          frente(); }
        break;

      default :                             // ROBOT PERDIDO (TODAS AS RESTANTES OPES)
        parar();
    }
  }
  while (modo_otimizado == 1) {
    caminho_otimizado();                    // Algoritmo calcula a melhor rota baseado na informao guardada na String caminho

    digitalWrite(buzzer, HIGH);             // Buzzer (2xBIP)
    delay(300);                                                                                                                         // VERIFICAR NA PRTICA SE PRECISA DE MAIS TEMPO PARA O BIP SOAR BEM
    digitalWrite(buzzer, LOW);
    delay(500);
    digitalWrite(buzzer, HIGH);                     
    delay(300);                                                                                                                       // VERIFICAR NA PRTICA SE PRECISA DE MAIS TEMPO PARA O BIP SOAR BEM
    digitalWrite(buzzer, LOW);
                                                                                                                                            //LCD exibe "Caminho Otimizado"
    vai();                                 // Executa a rota mais curta calculada anteriormente
  }
}


/*========== F U N   O  D E  O T I M I Z A   O ==========*/

String caminho_otimizado() {
                                                                                                                                                  //Algoritmo "Left-Hand"
  //String caminho = "EGEEEGFGEEGFEE";

  caminho.replace("EGE", "F");
  caminho.replace("EGF", "D");
  caminho.replace("DGE", "G");
  caminho.replace("FGF", "G");
  caminho.replace("EGD", "G");
  caminho.replace("FGE", "D");
  return caminho;
}

/* ==========   F U N   O  D E  E X E C U   O   ==========
   ========== D O  C A M I N H O  O T I M I Z A D O ==========*/
void vai() {
  unsigned int i = 0; 
  /*
  > Esta varivel que comea em 0, vai encrementando at o numero total de letras 
  que tiverem sido utilizadas para construir o caminho otimizado, ou seja percorre 
  a String toda, letra a letra, e executa a funo atribuida a cada letra conforme o 
  movimento a que correspondem.
  
  Comando charAt: https://www.arduino.cc/reference/en/language/variables/data-types/string/functions/charat/
  > O comando charAt obriga que a varivel usada para definir a posio (i), seja do 
  tipo unsigned int e a variavel que vai fornecer dados (caminho) seja do tipo String.
  */

  if(estado_sensores == "0011000") {                                    
    if(caminho.charAt(i) == "E") {                                                                                                                  //SERVE PARA ATUALIZAR A STRING CAMINHO CASO EXISTAM MUDANAS NO CAMINHO GRAVADO
      esquerda();
      i++; }

    if(caminho.charAt(i) == "D"){
      direita();
      i++; }

    if(caminho.charAt(i) == " F"){
      frente();
      i++ }

    frente();
  }
  if(estado_sensores == "0000000"){
    parar(); }
}

/* ==========   F U N   E S  D E  M O V I M E N T O   ========== */

void frente() {
  digitalWrite(motor_esq_avanca, HIGH);          // Motor esquerdo avana
  digitalWrite(motor_dir_avanca, HIGH);          // Motor direito avana
  delay(1000);                                                                                                                                // VERIFICAR NA PRTICA, SE PRECISA DE MAIS TEMPO (1s = 1cm?)
  digitalWrite(motor_esq_avanca, LOW);           // Motor esquerdo para
  digitalWrite(motor_dir_avanca, LOW);           // Motor direito para
}

void girar_180graus() {
  digitalWrite(motor_esq_avanca, HIGH);          // Motor esquerdo avana
  digitalWrite(motor_dir_recua, HIGH);           // Motor direito recua
  delay(1000);                                                                                                                    // VERIFICAR NA PRTICA, SE PRECISA DE MAIS TEMPO PARA VIRAR TOTALMENTE 180 GRAUS
  digitalWrite(motor_esq_avanca, LOW);           // Motor esquerdo para
  digitalWrite(motor_dir_avanca, LOW);           // Motor direito para
}

void esquerda() {
  digitalWrite(motor_dir_avanca, HIGH);          // Motor direito avana
  digitalWrite(motor_esq_avanca, LOW);           // Motor esquerdo parado
  digitalWrite(motor_esq_recua, LOW);
  delay(1000);                                                                                                                    // VERIFICAR NA PRTICA, SE PRECISA DE MAIS TEMPO PARA VIRAR TOTALMENTE  ESQUERDA
  digitalWrite(motor_dir_avanca, LOW);           // Motor direito para
}

void direita() {
  digitalWrite(motor_esq_avanca, HIGH);          // Motor esquerdo avana
  digitalWrite(motor_dir_avanca, LOW);           // Motor direito parado
  digitalWrite(motor_dir_recua, LOW);
  delay(1000);                                                                                                                    // VERIFICAR NA PRTICA, SE PRECISA DE MAIS TEMPO PARA VIRAR TOTALMENTE  DIREITA
  digitalWrite(motor_esq_avanca, LOW);           // Motor esquerdo para
}

void parar() {
  digitalWrite(motor_esq_avanca, LOW);
  digitalWrite(motor_dir_avanca, LOW);            // Motor esquerdo e direito param
  digitalWrite(motor_esq_recua, LOW);
  digitalWrite(motor_dir_recua, LOW);

  digitalWrite(buzzer, HIGH);                     // Buzzer (1xBIP)
  delay(300);                                                                                                                         // VERIFICAR NA PRTICA SE PRECISA DE MAIS TEMPO PARA O BIP SOAR BEM
  digitalWrite(buzzer, LOW);
}
