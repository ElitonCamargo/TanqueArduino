// Captura de dados bluetooth
#include <SoftwareSerial.h>
SoftwareSerial BT(12, 13); // RX, TX
int Eixo_X=0;
int Eixo_Y=0;


/*Pinagem dos motores*/
//motor_ESQUERDA
#define IN1 4
#define IN2 5
#define POTENCIA_E 10

//motor_DIREITA
#define IN3 6
#define IN4 7
#define POTENCIA_D 9

void setup() {
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(POTENCIA_E,OUTPUT);
  pinMode(POTENCIA_D,OUTPUT);
  Serial.begin(9600);
  BT.begin(9600); // Default communication rate of the Bluetooth module
  delay(500);
}

void loop() { 
  // for(int x = 255; x > 0; x--){
  //   motorEsquerdo(2,x);
  //   Serial.println(x);
  //   delay(100);
  // }
  // motorEsquerdo(2,120);
  // Read the incoming data from the Smartphone Android App
  while (BT.available() >= 2) {
    Eixo_X = BT.read();
    delay(10);
    Eixo_Y = BT.read();

    if(Eixo_X < 201 && Eixo_Y < 201){      
        Eixo_X -= 100;        
        Eixo_Y = 100 - Eixo_Y;        
    }
    printDados(Eixo_X, Eixo_Y);
    acao(Eixo_X, Eixo_Y);
  }
  acao(Eixo_X, Eixo_Y);
  delay(10);
  
}

void acao(int x, int y){
  if(x > -101 && x < 101){
    direcao(x, y);
  }
}

void direcao(int x, int y){    
    if(x != 0 | y != 0){// Tanque em movimento        
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
            motorEsquerdo(1,potencia+x*1);
        }
        else if(y > 10 && x > 10){ //Para frente -> Virando para direita
            int potencia = map(y,0,100,0,255);
            motorDireito(1,potencia-x*1);
            motorEsquerdo(1,potencia);
        }
        else if(y < -10 && x < -10){ //Para trás -> Virando para esquerda
            int potencia = map(y,0,100,0,255);
            motorDireito(2,potencia);
            motorEsquerdo(2,potencia+x*1);
        }
        else if(y < -10 && x > 10){ //Para trás -> Virando para direita
            int potencia = map(y,0,100,0,255);
            motorDireito(2,potencia-x*1);
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
  Serial.print(Eixo_X); 
  Serial.print(", Y:"); 
  Serial.println(Eixo_Y);
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