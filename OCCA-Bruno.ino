/*
  Projeto:Automação de Iluminação, Projeto SPMAKERWEEK29
  Autores: Técnicos Cícero Henrique, Bruno Silveira, Thayná Miguel, Adriano Santos
  Data: 12/08/2019
  Descrição: O desenvolvimento abaixo é parte de um projeto colaborativo entre rede pública de laboratórios Fab Lab LIVRE SP,
  para o evento SPMAKERWEEK2019 com tema Cidades Inteligentes.

  Modified by Bruno Silveira
  27.09.19
*/

//Carrega as bibliotecas
#include "EmonLib.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define pot A0

#define ledMin 3  //pino do primeiro led
#define ledMax 12 //pino do ultimo led

int potValue;
unsigned long agora = 0;

unsigned long currentMillis;
unsigned long previousMillis = 0;
const long interval = 100;

int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers


const byte intPin = 2;
unsigned int efeito = 1;

void tempo(int x) {
  agora = millis();
  while (millis() < agora + x) {
    //wait
  }
}

void verPot() {
  currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    potValue = map(analogRead(pot), 0, 1023, 0, 100);
    //Serial.println(potValue);
  }
}

void pisca(int x) {
  verPot();
  digitalWrite(x, HIGH);  //acende led da vez
  tempo(potValue);

  digitalWrite(x, LOW);   //apaga led da vez
  tempo(potValue);
}

void vaiEvolta() {
  int led = ledMin;

  while (led < ledMax) {
    pisca(led);
    led++;                //proximo led
  }

  while (led > ledMin) {
    pisca(led);
    led--;                //led anterior
  }
}

void intensidade() {
  int led = ledMin;

  while (led < ledMax + 1) {
    verPot();
    int intensidade = map(potValue, 0, 100, 0, 255);
    analogWrite(led, intensidade);  //acende led da vez
    led++;                //proximo led
  }
}

void piscaRandom() {
  int led = random(ledMin, ledMax + 1);
  pisca(led);
}

void parImpar() {
  for (int led = ledMin; led < ledMax + 1; led++) {
    if (led % 2 == 0) {
      digitalWrite(led, HIGH);  //acende led da vez
    }
  }

  verPot();
  tempo(potValue);

  for (int led = ledMin; led < ledMax + 1; led++) {
    if (led % 2 == 0) {
      digitalWrite(led, LOW);  //apaga led da vez
    }
  }
  tempo(potValue);

  for (int led = ledMin; led < ledMax + 1; led++) {
    if (led % 2 != 0) {
      digitalWrite(led, HIGH);  //acende led da vez
    }
  }

  verPot();
  tempo(potValue);

  for (int led = ledMin; led < ledMax + 1; led++) {
    if (led % 2 != 0) {
      digitalWrite(led, LOW);  //apaga led da vez
    }
  }
  tempo(potValue);
}

void mode() {
  unsigned long debounceTime = millis();

  if (debounceTime - lastDebounceTime > 200) {
    Serial.print("pressed \n");
    if (efeito < 4) {
      efeito++;
    }
    else efeito = 1;

  }
  lastDebounceTime = debounceTime;
  //Serial.println(efeito);
}


void setup() {
  Serial.begin(9600);

  for (int led = ledMin; led <= ledMax; led++) {
    pinMode(led, OUTPUT);
  }
  pinMode(intPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(intPin), mode, FALLING);
}


void loop() {

  switch (efeito) {
    case 1:
      intensidade();
      break;
    case 2:
      vaiEvolta();
      break;
    case 3:
      piscaRandom();
      break;
    case 4:
      parImpar();
      break;
  }

}
