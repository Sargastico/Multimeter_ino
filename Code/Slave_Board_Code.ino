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

//SLAVE BOARD (ARDUINO NANO)

//Frequencimetro arbitrário até 38.5kHz|| generalized wave freq detection with 38.5kHz sampling.
//Variavel de clipping || clipping indicator variables
boolean clipping = 0;

//Variaveis para armazaenamento || data storage variables
byte newData = 0;
byte prevData = 0;
unsigned int time = 0;// Armazena o tempo e envia valores para o 'timer' || keeps time and sends vales to store in timer[] 
int timer[10];// Armazenamento de tempo de occorencia || storage for timing of 'events'
int slope[10];//Armazenamento de occorrencia de declives || storage for slope of events
unsigned int totalTimer;// Usado para calcular o periodo || used to calculate period
unsigned int period;//Armazenamento para o periodo da onda ||storage for period of wave
byte index = 0;//byte de armazenamento para index momentanea ||current storage index
float frequency;//Armazenamento para calulos da frequencia || storage for frequency calculations
int maxSlope = 0;//Usado para calcular o declive maximo, para usar como um ponto de referencia || used to calculate max slope as trigger point
int newSlope;//Armazenamento para novas incidencias de declive || storage for incoming slope data

//Variaveis para determinar o ponto de referencia || variables for decided whether you have a match
byte noMatch = 0;//counts how many non-matches you've received to reset variables if it's been too long
byte slopeTol = 3;//slope tolerance- adjust this if you need
int timerTol = 10;//timer tolerance- adjust this if you need

//Variavel para detecção de amp || variables for amp detection
unsigned int ampTimer = 0;
byte maxAmp = 0;
byte checkMaxAmp;
byte ampThreshold = 30;//Aumentar caso o sinal possua muito ruido || raise if you have a very noisy signal

void setup(){
  
  Serial.begin(9600);
  
  cli();// Desabilita 'interrupts' || disable 'interrupts'
  
  //Seta uma amostrage continua do pino analógico 0 em 38.5kHz || set up continuous sampling of analog pin 0 at 38.5kHz
 
  //Limpa os registradores ADCSRA e ADCSRB || clear ADCSRA and ADCSRB registers
  ADCSRA = 0;
  ADCSRB = 0;
  
  //Seleção do multiplexador || Multiplexer Selection
  ADMUX |= (1 << REFS0); //Seta a tensão de referencia || set reference voltage
  ADMUX |= (1 << ADLAR); //Alinha o valor de ADC - Para que seja possivel ler somente os 8 maiores bits do registrador ADCH || left align the ADC value- so we can read highest 8 bits from ADCH register only
  
  // Controle de ADC e do status do registrador A || ADC Control and Status Register A
  ADCSRA |= (1 << ADPS2) | (1 << ADPS0); //Seta o clock do ADC para 32 prescaler = 500kHz set || ADC clock with 32 prescaler =500kHz
  ADCSRA |= (1 << ADATE); //Habilita o auto trigger || enabble auto trigger
  ADCSRA |= (1 << ADIE); //Habilita 'interrupts' quando completa a medição || enable 'interrupts' when measurement complete
  ADCSRA |= (1 << ADEN); //Habilita o ADC || enable ADC
  ADCSRA |= (1 << ADSC); //Inicia as medições ADC || start ADC measurements
  
  sei();//Habilita 'interrupts' || enable 'interrupts'
}

ISR(ADC_vect) {//Quando há um novo valor ADC || when new ADC value ready
  
  prevData = newData;//Armazaena o valor anterior || store previous value
  newData = ADCH;//Recebe o valor de A0 || get value from A0
  if (prevData < 127 && newData >=127){//Se está subindo e transpassando o ponto médio || if increasing and crossing midpoint
    newSlope = newData - prevData;//Calcula o delcive || calculate slope
    if (abs(newSlope-maxSlope)<slopeTol){//Verifica se os declives são iguais || if slopes are equal
      //Grava os novos dados e reseta o 'time' || record new data and reset time
      slope[index] = newSlope;
      timer[index] = time;
      time = 0;
      if (index == 0){//Reseta o declive máximo || new max slope just reset
        noMatch = 0;
        index++;//Incremento da index || increment index
      }
      else if (abs(timer[0]-timer[index])<timerTol && abs(slope[0]-newSlope)<slopeTol){//Verifica se a duração do 'timer' e dos declives são iguais || if timer duration and slopes match
        //Valor do temporazidor de soma || sum timer values
        totalTimer = 0;
        for (byte i=0;i<index;i++){
          totalTimer+=timer[i];
        }
        period = totalTimer;//Seta o periodo || set period
        //Reseta o valor da index para comparação || reset new zero index values to compare with
        timer[0] = timer[index];
        slope[0] = slope[index];
        index = 1;//Seta a index para 1 || set index to 1
        noMatch = 0;
      }
      else{//Caso ultrapasse o ponto médio, mas a duração de 'timer' e dos declives são diferentes|| crossing midpoint but not match
        index++;
        if (index > 9){
          reset();
        }
      }
    }
    else if (newSlope>maxSlope){//Verifica se o novo declive não é muito maior que o declive máximo || if new slope is much larger than max slope
      maxSlope = newSlope;
      time = 0;
      noMatch = 0;
      index = 0;
    }
    else{ //Declive não abrupto o suficiente || slope not steep enough
      noMatch++;
      if (noMatch>9){
        reset();
      }
    }
  }

  time++;
  ampTimer++;
  if (abs(127-ADCH)>maxAmp){
  	 maxAmp = abs(127-ADCH);
  }
  if (ampTimer==1000){
    ampTimer = 0;
    checkMaxAmp = maxAmp;
    maxAmp = 0;
  }
  
}

//Reseta algumas variaveis
void reset(){//clear out some variables
  index = 0;
  noMatch = 0;
  maxSlope = 0;

void loop(){
  
  checkClipping();
  
  if (checkMaxAmp>ampThreshold){
    frequency = 38462/float(period);//Calcula a frequncia tempo variação/periodo || calculate frequency timer rate/period
  
  //Imprime a frequencia na serial || Print the frequency on serial monitor 
    Serial.print(frequency);
    Serial.println(" hz");
  }
  delay(100);
}


