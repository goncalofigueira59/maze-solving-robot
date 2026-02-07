# Robô Autónomo para Navegação em Labirintos (MegaBot)

<p align="center">
  <img src="https://img.shields.io/badge/Categoria-Projeto%20Académico-blue" alt="Categoria: Projeto Académico">
  <img src="https://img.shields.io/badge/Microcontrolador-PIC18F2550-6C4675?logo=microchip-technology" alt="Microcontrolador: PIC18F2550">
  <img src="https://img.shields.io/badge/Competição-MicroRato-orange" alt="Competição: MicroRato">
  <img src="https://img.shields.io/badge/Software-Proteus-1A548B?logo=proteus-design-suite" alt="Software: Proteus">
</p>

> [Projeto Académico] Robô autónomo (MegaBot ) para navegação em labirintos, desenvolvido para a competição MicroRato (modalidade Pathfinder), e para a UC de Projeto Temático em Eletrónica e Instrumentação. Utiliza um PIC18F2550, sensores TCRT5000 e um algoritmo "Left Hand Rule".

---

## Índice

- Resumo do Projeto
- Enquadramento e Objetivos
- Arquitetura de Hardware
  - Modelação 3D e Estrutura Mecânica
  - Esquema Elétrico e Módulos Eletrónicos
- Arquitetura de Software e Algoritmos
  - Análise do Algoritmo "Maze Solver" (Conceito Inicial)
  - Análise do Algoritmo "Line Follower" (Implementação Final)
- Ensaios, Resultados e Desafios Superados
- Conclusão e Aprendizagens
- Ficheiros do Projeto
- Licença

---

### 1. Resumo do Projeto

Este projeto consistiu no desenvolvimento de um robô completamente autónomo para navegação em labirintos planos, orientado por linhas pretas. O sistema é controlado por um microcontrolador **PIC18F2550** e todos os circuitos de decisão estão integrados numa PCB personalizada, com o design elétrico realizado no software **Proteus**.

O objetivo inicial era desenvolver um robô para a modalidade **Pathfinder** da competição MicroRato, capaz de aprender o trajeto numa primeira passagem (usando o algoritmo **"Left Hand Rule"**) e otimizá-lo numa segunda volta. No entanto, devido a desafios práticos de hardware e software, a solução final evoluiu para um **robô seguidor de linha (Line Follower)** funcional e estável, que representa uma base sólida para melhorias futuras.

### 2. Enquadramento e Objetivos

Desenvolvido no âmbito do **Projeto Temático em Eletrónica e Instrumentação** (Licenciatura em Eletrónica e Mecânica Industrial, ESTGA-UA), este trabalho reuniu conhecimentos das disciplinas de Instrumentação e Controlo, Eletrónica e Microcontroladores e Sistemas Digitais.

- **Modalidade:** Pathfinder (Concurso MicroRato)
- **Robô:** Dimensões de 20x20x30 cm, conforme regulamento.
- **Labirinto:** Pista de simulação com linhas pretas de 34 mm de largura e 25 cm entre interseções.

### 3. Arquitetura de Hardware

#### 3.1. Modelação 3D e Estrutura Mecânica
O chassis e as rodas foram desenhados em **SolidEdge** e impressos em 3D com filamento PLA.
- **Chassis:** Design elipsoidal-esférico para um centro de gravidade baixo, minimizando interferências de luz ambiente nos sensores inferiores.
- **Rodas:** Design modificado para atingir um diâmetro de 17,4 cm, permitindo uma velocidade máxima teórica de 52,2 cm/s com motores de 180 RPM.
- **Apoio:** Uma roda esférica omnidirecional metálica foi usada como terceiro ponto de apoio para facilitar a rotação e aumentar o peso base do robô, melhorando a estabilidade.

#### 3.2. Esquema Elétrico e Módulos Eletrónicos
O circuito foi projetado de forma modular no software **Proteus**, garantindo que cada subsistema pudesse ser testado individualmente.

![Esquema Elétrico Geral](project-files/Esquema-elétrico.pdf)

- **Microcontrolador:** Placa "StartUSB for PIC" com o **PIC18F2550**, escolhido pela familiaridade e capacidade de controlo PWM.
- **Alimentação:**
  - Bateria Li-Po 3S1P de 11.1V (2200mAh).
  - **BMS (Battery Management System)** 20A-3S para proteção contra sobrecargas e descargas.
  - Regulador de tensão **7805** para converter os 12V da bateria para os 5V estáveis necessários para o PIC e os sensores.
- **Sensores:**
  - 7x Sensores Óticos Reflexivos **TCRT5000** para deteção da linha preta.
  - O sinal analógico de cada sensor é convertido para digital através de um comparador de tensão **LM339-N**, com um potenciómetro de 10kΩ para ajustar a sensibilidade global.
- **Atuadores:**
  - 2x Motores DC de 12V (37D Metal Gearmotor) com redução 50:1.
  - **Driver Ponte-H L298N** para controlar o sentido e a velocidade (via PWM) dos motores.

### 4. Arquitetura de Software e Algoritmos

Foram desenvolvidos dois algoritmos principais durante o projeto: o conceito inicial do "Maze Solver" e a implementação final do "Line Follower".

#### 4.1. Análise do Algoritmo "Maze Solver" (Conceito Inicial)
O algoritmo conceptual foi desenhado para resolver o labirinto usando a **"Left Hand Rule"**, uma estratégia clássica que prioriza virar à esquerda sempre que possível.

- **Lógica de Decisão:** O código utiliza uma estrutura `if-else if` que define uma hierarquia de movimentos, lendo o estado dos 7 sensores.
- **Mapeamento de Sensores:** A lógica foi invertida (`#define SENSOR_1 !PORTBbits.RB1`), tornando o código mais intuitivo, onde um valor `true` significa que o sensor deteta a linha preta.
- **Hierarquia de Movimentos:**
  1.  **Meta (`if S1 && S2 ... && S7`):** Se todos os sensores detetam preto, o robô para.
  2.  **Virar à Esquerda (`else if S1`):** Se o sensor mais à esquerda deteta uma linha, vira à esquerda (prioridade máxima).
  3.  **Seguir em Frente (`else if !S1 && S3`):** Se não há caminho à esquerda, mas há em frente, segue em frente.
  4.  **Virar à Direita (`else if !S1 && !S3 && S6`):** Se não há caminho à esquerda nem em frente, vira à direita.
  5.  **Meia-Volta (`else if !S1 && !S3 && S4 && !S6`):** Se deteta um beco sem saída (apenas o sensor central S4 ativo), faz uma rotação de 180°.

Este algoritmo representava o plano para a modalidade *Pathfinder*, que incluiria a memorização destes movimentos (`'E'`, `'F'`, `'D'`, `'G'`) num vetor para posterior otimização.

#### 4.2. Análise do Algoritmo "Line Follower" (Implementação Final)
A versão final e funcional do robô implementa um seguidor de linha robusto, focado nos sensores 2 e 5 para correção de trajetória.

- **Configuração do Hardware:**
  - **Pinos:** O `PORTB` é configurado como entrada digital para os sensores. As portas `RA` e `RC` são configuradas como saídas para controlar a direção e a velocidade dos motores.
  - **PWM:** Os módulos `CCP1` e `CCP2` são ativados em modo PWM, com o `Timer2` a gerar uma frequência de controlo de aproximadamente 4.2 kHz, ideal para os motores DC.
- **Lógica de Controlo no `main()` loop:**
  1.  **`if (!(SENSOR_2) && !(SENSOR_5))`**: Se ambos os sensores centrais (2 e 5) detetam a linha, o robô **move-se para a frente**.
  2.  **`else if (!(SENSOR_2) && (SENSOR_5))`**: Se o sensor esquerdo (2) deteta a linha mas o direito (5) não, significa que o robô está a desviar-se para a direita. Ação: **vira à esquerda** para corrigir.
  3.  **`else if ((SENSOR_2) && !(SENSOR_5))`**: Se o sensor direito (5) deteta a linha mas o esquerdo (2) não, o robô está a desviar-se para a esquerda. Ação: **vira à direita** para corrigir.
  4.  **`else if ((SENSOR_2) && (SENSOR_5))`**: Se ambos os sensores estão fora da linha (leem branco), o robô perdeu o trajeto. Ação: **para** por segurança.
- **Debugging:** O código envia continuamente o estado dos 7 sensores via comunicação série, uma ferramenta crucial que permitiu diagnosticar e calibrar o comportamento do robô em tempo real.

### 5. Ensaios, Resultados e Desafios Superados

- **Desafios de Hardware:**
  - **Impressão 3D:** Um erro de conversão de unidades (mm para polegadas) no SolidEdge exigiu a recalibração da exportação `.stl` para obter as dimensões corretas do chassis.
  - **Soldadura:** Foram encontrados problemas de sobreaquecimento de componentes e efeitos capacitivos por excesso de solda, que exigiram a substituição de peças e a refatoração de ligações.
- **Desafios de Software e Sensores:**
  - **Reflexão da Pista:** A fita preta utilizada inicialmente refletia demasiada luz infravermelha, causando leituras incorretas. A solução foi **lixar a superfície da fita** para a tornar mais fosca, melhorando drasticamente a precisão dos sensores.
- **Expectativa vs. Realidade:** A complexidade da integração hardware-software e os desafios de diagnóstico limitaram a implementação do modo *Pathfinder*. A equipa reavaliou os objetivos e focou-se em entregar um protótipo *Line Follower* estável e funcional, o que representa um progresso significativo e uma base sólida para o futuro.

### 6. Conclusão e Aprendizagens

Apesar de o objetivo final ter sido modificado, o projeto foi uma experiência de aprendizagem extremamente valiosa. Consolidou conhecimentos em eletrónica, programação de microcontroladores, desenho mecânico e, crucialmente, na resolução de problemas práticos que surgem na transição da simulação para o mundo real.

**Melhorias Futuras Propostas:**
- Fabricação de uma PCB profissional para garantir a fiabilidade dos circuitos.
- Implementação de um algoritmo de controlo **PID (Proporcional, Integral, Derivativo)**, utilizando os *encoders* dos motores para um movimento mais suave e preciso.
- Finalização do algoritmo *Pathfinder* com memorização e otimização de trajetos.
- Conexão e programação dos botões de controlo para uma operação mais autónoma.

### 7. Ficheiros do Projeto

- **Código Fonte:** Os códigos para o "Maze Solver" (conceito inicial) e "Line Follower" (implementação final) estão disponíveis na pasta `src/`.
- **Documentação e Simulação:** O esquema elétrico do Proteus, os modelos 3D do SolidEdge e outros anexos estão disponíveis na pasta `project-files/` ou em `design/`.

### 8. Licença

Este projeto está licenciado sob a **MIT License**. Veja o ficheiro `LICENSE` para mais detalhes.
