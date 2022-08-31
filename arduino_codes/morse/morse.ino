void setup() {
  Serial.begin(115200);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  Serial.println("Started");
}

void liga(){
  //digitalWrite(2,1);
  digitalWrite(13, 1);
} void desliga(){
  //digitalWrite(2,0);
  digitalWrite(13, 0);
}

unsigned long m[5] = 
{0b000000101000111010111000010100,
0b110111010000110000101100110010,
0b101110011000011100110100101010,
0b001000100000100100111101110110,
0b111011111001100110110001100010};

void morse(String str, int cont){
  if (str.length() == cont) return; 
  byte c = str.charAt(cont);
  if (c == 10) return;
  if (c == 32){
    delay(700);
    morse(str, cont+1);
  } c -= 97;
  byte pos = m[c/5]>>(6*(c-5*(c/5)));
  byte sz = ~(pos>>4)&3;
  for (int i = 3; i >= sz; i--){
    liga();
    if (bitRead(pos,i)){
      delay(300);
    } else {
      delay(100);
    } desliga();
    delay(100);
  } delay(300);
  morse(str, cont+1);
}

void loop() {
  if (Serial.available() > 0){
    Serial.println("Writing");
    //digitalWrite(4, 1);
    morse(Serial.readString(), 0);
    //digitalWrite(4, 0);
    Serial.println("Finished");
  }
}














/*
byte pos = command/5;
byte shift = 6*(command-5*pos);
int sz = ~(m[pos]>>(shift+4))&3;
for (int i = 3; i >= sz; i--){
  if (bitRead(m[pos]>>shift,i)){Serial.print('-');}
  else {Serial.print('.');}
}
*/
//there's a faster way... the bytes tell the letter you need to jump to... (5 to tell where to jump - 1 to contain the bit... so maybe not that fast)
//010100111000111010101000000000110010101100110000010000110111101010110100011100011000101110110110111101100100100000001000100010110001100110111001111011111100
//unsigned long m[5] = {0b010100111000111010101000000000,0b110010101100110000010000110111,0b101010110100011100011000101110,0b110110111101100100100000001000,0b100010110001100110111001111011}; //,0b111100
//Vai pro vetor certo
//Pega a posicao certa
//Ve qual o tamanho da palavra por (xx)xxxx
//pra cada bit, pisca e faz o delay     
//for (int i = 5; i > 0; i--){
   //Serial.println(m[i], BIN);
   //Serial.print(bitRead(x, i));
//}
//Serial.println("");
//for (int i = 29; i > 0; i--){
//  Serial.print(bitRead(m[(command-97)/5],i));
//}
//for (int i = 5; i >= 0; i--){
//  Serial.print(bitRead(m[pos]>>shift+4,i));
//}
