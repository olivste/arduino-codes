#include <Wire.h> // Biblioteca para comunicação I2C
#include <LiquidCrystal_I2C.h> // Biblioteca para LCD I2C

#define umidadeAnalogica A0 // Pino A0 para leitura analógica do sensor
#define relePin 8 // Pino para controle do relé

LiquidCrystal_I2C lcd(0x27, 16, 2); // Endereço I2C do LCD e suas dimensões

int valorUmidade; // Variável para armazenar o valor da umidade
int valorUmidadeDigital; // Variável para armazenar a saída digital do sensor de umidade do solo

void setup() {
  Serial.begin(9600); // Inicia a comunicação serial
  pinMode(umidadeAnalogica, INPUT); // Define umidadeAnalogica como entrada
  pinMode(relePin, OUTPUT); // Define relePin como saída
  lcd.init(); // Inicializa o LCD
  lcd.backlight(); // Ativa a luz de fundo do LCD
}

void loop() {
  valorUmidade = analogRead(umidadeAnalogica); // Realiza a leitura analógica do sensor e armazena em valorUmidade
  Serial.print(valorUmidade);
  Serial.print(' ');
  valorUmidade = map(valorUmidade, 1023, 440, 0, 100); // Transforma os valores analógicos em uma escala de 0 a 100
  
  if(valorUmidade > 100) {
    valorUmidade = 100;
  };

  // Exibe a umidade no LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Umidade:");
  lcd.print(valorUmidade);
  lcd.print("%  ");
  if(valorUmidade > 9) {
    lcd.print("  Umidade: ");
    lcd.print(valorUmidade);
    lcd.print("% ");
  }

  // Ativa o relé se a umidade estiver abaixo de 50%
  if (valorUmidade < 50) {
    digitalWrite(relePin, HIGH); // Ativa o relé
    lcd.setCursor(0, 1);
    lcd.print("  Esta regando  ");
    delay(1000);
  } else {
    digitalWrite(relePin, LOW); // Desativa o relé
    lcd.setCursor(0, 1);
    lcd.print("   Desativado   ");
  }

  // Exibe a umidade no monitor serial
  Serial.print("Umidade encontrada: ");
  Serial.print(valorUmidade);
  Serial.println(" %");
  
  delay(1000); // Atraso de 1000ms (1 segundos)
}
