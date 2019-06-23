/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ooo        ooooo             oooo      .    o8o                                  .                                  ooooo                       
`88.       .888'             `888    .o8    `"'                                .o8                                  `888'                       
 888b     d'888  oooo  oooo   888  .o888oo oooo  ooo. .oo.  .oo.    .ooooo.  .o888oo  .ooooo.  oooo d8b              888  ooo. .oo.    .ooooo.  
 8 Y88. .P  888  `888  `888   888    888   `888  `888P"Y88bP"Y88b  d88' `88b   888   d88' `88b `888""8P              888  `888P"Y88b  d88' `88b 
 8  `888'   888   888   888   888    888    888   888   888   888  888ooo888   888   888ooo888  888                  888   888   888  888   888 
 8    Y     888   888   888   888    888 .  888   888   888   888  888    .o   888 . 888    .o  888                  888   888   888  888   888 
o8o        o888o  `V88V"V8P' o888o   "888" o888o o888o o888o o888o `Y8bod8P'   "888" `Y8bod8P' d888b    ooooooooooo o888o o888o o888o `Y8bod8P' 


[+] CODED BY: Fernando C. AKA 'Sargastico' ||  Victor Ariel Garcia [+]

/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//MASTER BOARD (ARDUINO MEGA)

#include <Wire.h>
#include <LiquidCrystal.h>

//Variavéis Globais
LiquidCrystal lcd(7, 6, 2, 3, 4, 5);

#define BUZZER 8 // Ligar o buzzer (Som) no pino 10 A8  
int botao = 8; //Botão para mudar escala (INDUTIMETRO)
int botaomedidor = 9;
int estadobotao = 0; //Variavel de verificação estado botao (INDUTIMETRO)
int acumuladorbotao = 1; //Acumulador botao
int estadobotao_tres = 0; //Variavel de verificação estado botao
int acumuladorbotao_tres = 1; //Acumulador botao
int estadobotaomedidor = 1;
int acumuladorbotaomedidor = 0;
int definirmedidor = 1;
int trava;
int travamedidor;
//--------------------------------------------------
//Variavéis Indutimetro
double pulse, frequency, inductance, capacitance; 
//--------------------------------------------------
//Variavéis Capacimetro
unsigned long startTime;
unsigned long elapsedTime;
float microFarads;
float nanoFarads;
float resistorValue;
int analogPin = A0;// pino analógico para medir a tensão no capacitor
int chargePin1 = 23;// pino para carga do capacitor com resistor de 1k
int chargePin10 = 26;// pino para carga do capacitor com resistor de 10k
int chargePin100 = 25;// pino para carga do capacitor com resistor de 100k
int chargePin1000 = 22;// pino para carga do capacitor com resistor de 1M
int dischargePin = 24;// pino para descarregar capacitor através de resistor de 1k
//--------------------------------------------------
//Variavéis VoltimetroCC
#define pin_escala_1v 27
#define pin_escala_10v 28
#define pin_escala_100v 29
int pin_div = A1;// Defini o pino analógico A1 como entrada da tensão
float R1 = 10000.0;// Valor do resistor R1 para o cálculo da tensão
float R2_1V = 12000.0;// Valor do resistor R2 para o cálculo da tensão
float R2_10V = 7000.0;// Valor do resistor R2 para o cálculo da tensão
float R2_100V = 440.0; // Valor do resistor R2 para o cálculo da tensão
int estado_volt;
//--------------------------------------------------
//Variavéis VoltimetroAC
#define pin_escala_1v_ac 33
#define pin_escala_10v_ac 34
#define pin_escala_100v_ac 35
int pin_div_ac = A3;// Defini o pino analógico A1 como entrada da tensão
float R1_ac = 10000.0;// Valor do resistor R1 para o cálculo da tensão
float R2_1V_ac = 12000.0;// Valor do resistor R2 para o cálculo da tensão
float R2_10V_ac = 7000.0;// Valor do resistor R2 para o cálculo da tensão
float R2_100V_ac = 440.0;// Valor do resistor R2 para o cálculo da tensão
int estado_volt_ac;
float tensao_maxima;
//--------------------------------------------------
//Variavéis Ohmimetro
float rx = 0.0;
float r1_10 = 240.0; // escala de 0 a 10 ohm
float r1_100 = 300.0; // escala de 0 a 100 ohm
float r1_1k = 1000.0; // escala de 0 a 1000 ohm
int pino_10 = 30;
int pino_100 = 31;
int pino_1k = 32;
int leitura = 0;
float vx = 0.0;
//--------------------------------------------------
//Variavéis Frequencimetro
float frequencia;
//--------------------------------------------------
//Variavéis AmperimetroCC
#define pin_escala_1mA 36
#define pin_escala_10mA 37
#define pin_escala_100mA 38
int porta_analog = A4;
int recebe_porta;
float corrente;
float corrente_mili;
//--------------------------------------------------
//Variavéis AmperimetroAC
#define pin_escala_1mA_ac 39
#define pin_escala_10mA_ac 40
#define pin_escala_100mA_ac 41
int porta_analog_ac = A5;
int recebe_porta_ac;
float corrente_ac;
float corrente_mili_ac;
float corrente_maxima;
//--------------------------------------------------
void setup() {

  lcd.begin(16, 2);
  lcd.clear();
  Serial.begin(9600);
  pinMode(BUZZER, OUTPUT);
}
//------------------------------------------
void funcaobotao() {
  estadobotao = digitalRead(botao);
  while ((estadobotao) == HIGH && (trava == 0)) {
    delay(40);
    if (estadobotao == HIGH) {
      acumuladorbotao++;
      trava = 1;
    }
  }
  if ((estadobotao) == LOW && (trava == 1)) {
    trava = 0;
  }
  if (acumuladorbotao == 5) {
    acumuladorbotao = 1;
  }
}
//-----------------------------------------
void funcaobotaomedidor() {

  estadobotaomedidor = digitalRead(botaomedidor);
  while ((estadobotaomedidor) == HIGH && (travamedidor == 0)) {
    delay(10);
    if (estadobotaomedidor == HIGH) {
      acumuladorbotaomedidor++;
      travamedidor = 1;
    }
  }
  if ((estadobotaomedidor) == LOW && (travamedidor == 1)) {
    travamedidor = 0;
  }
  if (acumuladorbotaomedidor == 5) {
    acumuladorbotaomedidor = 1;
  }
}
//-----------------------------------------
void funcaobotao_tres() {

  estadobotao_tres = digitalRead(botao);
  while ((estadobotao_tres) == HIGH && (trava == 0)) {
    delay(40);
    if (estadobotao_tres == HIGH) {
      acumuladorbotao_tres++;
      trava = 1;
    }
  }
  if ((estadobotao_tres) == LOW && (trava == 1)) {
    trava = 0;
  }
  if (acumuladorbotao_tres == 4) {
    acumuladorbotao_tres = 1;
  }
}
//=============================================
void indutimetro() {

  acumuladorbotao == 1;
  if (acumuladorbotaomedidor == 1) {
    funcaobotaomedidor();
    acumuladorbotao == 1;
    pinMode(11, INPUT);
    pinMode(12, INPUT);
    pinMode(13, OUTPUT);
  }

  while (acumuladorbotaomedidor == 1) {
    funcaobotaomedidor();
    digitalWrite(13, HIGH);
    delay(10);
    digitalWrite(13, LOW);
    delayMicroseconds(10); 
    pulse = pulseIn(11, HIGH, 5000); 
    funcaobotao();
    if (pulse < 0.1)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Indutimetro");
      lcd.setCursor(0, 1);
      lcd.print("Escala:");
      lcd.print(acumuladorbotao);
      if (acumuladorbotao == 1) {
        lcd.setCursor(12, 1);
        lcd.print("10uH");
      }
      if (acumuladorbotao == 2) {
        lcd.setCursor(11, 1);
        lcd.print("100uH");
      }
      if (acumuladorbotao == 3) {
        lcd.setCursor(12, 1);
        lcd.print("10mH");
      }
      if (acumuladorbotao == 4) {
        lcd.setCursor(11, 1);
        lcd.print("100mH");
      }
      delay(100);
    }

    if (pulse > 0.1) { 

      capacitance = 2.E-6; 
      frequency = 1.E6 / (2 * pulse);
      inductance = (1. / (capacitance * frequency * frequency * 4.*3.14159 * 3.14159)); 
      inductance *= 1E6;
      Serial.println(inductance);
      delay(300);

      if (acumuladorbotao == 1) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Indutancia:");
        lcd.setCursor(12, 0);
        lcd.print("ES:");
        lcd.print(acumuladorbotao);
        lcd.setCursor(0, 1);
        lcd.print(inductance - 4.10);
        lcd.print("uH");
        lcd.setCursor(12, 1);
        lcd.print("10uH");
        delay(50);
      }
      if (acumuladorbotao == 2) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Indutancia:");
        lcd.setCursor(12, 0);
        lcd.print("ES:");
        lcd.print(acumuladorbotao);
        lcd.setCursor(0, 1);
        lcd.print(inductance - 4.10);
        lcd.print("uH");
        lcd.setCursor(11, 1);
        lcd.print("100uH");
        delay(50);
      }

      if (acumuladorbotao == 3) {
        inductance = inductance / 1000;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Indutancia:");
        lcd.setCursor(12, 0);
        lcd.print("ES:");
        lcd.print(acumuladorbotao);
        lcd.setCursor(0, 1);
        lcd.print(inductance - 0.0041);
        lcd.print("mH");
        lcd.setCursor(12, 1);
        lcd.print("10mH");
        delay(50);
      }

      if (acumuladorbotao == 4) {
        inductance = inductance / 1000;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Indutancia:");
        lcd.setCursor(12, 0);
        lcd.print("ES:");
        lcd.print(acumuladorbotao);
        lcd.setCursor(0, 1);
        lcd.print(inductance - 0.0041);
        lcd.print("mH");
        lcd.setCursor(11, 1);
        lcd.print("100mH");
        delay(50);
      }
    }
  }
}
//-----------------------------------------------------
void descarregaCap() {

  pinMode(chargePin10, INPUT);
  pinMode(chargePin100, INPUT);
  pinMode(chargePin1000, INPUT);
  pinMode(chargePin1, INPUT);
  pinMode(dischargePin, OUTPUT);
  digitalWrite(dischargePin, LOW);
  while (analogRead(analogPin) > 0);
  pinMode(dischargePin, INPUT);
  funcaobotao();
  funcaobotaomedidor();
}
//=========================================================
void capacimetro() {

  acumuladorbotao == 1;
  funcaobotaomedidor();
  pinMode(chargePin10, INPUT);
  pinMode(chargePin100, INPUT);
  pinMode(chargePin1000, INPUT);
  pinMode(chargePin1, OUTPUT);
  digitalWrite(chargePin1, HIGH);

  while (acumuladorbotaomedidor == 2) {
    funcaobotao();
    funcaobotaomedidor();
    descarregaCap();
    if (microFarads < 0.01) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Capacimetro");
      lcd.setCursor(0, 1);
      lcd.print("Escala:");
      lcd.print(acumuladorbotao);
      if (acumuladorbotao == 1) {
        lcd.setCursor(12, 1);
        lcd.print("10nF");
      }
      if (acumuladorbotao == 2) {
        lcd.setCursor(11, 1);
        lcd.print("100nF");
      }
      if (acumuladorbotao == 3) {
        lcd.setCursor(13, 1);
        lcd.print("1uF");
      }
      if (acumuladorbotao == 4) {
        lcd.setCursor(12, 1);
        lcd.print("10uF");
      }
      delay(100);
    }
    if (acumuladorbotao == 1) {
      pinMode(chargePin10, INPUT);
      pinMode(chargePin100, INPUT);
      pinMode(chargePin1000, INPUT);
      pinMode(chargePin1, OUTPUT);    
      digitalWrite(chargePin1, HIGH); 

      startTime = micros();

      while (analogRead(analogPin) < 648); 
      elapsedTime = micros() - startTime;
      microFarads = ((float)elapsedTime / 1000000);
      nanoFarads = (microFarads * 1000) - 0.228;
      funcaobotaomedidor();
      funcaobotao();
      if (microFarads > 0.0005) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Cap.:");
        lcd.setCursor(12, 0);
        lcd.print("ES:");
        lcd.print(acumuladorbotao);
        lcd.setCursor(0, 1);
        lcd.print(nanoFarads);
        lcd.print("nF");
        lcd.setCursor(12, 1);
        lcd.print("10nF");
        delay(100);
      }
    }

    if (acumuladorbotao == 2) {
      pinMode(chargePin1, INPUT);
      pinMode(chargePin100, INPUT);
      pinMode(chargePin1000, INPUT);
      pinMode(chargePin10, OUTPUT);    
      digitalWrite(chargePin10, HIGH);

      startTime = micros();

      while (analogRead(analogPin) < 648); // 647 é 63.2% de 1023 que é o valor máximo da entrada analógica
      elapsedTime = micros() - startTime;
      microFarads = ((float)elapsedTime / 100000);
      funcaobotaomedidor();
      funcaobotao();
      nanoFarads = ((microFarads * 1000) - 1.16);
      if (microFarads > 0.008) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Cap.:");
        lcd.setCursor(12, 0);
        lcd.print("ES:");
        lcd.print(acumuladorbotao);
        lcd.setCursor(0, 1);
        lcd.print(nanoFarads);
        lcd.print("nF");
        lcd.setCursor(11, 1);
        lcd.print("100nF");
        delay(100);
      }
    }

    if (acumuladorbotao == 3) {
      pinMode(chargePin1, INPUT);
      pinMode(chargePin10, INPUT);
      pinMode(chargePin1000, INPUT);
      pinMode(chargePin100, OUTPUT);    
      digitalWrite(chargePin100, HIGH); 
 
      startTime = micros();

      while (analogRead(analogPin) < 648); // 647 é 63.2% de 1023 que é o valor máximo da entrada analógica
      elapsedTime = micros() - startTime;

      microFarads = ((float)elapsedTime / 10000) - 0.01;

      funcaobotaomedidor();
      funcaobotao();
      if (microFarads > 0.01) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Cap.:");
        lcd.setCursor(12, 0);
        lcd.print("ES:");
        lcd.print(acumuladorbotao);
        lcd.setCursor(0, 1);
        lcd.print(microFarads);
        lcd.print("uF");
        lcd.setCursor(13, 1);
        lcd.print("1uF");
        delay(100);
      }
    }

    if (acumuladorbotao == 4) {
      pinMode(chargePin1, INPUT);
      pinMode(chargePin10, INPUT);
      pinMode(chargePin100, INPUT);
      pinMode(chargePin1000, OUTPUT);    
      digitalWrite(chargePin1000, HIGH); 

      startTime = micros();

      while (analogRead(analogPin) < 648); // 647 é 63.2% de 1023 que é o valor máximo da entrada analógica
      elapsedTime = micros() - startTime;

      microFarads = (((float)elapsedTime / 1000) - 0.11);

      funcaobotaomedidor();
      funcaobotao();
      if (microFarads > 0.01) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Cap.:");
        lcd.setCursor(12, 0);
        lcd.print("ES:");
        lcd.print(acumuladorbotao);
        lcd.setCursor(0, 1);
        lcd.print(microFarads);
        lcd.print("uF");
        lcd.setCursor(12, 1);
        lcd.print("10uF");
        delay(100);
      }
    }
  }
}
//========================================================
void voltimetrocc() {

  pinMode(pin_div, INPUT);
  funcaobotao_tres();
  funcaobotaomedidor();

  while (acumuladorbotaomedidor == 5) {
    funcaobotao_tres();
    funcaobotaomedidor();
    if (analogRead(pin_div) < 1)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Voltimetro CC");
      lcd.setCursor(0, 1);
      lcd.print("Escala:");
      lcd.print(acumuladorbotao_tres);

      if (acumuladorbotao_tres == 1) {
        lcd.setCursor(14, 1);
        lcd.print("1V");
      }
      if (acumuladorbotao_tres == 2) {
        lcd.setCursor(13, 1);
        lcd.print("10V");
      }
      if (acumuladorbotao_tres == 3) {
        lcd.setCursor(12, 1);
        lcd.print("100V");
      }
      delay(100);
    }

    if (analogRead(pin_div > 3)) {

      if (acumuladorbotao_tres == 1) {
        funcaobotao_tres();
        funcaobotaomedidor();

        //COMEÇA DA MAIOR ESCALA INDEPENDETEMENTE, PARA EVITAR QUEIMAR O MULTIMETRO
        pinMode(pin_escala_1v, INPUT);
        pinMode(pin_escala_10v, INPUT);
        pinMode(pin_escala_100v, OUTPUT);
        digitalWrite(pin_escala_100v, LOW);
        float v_lido = 0;// Declara uma variável para leitura dos valores analógicos
        float v_medido = 0;// Variável que armazenará a média das amostras da tensão cc
        float v_conv = 0;
        v_lido = analogRead(pin_div);// Lê o valor analógico e armazena na variável v_lido
        v_medido = (v_lido * (R1 + R2_100V)) / (R2_100V); // Faz o cálculo da tensão medida correspondente a leitura analógica
        v_conv = ((v_medido * 5) / 1023);// Somas dos valores de tensão medidos

        //VERIFICA SE ESTÁ NA ESCALA CERTA PARA NÃO QUEIMAR O MULTIMETRO
        if (v_conv <= 2) {
          pinMode(pin_escala_1v, OUTPUT);
          pinMode(pin_escala_10v, INPUT);
          pinMode(pin_escala_100v, INPUT);
          digitalWrite(pin_escala_1v, LOW);
          float v_lido = 0;// Declara uma variável para leitura dos valores analógicos
          float v_medido = 0;// Variável que armazenará a média das amostras da tensão cc
          float v_conv = 0;
          v_lido = analogRead(pin_div);// Lê o valor analógico e armazena na variável v_lido
          v_medido = (v_lido * (R1 + R2_1V)) / (R2_1V);// Faz o cálculo da tensão medida correspondente a leitura analógica
          v_conv = ((v_medido * 5) / 1023) * 1000;// Somas dos valores de tensão medidos
          // Calcula a média de todos os valores de tensão
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Tensao CC:");
          lcd.setCursor(12, 0);
          lcd.print("ES:");
          lcd.print(acumuladorbotao_tres);
          lcd.setCursor(0, 1);
          lcd.print(v_conv);
          lcd.print("mV");
          lcd.setCursor(14, 1);
          lcd.print("1V");
          delay(500);
        }
        else {
          while (acumuladorbotao_tres == 1) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("ALERTA ESCALA!");
            lcd.setCursor(0, 1);
            lcd.print("ESCALA:");
            lcd.print(acumuladorbotao_tres);
            delay(10);
            funcaobotao_tres();
            funcaobotaomedidor();
            digitalWrite(BUZZER, HIGH);//liga o buzzer
            delay(10); //fica com ele ligado durante 200 milissegundos
            digitalWrite(BUZZER, LOW); //desliga o buzzer
            delay(200);
          }
        }
      }

      if (acumuladorbotao_tres == 2) {
        funcaobotao_tres();
        funcaobotaomedidor();

        //COMEÇA DA MAIOR ESCALA INDEPENDETEMENTE, PARA EVITAR QUEIMAR O MULTIMETRO
        pinMode(pin_escala_1v, INPUT);
        pinMode(pin_escala_10v, INPUT);
        pinMode(pin_escala_100v, OUTPUT);
        digitalWrite(pin_escala_100v, LOW);
        float v_lido = 0;// Declara uma variável para leitura dos valores analógicos
        float v_medido = 0;// Variável que armazenará a média das amostras da tensão cc
        float v_conv = 0;
        v_lido = analogRead(pin_div);// Lê o valor analógico e armazena na variável v_lido
        v_medido = (v_lido * (R1 + R2_100V)) / (R2_100V); // Faz o cálculo da tensão medida correspondente a leitura analógica
        v_conv = ((v_medido * 5) / 1023);// Somas dos valores de tensão medidos

        //VERIFICA SE ESTÁ NA ESCALA CERTA PARA NÃO QUEIMAR O MULTIMETRO
        if (v_conv <= 10) {
          pinMode(pin_escala_1v, INPUT);
          pinMode(pin_escala_10v, OUTPUT);
          pinMode(pin_escala_100v, INPUT);
          digitalWrite(pin_escala_10v, LOW);
          float v_lido = 0;// Declara uma variável para leitura dos valores analógicos
          float v_medido = 0;// Variável que armazenará a média das amostras da tensão cc
          float v_conv = 0;
          v_lido = analogRead(pin_div);// Lê o valor analógico e armazena na variável v_lido
          v_medido = (v_lido * (R1 + R2_10V)) / (R2_10V); // Faz o cálculo da tensão medida correspondente a leitura analógica
          v_conv = ((v_medido * 5) / 1023);// Somas dos valores de tensão medidos
          // Calcula a média de todos os valores de tensão
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Tensao CC:");
          lcd.setCursor(12, 0);
          lcd.print("ES:");
          lcd.print(acumuladorbotao_tres);
          lcd.setCursor(0, 1);
          lcd.print(v_conv);
          lcd.print("V");
          lcd.setCursor(13, 1);
          lcd.print("10V");
          Serial.println("Tensao VCC:");
          Serial.println(v_conv);
          delay(500);
        }
        else {
          while (acumuladorbotao_tres == 2) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("ALERTA ESCALA!");
            lcd.setCursor(0, 1);
            lcd.print("ESCALA:");
            lcd.print(acumuladorbotao_tres);
            delay(10);
            funcaobotao_tres();
            funcaobotaomedidor();
            digitalWrite(BUZZER, HIGH);//liga o buzzer
            delay(10); //fica com ele ligado durante 200 milissegundos
            digitalWrite(BUZZER, LOW); //desliga o buzzer
            delay(20);
          }
        }
      }

      if (acumuladorbotao_tres == 3) {
        funcaobotao_tres();
        funcaobotaomedidor();
        pinMode(pin_escala_1v, INPUT);
        pinMode(pin_escala_10v, INPUT);
        pinMode(pin_escala_100v, OUTPUT);
        digitalWrite(pin_escala_100v, LOW);
        float v_lido = 0;// Declara uma variável para leitura dos valores analógicos
        float v_medido = 0;// Variável que armazenará a média das amostras da tensão cc
        float v_conv = 0;
        v_lido = analogRead(pin_div);// Lê o valor analógico e armazena na variável v_lido
        v_medido = (v_lido * (R1 + R2_100V)) / (R2_100V); // Faz o cálculo da tensão medida correspondente a leitura analógica
        v_conv = ((v_medido * 5) / 1023);// Somas dos valores de tensão medidos
        // Calcula a média de todos os valores de tensão
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Tensao CC:");
        lcd.setCursor(12, 0);
        lcd.print("ES:");
        lcd.print(acumuladorbotao_tres);
        lcd.setCursor(0, 1);
        lcd.print(v_conv);
        lcd.print("V");
        lcd.setCursor(12, 1);
        lcd.print("100V");
        Serial.println("Tensao VCC:");
        Serial.println(v_conv);
        delay(500);
      }
    }
  }
}
//===============================================
void ohmimetro() {

  funcaobotao_tres();
  funcaobotaomedidor();
  if (acumuladorbotao_tres == 1) {
    pinMode(pino_10, OUTPUT);
    pinMode(pino_100, INPUT);
    pinMode(pino_1k, INPUT);
    digitalWrite(pino_10, HIGH);
    pinMode(A2, INPUT);
    analogReference(INTERNAL1V1);
  }

  funcaobotao_tres();
  funcaobotaomedidor();
  Serial.println(leitura);

  if (leitura > 1014) {
    funcaobotao_tres();
    funcaobotaomedidor();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ohmimetro");
    lcd.setCursor(0, 1);
    lcd.print("Escala:");
    lcd.print(acumuladorbotao);
    if (acumuladorbotao_tres == 1) {
      lcd.setCursor(12, 1);
      lcd.print("10R");
    }
    if (acumuladorbotao_tres == 2) {
      lcd.setCursor(11, 1);
      lcd.print("100R");
    }
    if (acumuladorbotao_tres == 3) {
      lcd.setCursor(13, 1);
      lcd.print("1kR");
    }
    delay(100);
  }

  if (acumuladorbotao_tres == 1) {

    funcaobotao_tres();
    funcaobotaomedidor();
    leitura = analogRead(A2) - 7;
    Serial.println(leitura);
    vx = ((1.1 * leitura) / 1023);
    rx = (-r1_10 * vx) / (vx - 5);
    if (leitura < 1014) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Ohmimetro:");
      lcd.setCursor(12, 0);
      lcd.print("ES:");
      lcd.print(acumuladorbotao_tres);
      lcd.setCursor(0, 1);
      lcd.print(rx);
      lcd.print("R");
      lcd.setCursor(13, 1);
      lcd.print("10R");
      delay(500);
    }
  }

  if (acumuladorbotao_tres == 2) {
    pinMode(pino_10, INPUT);
    pinMode(pino_100, OUTPUT);
    pinMode(pino_1k, INPUT);
    digitalWrite(pino_100, HIGH);
    pinMode(A2, INPUT);
    analogReference(INTERNAL1V1);
  }
  if (acumuladorbotao_tres == 2) {
    funcaobotao_tres();
    funcaobotaomedidor();
    leitura = analogRead(A2) - 4;
    Serial.println(leitura);
    vx = ((1.1 * leitura) / 1023);
    rx = (-r1_100 * vx) / (vx - 5);
    if (leitura < 1014) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Ohmimetro:");
      lcd.setCursor(12, 0);
      lcd.print("ES:");
      lcd.print(acumuladorbotao_tres);
      lcd.setCursor(0, 1);
      lcd.print(rx);
      lcd.print("R");
      lcd.setCursor(12, 1);
      lcd.print("100R");
      delay(500);
    }
  }

  if (acumuladorbotao_tres == 3) {
    Serial.println(acumuladorbotaomedidor);
    pinMode(pino_10, INPUT);
    pinMode(pino_100, INPUT);
    pinMode(pino_1k, OUTPUT);
    digitalWrite(pino_1k, HIGH);
    analogReference(INTERNAL2V56);
  }
  if (acumuladorbotao_tres == 3) {
    funcaobotao_tres();
    funcaobotaomedidor();
    leitura = analogRead(A2);
    vx = ((2.56 * leitura) / 1023);
    rx = (-r1_1k * vx) / (vx - 5);
    Serial.println(leitura);
    if (leitura < 1014) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Ohmimetro:");
      lcd.setCursor(12, 0);
      lcd.print("ES:");
      lcd.print(acumuladorbotao_tres);
      lcd.setCursor(0, 1);
      lcd.print(rx);
      lcd.print("R");
      lcd.setCursor(13, 1);
      lcd.print("1kR");
      delay(500);
    }
  }
}

void frequencimetro() {

  if (Serial.available()) {
    frequencia = Serial.read();
  }

  if (acumuladorbotao_tres == 1) {
    funcaobotao_tres();
    funcaobotaomedidor();
    frequencia = frequencia;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Frequencia:");
    lcd.setCursor(12, 0);
    lcd.print("ES:");
    lcd.print(acumuladorbotao_tres);
    lcd.setCursor(0, 1);
    lcd.print(frequencia);
    lcd.print("Hz");
    lcd.setCursor(13, 1);
    lcd.print("10Hz");
    delay(500);
  }
  if (acumuladorbotao_tres == 2) {
    funcaobotao_tres();
    funcaobotaomedidor();
    frequencia = frequencia;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Frequencia:");
    lcd.setCursor(12, 0);
    lcd.print("ES:");
    lcd.print(acumuladorbotao_tres);
    lcd.setCursor(0, 1);
    lcd.print(frequencia);
    lcd.print("Hz");
    lcd.setCursor(13, 1);
    lcd.print("100Hz");
    delay(500);
  }
  if (acumuladorbotao_tres == 3) {
    funcaobotao_tres();
    funcaobotaomedidor();
    frequencia = frequencia;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Frequencia:");
    lcd.setCursor(12, 0);
    lcd.print("ES:");
    lcd.print(acumuladorbotao_tres);
    lcd.setCursor(0, 1);
    lcd.print(frequencia);
    lcd.print("Hz");
    lcd.setCursor(13, 1);
    lcd.print("1kHz");
    delay(500);
  }
}
//==============================================
void voltimetroac() {

  pinMode(pin_div_ac, INPUT);
  funcaobotao_tres();
  funcaobotaomedidor();

  while (acumuladorbotaomedidor == 6) {
    funcaobotao_tres();
    funcaobotaomedidor();
    if (analogRead(pin_div_ac) < 1)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Voltimetro AC");
      lcd.setCursor(0, 1);
      lcd.print("Escala:");
      lcd.print(acumuladorbotao_tres);

      if (acumuladorbotao_tres == 1) {
        lcd.setCursor(14, 1);
        lcd.print("1V");
      }
      if (acumuladorbotao_tres == 2) {
        lcd.setCursor(13, 1);
        lcd.print("10V");
      }
      if (acumuladorbotao_tres == 3) {
        lcd.setCursor(12, 1);
        lcd.print("100V");
      }
      delay(100);
    }


    if (analogRead(pin_div_ac > 3)) {

      if (acumuladorbotao_tres == 1) {
        funcaobotao_tres();
        funcaobotaomedidor();

        //COMEÇA DA MAIOR ESCALA INDEPENDETEMENTE, PARA EVITAR QUEIMAR O MULTIMETRO
        pinMode(pin_escala_1v_ac, INPUT);
        pinMode(pin_escala_10v_ac, INPUT);
        pinMode(pin_escala_100v_ac, OUTPUT);
        digitalWrite(pin_escala_100v_ac, LOW);
        float v_lido = 0;// Declara uma variável para leitura dos valores analógicos
        float v_medido = 0;// Variável que armazenará a média das amostras da tensão cc
        float v_conv = 0;
        v_lido = analogRead(pin_div_ac);// Lê o valor analógico e armazena na variável v_lido
        v_medido = (v_lido * (R1 + R2_100V)) / (R2_100V); // Faz o cálculo da tensão medida correspondente a leitura analógica
        v_conv = ((v_medido * 5) / 1023);//Somas dos valores de tensão medidos

        //VERIFICA SE ESTÁ NA ESCALA CERTA PARA NÃO QUEIMAR O MULTIMETRO
        if (v_conv <= 2) {

          for (int i = 0 ; i < 40 ; i++) {
            funcaobotao_tres();
            funcaobotaomedidor();

            pinMode(pin_escala_1v_ac, OUTPUT);
            pinMode(pin_escala_10v_ac, INPUT);
            pinMode(pin_escala_100v_ac, INPUT);
            digitalWrite(pin_escala_1v_ac, LOW);
            float v_lido = 0;// Declara uma variável para leitura dos valores analógicos
            float v_medido = 0;// Variável que armazenará a média das amostras da tensão cc
            float v_conv = 0;
            v_lido = analogRead(pin_div_ac);// Lê o valor analógico e armazena na variável v_lido
            v_medido = (v_lido * (R1 + R2_1V)) / (R2_1V);// Faz o cálculo da tensão medida correspondente a leitura analógica
            v_conv = ((v_medido * 5) / 1023) * 1000;// Somas dos valores de tensão medidos

            if (v_conv > tensao_maxima) {
              tensao_maxima = v_conv;
            }

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Tensao AC:");
            lcd.setCursor(12, 0);
            lcd.print("ES:");
            lcd.print(acumuladorbotao_tres);
            lcd.setCursor(0, 1);
            lcd.print(tensao_maxima/sqrt(2)); // Monstra valor RMS
            lcd.print("mV");
            lcd.setCursor(14, 1);
            lcd.print("1V");
            delay(500);
          }
        }
        else {
          while (acumuladorbotao_tres == 1) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("ALERTA ESCALA!");
            lcd.setCursor(0, 1);
            lcd.print("ESCALA:");
            lcd.print(acumuladorbotao_tres);
            delay(10);
            funcaobotao_tres();
            funcaobotaomedidor();
            digitalWrite(BUZZER, HIGH);//liga o buzzer
            delay(10); //fica com ele ligado durante 200 milissegundos
            digitalWrite(BUZZER, LOW); //desliga o buzzer
            delay(200);
          }
        }
      }

      if (acumuladorbotao_tres == 2) {
        funcaobotao_tres();
        funcaobotaomedidor();

        //COMEÇA DA MAIOR ESCALA INDEPENDETEMENTE, PARA EVITAR QUEIMAR O MULTIMETRO
        pinMode(pin_escala_1v_ac, INPUT);
        pinMode(pin_escala_10v_ac, INPUT);
        pinMode(pin_escala_100v_ac, OUTPUT);
        digitalWrite(pin_escala_100v_ac, LOW);
        float v_lido = 0;                            // Declara uma variável para leitura dos valores analógicos
        float v_medido = 0;                          // Variável que armazenará a média das amostras da tensão cc
        float v_conv = 0;
        v_lido = analogRead(pin_div_ac);              // Lê o valor analógico e armazena na variável v_lido
        v_medido = (v_lido * (R1 + R2_100V)) / (R2_100V); // Faz o cálculo da tensão medida correspondente a leitura analógica
        v_conv = ((v_medido * 5) / 1023);         // Somas dos valores de tensão medidos

        //VERIFICA SE ESTÁ NA ESCALA CERTA PARA NÃO QUEIMAR O MULTIMETRO
        if (v_conv <= 10) {

          for (int i = 0 ; i < 40 ; i++) {
            pinMode(pin_escala_1v_ac, INPUT);
            pinMode(pin_escala_10v_ac, OUTPUT);
            pinMode(pin_escala_100v_ac, INPUT);
            digitalWrite(pin_escala_10v_ac, LOW);
            float v_lido = 0;                            // Declara uma variável para leitura dos valores analógicos
            float v_medido = 0;                          // Variável que armazenará a média das amostras da tensão cc
            float v_conv = 0;
            v_lido = analogRead(pin_div_ac);              // Lê o valor analógico e armazena na variável v_lido
            v_medido = (v_lido * (R1 + R2_10V)) / (R2_10V); // Faz o cálculo da tensão medida correspondente a leitura analógica
            v_conv = ((v_medido * 5) / 1023);         // Somas dos valores de tensão medidos

            if (v_conv > tensao_maxima) {
              tensao_maxima = v_conv;
            }

            // Calcula a média de todos os valores de tensão
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Tensao AC:");
            lcd.setCursor(12, 0);
            lcd.print("ES:");
            lcd.print(acumuladorbotao_tres);
            lcd.setCursor(0, 1);
            lcd.print(tensao_maxima/sqrt(2));
            lcd.print("V");
            lcd.setCursor(13, 1);
            lcd.print("10V");
            delay(500);
          }
        }
        else {
          while (acumuladorbotao_tres == 2) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("ALERTA ESCALA!");
            lcd.setCursor(0, 1);
            lcd.print("ESCALA:");
            lcd.print(acumuladorbotao_tres);
            delay(10);
            funcaobotao_tres();
            funcaobotaomedidor();
            digitalWrite(BUZZER, HIGH);//liga o buzzer
            delay(10); //fica com ele ligado durante 200 milissegundos
            digitalWrite(BUZZER, LOW); //desliga o buzzer
            delay(20);
          }
        }
      }

      if (acumuladorbotao_tres == 3) {
        funcaobotao_tres();
        funcaobotaomedidor();
        for (int i = 0 ; i < 40 ; i++) {
          pinMode(pin_escala_1v_ac, INPUT);
          pinMode(pin_escala_10v_ac, INPUT);
          pinMode(pin_escala_100v_ac, OUTPUT);
          digitalWrite(pin_escala_100v_ac, LOW);
          float v_lido = 0;// Declara uma variável para leitura dos valores analógicos
          float v_medido = 0;// Variável que armazenará a média das amostras da tensão cc
          float v_conv = 0;
          v_lido = analogRead(pin_div_ac);// Lê o valor analógico e armazena na variável v_lido
          v_medido = (v_lido * (R1 + R2_100V)) / (R2_100V);// Faz o cálculo da tensão medida correspondente a leitura analógica
          v_conv = ((v_medido * 5) / 1023);// Somas dos valores de tensão medidos
          // Calcula a média de todos os valores de tensão
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Tensao AC:");
          lcd.setCursor(12, 0);
          lcd.print("ES:");
          lcd.print(acumuladorbotao_tres);
          lcd.setCursor(0, 1);
          lcd.print(tensao_maxima/sqrt(2));
          lcd.print("V");
          lcd.setCursor(12, 1);
          lcd.print("100V");
          delay(500);
        }
      }
    }
  }
}
//========================================================================================
void amperimetrocc() {

  pinMode(porta_analog, INPUT);
  pinMode(pin_escala_1mA, INPUT);
  pinMode(pin_escala_10mA, INPUT);
  pinMode(pin_escala_100mA, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  funcaobotao();
  funcaobotaomedidor();
  while (acumuladorbotaomedidor == 7) {
    if (acumuladorbotao_tres == 1) {
      funcaobotao_tres();
      funcaobotaomedidor();
      if (corrente_mili <= 3) {
        pinMode(pin_escala_1mA, OUTPUT);
        pinMode(pin_escala_10mA, INPUT);
        pinMode(pin_escala_100mA, INPUT);
        digitalWrite(pin_escala_1mA, LOW);
        recebe_porta = analogRead(porta_analog);
        corrente = (0.001 * recebe_porta) / 996;
        corrente_mili = (corrente * 1000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Corrente CC:");
        lcd.setCursor(12, 0);
        lcd.print("ES:");
        lcd.print(acumuladorbotao_tres);
        lcd.setCursor(0, 1);
        lcd.print(corrente_mili);
        lcd.print("mA");
        lcd.setCursor(14, 1);
        lcd.print("1mA");
        delay(500);
      }
    }

    if (acumuladorbotao_tres == 2) {
      funcaobotao_tres();
      funcaobotaomedidor();
      pinMode(pin_escala_1mA, INPUT);
      pinMode(pin_escala_10mA, OUTPUT);
      pinMode(pin_escala_100mA, INPUT);
      digitalWrite(pin_escala_10mA, LOW);
      recebe_porta = analogRead(porta_analog);
      corrente = (0.01 * recebe_porta) / 762;
      corrente_mili = (corrente * 1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Corrente CC:");
      lcd.setCursor(12, 0);
      lcd.print("ES:");
      lcd.print(acumuladorbotao_tres);
      lcd.setCursor(0, 1);
      lcd.print(corrente_mili);
      lcd.print("mA");
      lcd.setCursor(14, 1);
      lcd.print("10mA");
      delay(500);
    }

    if (acumuladorbotao_tres == 3) {
      funcaobotao_tres();
      funcaobotaomedidor();
      pinMode(pin_escala_1mA, INPUT);
      pinMode(pin_escala_10mA, INPUT);
      pinMode(pin_escala_100mA, OUTPUT);
      digitalWrite(pin_escala_100mA, LOW);
      recebe_porta = analogRead(porta_analog);
      corrente = (0.1 * recebe_porta) / 807;
      corrente_mili = (corrente * 1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Corrente CC:");
      lcd.setCursor(12, 0);
      lcd.print("ES:");
      lcd.print(acumuladorbotao_tres);
      lcd.setCursor(0, 1);
      lcd.print(corrente_mili);
      lcd.print("mA");
      lcd.setCursor(14, 1);
      lcd.print("100mA");
      delay(500);
    }
  }
}
//========================================================================================
void amperimetroac() {

  pinMode(porta_analog_ac, INPUT);                    
  pinMode(pin_escala_1mA_ac, INPUT);
  pinMode(pin_escala_10mA_ac, INPUT);
  pinMode(pin_escala_100mA_ac, INPUT);
  lcd.begin(16, 2);
  lcd.clear();
  funcaobotao();
  funcaobotaomedidor();
  while (acumuladorbotaomedidor == 8) {
    if (acumuladorbotao_tres == 1) {
      funcaobotao_tres();
      funcaobotaomedidor();
      if (corrente_mili <= 3) {
        for (int i = 0 ; i < 40 ; i++) {
          pinMode(pin_escala_1mA_ac, OUTPUT);
          pinMode(pin_escala_10mA_ac, INPUT);
          pinMode(pin_escala_100mA_ac, INPUT);
          digitalWrite(pin_escala_1mA_ac, LOW);
          recebe_porta_ac = analogRead(porta_analog_ac);
          corrente_ac = (0.001 * recebe_porta_ac) / 996;
          corrente_mili_ac = (corrente_ac * 1000);

          if (corrente_mili_ac > corrente_maxima) {
            corrente_maxima = corrente_mili_ac;
          }
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Corrente AC:");
          lcd.setCursor(12, 0);
          lcd.print("ES:");
          lcd.print(acumuladorbotao_tres);
          lcd.setCursor(0, 1);
          lcd.print(corrente_maxima);
          lcd.print("mA");
          lcd.setCursor(14, 1);
          lcd.print("1mA");
          delay(500);
        }
      }
    }

    if (acumuladorbotao_tres == 2) {
      funcaobotao_tres();
      funcaobotaomedidor();
      for (int i = 0 ; i < 40 ; i++) {
        pinMode(pin_escala_1mA_ac, INPUT);
        pinMode(pin_escala_10mA_ac, OUTPUT);
        pinMode(pin_escala_100mA_ac, INPUT);
        digitalWrite(pin_escala_10mA_ac, LOW);
        recebe_porta_ac = analogRead(porta_analog_ac);
        corrente_ac = (0.01 * recebe_porta_ac) / 762;
        corrente_mili_ac = (corrente_ac * 1000);
        if (corrente_mili_ac > corrente_maxima) {
          corrente_maxima = corrente_mili_ac;
        }
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Corrente CC:");
        lcd.setCursor(12, 0);
        lcd.print("ES:");
        lcd.print(acumuladorbotao_tres);
        lcd.setCursor(0, 1);
        lcd.print(corrente_maxima);
        lcd.print("mA");
        lcd.setCursor(14, 1);
        lcd.print("10mA");
        delay(500);
      }
    }

    if (acumuladorbotao_tres == 3) {
      funcaobotao_tres();
      funcaobotaomedidor();
      for (int i = 0 ; i < 40 ; i++) {
        pinMode(pin_escala_1mA_ac, INPUT);
        pinMode(pin_escala_10mA_ac, INPUT);
        pinMode(pin_escala_100mA_ac, OUTPUT);
        digitalWrite(pin_escala_100mA_ac, LOW);
        recebe_porta_ac = analogRead(porta_analog_ac);
        corrente_ac = (0.1 * recebe_porta_ac) / 807;
        corrente_mili_ac = (corrente_ac * 1000);
        if (corrente_mili_ac > corrente_maxima) {
          corrente_maxima = corrente_mili_ac;
        }
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Corrente CC:");
        lcd.setCursor(12, 0);
        lcd.print("ES:");
        lcd.print(acumuladorbotao_tres);
        lcd.setCursor(0, 1);
        lcd.print(corrente_maxima);
        lcd.print("mA");
        lcd.setCursor(14, 1);
        lcd.print("100mA");
        delay(500);
      }
    }
  }
}

void loop() {
  if (acumuladorbotaomedidor == 0) {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("MULTIMETRO");
    lcd.setCursor(4, 1);
    lcd.print("DIGITAL");
    delay(100);

    funcaobotaomedidor();
  }
  if (acumuladorbotaomedidor == 1) {

    indutimetro();
  }

  if (acumuladorbotaomedidor == 2) {

    capacimetro();
  }

  if (acumuladorbotaomedidor == 3) {

    ohmimetro();
  }

  if (acumuladorbotaomedidor == 4) {
    frequencimetro();
  }

  if (acumuladorbotaomedidor == 5) {

    voltimetrocc();
  }

  if (acumuladorbotaomedidor == 6) {

    voltimetroac();
  }

  if (acumuladorbotaomedidor == 7) {

    amperimetrocc();
  }

  if (acumuladorbotaomedidor == 8) {

    amperimetroac();
  }
}
