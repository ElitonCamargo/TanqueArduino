// Captura de dados bluetooth
#include <SoftwareSerial.h>
SoftwareSerial BT(2, 3); // RX, TX
int Eixo_X=0;
int Eixo_Y=0;


/*Pinagem dos motores Esteira*/
//motor_ESQUERDA
#define IN1 4
#define IN2 5
#define POTENCIA_E 9

//motor_DIREITA
#define IN3 6
#define IN4 7
#define POTENCIA_D 10

//Pinagem para o braço robotico.
//motor 1
#define SERVO1 A1 // Variável Servo - Servo para levantar e baixar o braço
int SERVO1_posicao = 60; // Posição Servo

//motor 2
#define SERVO2 A2 // Variável Servo - Servo do avanço do braço, para frente e para trás.
int SERVO2_posicao = 130; // Posição Servo - Início retraido em 130, vai de 0 a 180;

//motor 3
#define SERVO3 A3 // Variável Servo - Servo da gara
int SERVO3_posicao = 140; // Posição Servo - //Início fechado em 140, vai de 75 a 150;

//motor 4
#define SERVO4 A4 // Variável Servo - Servo da base giratória
int SERVO4_posicao = 80; // Posição Servo //Início centralizado em 80, vai de 0 a 160;


void setup() {
  pinMode(SERVO1, OUTPUT);
  //obs: "moverServo(Servo,posição)" é uma função que criei para não depender de bibliotecas para este fim.
  moverServo(SERVO1,SERVO1_posicao); 

  pinMode(SERVO2, OUTPUT);
  moverServo(SERVO2,SERVO2_posicao);

  pinMode(SERVO3, OUTPUT);
  moverServo(SERVO3,SERVO3_posicao);

  pinMode(SERVO4, OUTPUT);
  moverServo(SERVO4,SERVO4_posicao); 

  delay(1000); // Colocar os Servos na posição ao inicializar o Arduino

  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(POTENCIA_E,OUTPUT);
  pinMode(POTENCIA_D,OUTPUT);
  Serial.begin(9600);
  BT.begin(9600); 
  delay(500);
}

void loop() { 

  if (BT.available() > 1) { // Se o bluetooth enviar pelomentos dois dados (X, Y) inicializamos a captura dos dados
    Eixo_X = BT.read();
    Eixo_Y = BT.read();

    if(Eixo_X < 201 && Eixo_Y < 201){ //Se os dados enviados forem do primeira joystick (Direção do tamque) eles teram valor entre 0 e 200
        Eixo_X -= 100;          //convertendo os dados para um plano cartesiano de origem x=0 e y=0 e range que vai de -100 até 100 para x e y;    
        Eixo_Y = 100 - Eixo_Y;   
    }
  }  
  acao(Eixo_X, Eixo_Y); //A função ação verifica o que deve ser feito ao receber os dados;
    
}

void moverServo(uint8_t servo, int angulo)              
{
    int tempHIGH = map(angulo,0,180,600,2400);
    int tempLOW = 20000 - tempHIGH;
    digitalWrite(servo, HIGH);  
    delayMicroseconds(tempHIGH); 
    digitalWrite(servo, LOW); 
    delayMicroseconds(tempLOW);                               
}

void acao(int x, int y){
  if(x > -101 && x < 101){    
    direcao(x, y);
  }
  if(x==213){ //Levantar braço
    if(SERVO1_posicao < 170){
      printDados(Eixo_X, Eixo_Y);
      SERVO1_posicao++; 
    }  
  }
  if(x==212){ //Baixar braço
    if(SERVO1_posicao > 10){
      printDados(Eixo_X, Eixo_Y);
      SERVO1_posicao--;   
    }  
  }
  if(x==215){ //Avançar braço para frente
    if(SERVO2_posicao < 180){
      printDados(Eixo_X, Eixo_Y);
      SERVO2_posicao++;  
    }  
  }
  if(x==216){ //Retrair braço para trás
    if(SERVO2_posicao > 0){
      printDados(Eixo_X, Eixo_Y);
      SERVO2_posicao--;    
    }    
  }
  if(x==210){ //Abrir gara
    if(SERVO3_posicao > 75){
      printDados(Eixo_X, Eixo_Y);
      SERVO3_posicao--;    
    }    
  }
  if(x==211){ //Fechar gara
    if(SERVO3_posicao < 150){
      printDados(Eixo_X, Eixo_Y);
      SERVO3_posicao++;  
    }  
  }
  if(x==217){ //Girar base para esquerda
    if(SERVO4_posicao > 0){
      printDados(Eixo_X, Eixo_Y);
      SERVO4_posicao--;    
    }    
  }
  if(x==218){ //Girar base para direita
    if(SERVO4_posicao < 161){
      printDados(Eixo_X, Eixo_Y);
      SERVO4_posicao++;  
    }  
  }

  moverServo(SERVO1, SERVO1_posicao);
  moverServo(SERVO2, SERVO2_posicao);
  moverServo(SERVO3, SERVO3_posicao); 
  moverServo(SERVO4, SERVO4_posicao); 
}

void direcao(int x, int y){    
    if(x != 0 | y != 0){// Tanque em movimento
        printDados(Eixo_X, Eixo_Y);   
        if(x > -11 && x < 11){// Andar em linha reta          
            if(y > 0){// Andar para frente            
              int potencia = map(y,0,100,0,255);
              motorDireito(1,potencia);
              motorEsquerdo(1,potencia);
            }
            else{// Andar para trás
              int potencia = map(-y,0,100,0,255);   
              motorDireito(2,potencia);
              motorEsquerdo(2,potencia);
            }
        }
        else if(y < 11 &&y > -11 && x > 10){ // Girando para direita
            int potencia = map(x,0,100,0,255);
            girarParaDireita(potencia);
            
        }
        else if(y < 11 &&y > -11 && x < -10){ // Girando para esquerda
            int potencia = map(-x,0,100,0,255);
            girarParaEsquerda(potencia);          
        }
        else if(y > 10 && x < -10){ //Para frente -> Virando para esquerda
            int potencia = map(y,0,100,0,255);
            motorDireito(1,potencia);
            motorEsquerdo(1,potencia+x*1.3);
        }
        else if(y > 10 && x > 10){ //Para frente -> Virando para direita
            int potencia = map(y,0,100,0,255);
            motorDireito(1,potencia-x*1.3);
            motorEsquerdo(1,potencia);
        }
        else if(y < -10 && x < -10){ //Para trás -> Virando para esquerda
            int potencia = map(y,0,100,0,255);
            motorDireito(2,potencia);
            motorEsquerdo(2,potencia+x*1.3);
        }
        else if(y < -10 && x > 10){ //Para trás -> Virando para direita
            int potencia = map(y,0,100,0,255);
            motorDireito(2,potencia-x*1.3);
            motorEsquerdo(2,potencia);
        }
        
    }
    else{
      // Tanque parado
      frear();
    }
}
void frear(){
  motorEsquerdo(0,0);
  motorDireito(0,0);
}
void girarParaDireita(int potencia){
    motorDireito(2,potencia);
    motorEsquerdo(1,potencia);
}
void girarParaEsquerda(int potencia){
    motorDireito(1,potencia);
    motorEsquerdo(2,potencia);
}
void printDados(int x, int y){
  Serial.print("X:"); 
  Serial.print(x); 
  Serial.print(", Y:"); 
  Serial.println(y);
}
void motorEsquerdo(int sentido, int potencia){
  analogWrite(POTENCIA_E,potencia);
  if(sentido == 0){ //Freio
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,HIGH);
  }
  else{
     if(sentido == 1){ //Para frente
        digitalWrite(IN3,HIGH);
        digitalWrite(IN4,LOW);
      }
      else{ // Para Trás  
        digitalWrite(IN3,LOW);
        digitalWrite(IN4,HIGH);
      }
  }
}

void motorDireito(int sentido, int potencia){
  analogWrite(POTENCIA_D,potencia);
  if(sentido == 0){ //Freio
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,HIGH);
  }
  else{
     if(sentido == 1){ //Para frente
        digitalWrite(IN1,LOW);
        digitalWrite(IN2,HIGH);
      }
      else{ // Para Trás
        digitalWrite(IN1,HIGH);
        digitalWrite(IN2,LOW);
      }
  }
}