#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define UMIDADE_ANALOGICA A0
#define RELE_PIN 8

#define LIMITE_LIGA 48
#define LIMITE_DESLIGA 52

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variáveis globais para calibração e leitura
int umidadeMinima;
int umidadeMaxima;
int valorUmidade;
bool releLigado = false;
bool calibracaoCompleta = false;

void setup() {
  Serial.begin(9600);  // Inicializa a comunicação serial
  pinMode(UMIDADE_ANALOGICA, INPUT);
  pinMode(RELE_PIN, OUTPUT);
  lcd.init();
  lcd.backlight();

  calibrarSensor();
}

void loop() {
  if (calibracaoCompleta) {
    lerUmidade();
    exibirUmidadeLCD();
    controlarRele();
    delay(1000);  // Delay para evitar leituras muito rápidas
  }
}

void calibrarSensor() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   Calibrando  ");

  lcd.setCursor(0, 1);
  contadorRegressivo(7, "Solo seco    ");  // Contador de 7 segundos
  umidadeMinima = analogRead(UMIDADE_ANALOGICA);

  lcd.setCursor(0, 1);
  contadorRegressivo(7, "Solo umido   ");  // Contador de 7 segundos
  umidadeMaxima = analogRead(UMIDADE_ANALOGICA);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Calibracao OK!");
  delay(2000);

  Serial.print("Umidade Mínima: ");
  Serial.println(umidadeMinima);
  Serial.print("Umidade Máxima: ");
  Serial.println(umidadeMaxima);

  calibracaoCompleta = true;
}
void contadorRegressivo(int segundos, const char* mensagem) {
  for (int i = segundos; i >= 0; i--) {
    lcd.setCursor(0, 1);
    lcd.print(mensagem);  // Exibe a mensagem fixa
    lcd.print(i);
    lcd.print("s");
    delay(1000);  // Espera 1 segundo
  }
}
void lerUmidade() {
  int leitura = analogRead(UMIDADE_ANALOGICA);
  valorUmidade = map(leitura, umidadeMinima, umidadeMaxima, 0, 100);
  valorUmidade = constrain(valorUmidade, 0, 100);
}

void exibirUmidadeLCD() {
  lcd.setCursor(0, 0);
  lcd.print("  Umidade: ");
  lcd.print(valorUmidade);
  lcd.print("%   ");
}

void controlarRele() {
  if (valorUmidade < LIMITE_LIGA && !releLigado) {
    ativarRele();
    releLigado = true;
  } else if (valorUmidade > LIMITE_DESLIGA && releLigado) {
    desativarRele();
    releLigado = false;
  }

  lcd.setCursor(0, 1);
  lcd.print(releLigado ? "     Regando   " : "    Desligado  ");
}

void ativarRele() {
  digitalWrite(RELE_PIN, HIGH);
}

void desativarRele() {
  digitalWrite(RELE_PIN, LOW);
}
