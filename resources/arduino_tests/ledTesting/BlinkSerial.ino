/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/

void setup() {
  Serial.begin(115200);
  pinMode(A0, OUTPUT);
  pinMode(4, OUTPUT);
}

void liga(){
  analogWrite(A0,1023);
  digitalWrite(4,1023);
  return;
}

void desliga(){
  analogWrite(A0, 0);
  return;
}

int brilhar(int brilhando){
  if (brilhando > 0){
    brilhando += 50;
    if (brilhando >= 1023){
      brilhando = 1023;
    }
    analogWrite(A0, brilhando);
    delay(400);
  }
  return brilhando;
}

int piscar(int piscando){
  if (piscando > 0){
    if (piscando %2 == 0){
      liga();
    } 
    else {
      desliga();
    }
    piscando -= 1;
    delay(500);
  }
  return piscando; 
}

int mor = 0;
bool rgb = false;
int translator[26][4] = {{1,0,2,2},{0,1,1,1},{0,1,0,1},{0,1,1,2},{1,2,2,2},{1,1,0,1},{0,0,1,2},{1,1,1,1},{1,1,2,2},{1,0,0,0},{0,1,0,2},{1,0,1,1},{0,0,2,2},{0,1,2,2},{0,0,0,2},{1,0,0,1},{0,0,1,0},{1,0,1,2},{1,1,1,2},{0,2,2,2},{1,1,0,2},{1,1,1,0},{1,0,0,2},{0,1,1,0}, {0,1,0,0},{0,0,1,1}};
void morse(int m){
  if(m!=10){morseCode(translator[m-97]);
  }
  else{
    mor-=1; 
    if (mor == 0){
      analogWrite(A1, 0);
      Serial.println("Fim do Morse");
      rgb = false;
    }
  }
}
void morseCode(int t[]){
  for (int i = 0; i < 4; i++){
     if (t[i] == 1){
       liga();
       delay(100);
     }
     else if (t[i] == 0){
       liga();
       delay(500);
     }
     desliga();
     delay(200);
  }
  delay(400);
}

int brilhando = 0;
int piscando = 0;
void loop() {
  if (Serial.available() > 0){
    int command = Serial.read();
    if (mor > 0){
      morse(command);
    }
    else{
      switch (command){
        case 108:
          liga();
          Serial.println("Ligado");
          break;
        case 100:
          desliga();
          Serial.println("Desligado");
          brilhando = 0;
          piscando = 0;
          break;
        case 109:
          Serial.println("Morse");
          analogWrite(A1, 1023);
          rgb = true;
          mor = 2; 
          break;
        case 112:
          Serial.println("Piscando");
          piscando = 10; 
          break;
        case 98:
          Serial.println("Brilho");
          brilhando += 50;
          break;
        case 114:
          Serial.println("RGB!");
          rgb = not rgb;
          analogWrite(A1, 1023*rgb);
          break;
        case 10:
          break;
        default:
          Serial.println("Comando Invalido");
          break;
      }
    }
  } 
  brilhando = brilhar(brilhando);
  piscando = piscar(piscando);
}
