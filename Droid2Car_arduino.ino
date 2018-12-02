#include <TM1637Display.h>
#include <SoftwareSerial.h>
SoftwareSerial BT1(4,2); // RX, TX

// Motor A pins (enableA = enable motor, pinA1 = forward, pinA2 = backward)
int enableA = 10;
int pinA1 = 3;
int pinA2 = 12;

//Motor B pins (enabledB = enable motor, pinB2 = forward, pinB2 = backward)
int enableB = 7;
int pinB1 = 5;
int pinB2 = 6;

//Zumbador
int pinZumbador = 11;

//Bluetooth
int txBT = 4;
int rxBT = 2;

boolean comandos[] = {false,false,false,false,false,false};
int tiempoMovimiento = 700;
int tiempoGiro = 575;
int freno = 500;
//LCD03 lcd;
boolean parado = false;

const int CLK = 9;
const int DIO = 8;
TM1637Display display(CLK, DIO);

const uint8_t SEG_HOLA[] = {
  SEG_F | SEG_E | SEG_G | SEG_B | SEG_C,           // h
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // o
  SEG_F | SEG_E | SEG_D,                           // l
  SEG_A | SEG_B | SEG_C | SEG_F | SEG_E | SEG_G    // a
  };
const uint8_t SEG_CASA[] = {
  SEG_A | SEG_F | SEG_E | SEG_D,                   // c
  SEG_A | SEG_B | SEG_C | SEG_F | SEG_E | SEG_G,   // a
  SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,           // s
  SEG_A | SEG_B | SEG_C | SEG_F | SEG_E | SEG_G    // a
  };

const uint8_t SEG_SALA[] = {
  SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,           // s
  SEG_A | SEG_B | SEG_C | SEG_F | SEG_E | SEG_G,   // a
  SEG_F | SEG_E | SEG_D,                           // l
  SEG_A | SEG_B | SEG_C | SEG_F | SEG_E | SEG_G    // a
  };

const uint8_t SEG_BANO[] = {
  SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,           // b
  SEG_A | SEG_B | SEG_C | SEG_F | SEG_E | SEG_G,   // a
  SEG_E | SEG_G | SEG_C | SEG_A,                   // ñ
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F    // o
  };

const uint8_t SEG_PASILLO[] = {
  SEG_A | SEG_B | SEG_G | SEG_F | SEG_E,           // p
  SEG_A | SEG_B | SEG_C | SEG_F | SEG_E | SEG_G,   // a
  SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,           // s
  SEG_F | SEG_E                                    // i
  };

const uint8_t SEG_COCINA[] = {
  SEG_A | SEG_F | SEG_E | SEG_D,                   // c
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // o
  SEG_A | SEG_F | SEG_E | SEG_D,                   // c
  SEG_F | SEG_E                                    // i
  };

const uint8_t SEG_SALA1[] = {
  SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,           // s
  SEG_A | SEG_B | SEG_C | SEG_F | SEG_E | SEG_G,   // a
  0x00,                   
  SEG_B | SEG_C                                    // 1
  };

const uint8_t SEG_SALA2[] = {
  SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,           // s
  SEG_A | SEG_B | SEG_C | SEG_F | SEG_E | SEG_G,   // a
  0x00,                   
  SEG_A | SEG_B | SEG_G | SEG_E | SEG_D            // 2
  };

void setup() {
  pinMode(enableA, OUTPUT);
  pinMode(pinA1, OUTPUT);
  pinMode(pinA2, OUTPUT);

  pinMode(enableB, OUTPUT);
  pinMode(pinB1, OUTPUT);
  pinMode(pinB2, OUTPUT);

  pinMode(pinZumbador , OUTPUT); 

  Serial.begin(9600);
  BT1.begin(9600);
  encenderMotores();
  frenar();
//  lcd.begin(20, 4);                      
//  lcd.print("Droid2Car listo");
  display.setBrightness(0x0a);
  display.setSegments(SEG_HOLA);
}

void loop() {
  if (BT1.available()){
    char C = BT1.read(); 
    if(C == '*'){
      digitalWrite(pinZumbador, HIGH);
      delay(1000);
      digitalWrite(pinZumbador, LOW);
      for(int i = 0; i < 6; i++){
        if(comandos[i]){
          ejecutarComando(i+1);
        }
      }
      for(int i = 0; i < 6; i++){
        comandos[i] = false;
      }
    }
    else{
      int pos = C - '0';
      if(pos >= 0 && pos <=6){
        comandos[pos-1] = true;
      }
    }
  }
}

void ejecutarComando(int comando){
  switch (comando){
    case 1:
//      lcd.clear();                   
//      lcd.print("Limpiando baño...");
      display.setSegments(SEG_BANO);
      encenderMotores();
      //Ida
      if(!parado)
        avanzar();
      if(!parado)
        rotarIzq();
      if(!parado)
        avanzar();
      if(!parado)
      //Recorrer cuarto
      if(!parado)
        rotarIzq();
      if(!parado)
        avanzar();
      //Media vuelta
      if(!parado)
        rotarDer();
      if(!parado)
        rotarDer();
      //Vuelta
      if(!parado)
        avanzar();
      if(!parado)
        rotarDer();
      if(!parado)
        avanzar();
      if(!parado)
        rotarDer();
      if(!parado)
        avanzar();
      //Apuntar adelante
      if(!parado)
        rotarDer();
      if(!parado)
        rotarDer();
//      lcd.clear();                   
//      lcd.print("Esperando orden...");
      display.setSegments(SEG_CASA);
      parado=false;
      break;
    case 6:
//      Serial.println("Limpiando pasillo...");
      display.setSegments(SEG_PASILLO);
      encenderMotores();
      //Ida
      if(!parado)
        avanzar();
      if(!parado)
        avanzar();
      if(!parado)
        rotarDer();
      if(!parado)
        avanzar();
      if(!parado)
      //Media vuelta
      if(!parado)
        rotarDer();
      if(!parado)
        rotarDer();
      //Vuelta
      if(!parado)
        avanzar();
      if(!parado)
        rotarIzq();
      if(!parado)
        avanzar();
      if(!parado)
        avanzar();
      //Apuntar adelante
      if(!parado)
        rotarDer();
      if(!parado)
        rotarDer();
//      lcd.clear();                   
//      lcd.print("Esperando orden...");
      display.setSegments(SEG_CASA);
      parado=false;
      break;
    case 2:
//      Serial.println("Limpiando cocina...");
      display.setSegments(SEG_COCINA);
      encenderMotores();
      //Ida
      if(!parado)
        avanzar();
      if(!parado)
        avanzar();
      if(!parado)
        rotarIzq();
      if(!parado)
        avanzar();
      //Recorrer cuarto
      if(!parado)
        rotarDer();
      if(!parado)
        avanzar();
      if(!parado)
        avanzar();
      //Media vuelta
      if(!parado)
        rotarIzq();
      if(!parado)
        rotarIzq();
      //Vuelta
      if(!parado)
        avanzar();
      if(!parado)
        avanzar();
      if(!parado)
        rotarIzq();
      if(!parado) 
        avanzar();
      if(!parado)
        rotarDer();
      if(!parado)
        avanzar();
      if(!parado)
        avanzar();
      //Apuntar adelante
      if(!parado)
        rotarDer();
      if(!parado)
        rotarDer();
//      lcd.clear();                   
//      lcd.print("Esperando orden...");
      display.setSegments(SEG_CASA);
      parado=false;
      break;
    case 3:
//      lcd.clear();                   
//      lcd.print("Limpiando cuarto 1...");
      display.setSegments(SEG_SALA1);
      encenderMotores();
      //Ida
      if(!parado)
        avanzar();
      if(!parado)
        avanzar();
      if(!parado)
        rotarDer();
      if(!parado)
        avanzar();
      if(!parado)
        rotarIzq();
      if(!parado)
        avanzar();
      //Recorrer cuarto
      if(!parado)
        avanzar();
      if(!parado)
        rotarIzq();
      if(!parado)
        avanzar();
      if(!parado)
        rotarIzq();
      if(!parado)
        avanzar();
      if(!parado)
        rotarIzq();
      if(!parado)
        avanzar();
      if(!parado)
        rotarDer();
      if(!parado)
        avanzar();
      if(!parado)
        rotarDer();
      //Vuelta
      if(!parado)
        avanzar();
      if(!parado)
        rotarIzq();
      if(!parado)
        avanzar();
      if(!parado)
        avanzar();
      //Apuntar adelante
      if(!parado)
        rotarDer();
      if(!parado)
        rotarDer();
//      lcd.clear();                   
//      lcd.print("Esperando orden...");
      display.setSegments(SEG_CASA);
      parado=false;
      break;
    case 4:
//      lcd.clear();                   
//      lcd.print("Limpiando cuarto 2...");
      display.setSegments(SEG_SALA2);
      encenderMotores();
      //Ida
      if(!parado) 
        avanzar();
      if(!parado)
        avanzar();
      if(!parado)
        rotarDer();
      if(!parado)
        avanzar();
      if(!parado)
        avanzar();
      //Recorrer cuarto
      if(!parado)
        avanzar();
      if(!parado)
        rotarIzq();
      if(!parado)
        avanzar();
      if(!parado)
        avanzar();
      if(!parado)
        rotarIzq();
      if(!parado)
        avanzar();
      if(!parado)
        rotarIzq();
      if(!parado)
        avanzar();
      if(!parado)
        avanzar();
      if(!parado)
        rotarDer();
      if(!parado)
        avanzar();
      //Vuelta
      if(!parado)
        avanzar();
      if(!parado)
        rotarIzq();
      if(!parado)
        avanzar();
      if(!parado)
        avanzar();
      //Apuntar adelante
      if(!parado)
        rotarDer();
      if(!parado)
        rotarDer();
//      lcd.clear();                   
//      lcd.print("Esperando orden...");
      display.setSegments(SEG_CASA);
      parado=false;
      break;
    case 5:
//      lcd.clear();                   
//      lcd.print("Limpiando salon...");
      display.setSegments(SEG_SALA);
      encenderMotores();
      //Ida
      if(!parado)
        avanzar();
      if(!parado)
        rotarDer();
      if(!parado)
        avanzar();
      if(!parado)
        //Recorrer cuarto
        avanzar();
      if(!parado)
        avanzar();
      if(!parado)
        rotarDer();
      if(!parado)
        avanzar();
      if(!parado)
        rotarDer();
      if(!parado)
        avanzar();
      if(!parado)
        avanzar();
      if(!parado)
        rotarDer();
      if(!parado)
        avanzar();
      if(!parado)
        avanzar();
      //Vuelta
      if(!parado)
        rotarIzq();
      if(!parado)
        avanzar();
      if(!parado)
        rotarIzq();
      if(!parado)
        avanzar();
      if(!parado)
        avanzar();
      //Apuntar adelante
      if(!parado)
        rotarDer();
      if(!parado)
        rotarDer();
//      lcd.clear();                   
//      lcd.print("Esperando orden...");
      display.setSegments(SEG_CASA);
      parado=false;
      break;
   case 9:
        rotarDer();
        rotarIzq();
   default:
//      lcd.clear();                   
//      lcd.print("Parando motores...");
      frenar();
//      lcd.clear();                   
//      lcd.print("Esperando orden...");
      break;
    }
  }

void encenderMotores()
{
  motorAOn();
  motorBOn();
}

void apagarMotores()
{
  motorAOff();
  motorBOff();
}

void comprobarParar(){
  if (BT1.available()){
    char C = BT1.read(); 
    if(C == '0'){
      digitalWrite(pinZumbador, HIGH);
      delay(1000);
      digitalWrite(pinZumbador, LOW);
      motorAFrenar();
      motorBFrenar();
      for(int i = 0; i < 6; i++){
        comandos[i] = false;
      }
      display.setSegments(SEG_CASA);
      parado = true;
    }
  }
}

void avanzar()
{
    comprobarParar(); 
    motorAAvanzar();
    motorBAvanzar();
    delay(tiempoMovimiento);
    frenar();
}

void atras()
{
    comprobarParar(); 
    motorAAtras();
    motorBAtras();
    delay(tiempoMovimiento);
    frenar();
}

void rotarIzq()
{
    comprobarParar(); 
    motorAAvanzar();
    //motorBAtras();
    delay(tiempoGiro);
    frenar();
}

void rotarDer()
{
    comprobarParar(); 
    //motorAAtras();
    motorBAvanzar();
    delay(tiempoGiro);
    frenar();
}

void frenar()
{
    comprobarParar(); 
    motorAFrenar();
    motorBFrenar();
    delay(freno);
}

//enable motors
void motorAOn()
{
  digitalWrite(enableA, HIGH);
}

void motorBOn()
{
  digitalWrite(enableB, HIGH);
}

//disable motors
void motorAOff()
{
  digitalWrite(enableB, LOW);
}

void motorBOff()
{
  digitalWrite(enableA, LOW);
}

//motor A controls
void motorAAvanzar()
{
  digitalWrite(pinA1, HIGH);
  digitalWrite(pinA2, LOW);
}

void motorAAtras()
{
  digitalWrite(pinA1, LOW);
  digitalWrite(pinA2, HIGH);
}

//motor B controls
void motorBAvanzar()
{
  digitalWrite(pinB1, HIGH);
  digitalWrite(pinB2, LOW);
}

void motorBAtras()
{
  digitalWrite(pinB1, LOW);
  digitalWrite(pinB2, HIGH);
}

void motorAFrenar()
{
  digitalWrite(pinA1, HIGH);
  digitalWrite(pinA2, HIGH);
}

void motorBFrenar()
{
  digitalWrite(pinB1, HIGH);
  digitalWrite(pinB2, HIGH);
}

