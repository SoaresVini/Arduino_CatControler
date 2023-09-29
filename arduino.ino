#include <Servo.h>
#include <LiquidCrystal.h>

Servo servoTampa;
Servo servoComida;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int pinoDistancia = 13;
int pinoVermelho = 8;
int pinoAzul = 7;
int pinoVerde = 6;
const int anguloTampaFechada = 90;
const int anguloTampaAberta = 0;
unsigned long ultimoTempoAbertura = 0;
unsigned long minutos = 0;
unsigned long tempoDecorrido = 0;
unsigned long segundos = 0;

void setup() {
  // Configurações iniciais dos LEDs
  pinMode(pinoVermelho, OUTPUT);
  pinMode(pinoAzul, OUTPUT);
  pinMode(pinoVerde, OUTPUT);
  digitalWrite(pinoAzul, LOW);
  digitalWrite(pinoVerde, LOW);
  digitalWrite(pinoVermelho, LOW);

  lcd.begin(16, 2); // Inicializa o display LCD com 16 colunas e 2 linhas
  lcd.clear();      // Limpa o display LCD
  lcd.setCursor(0, 0);
  lcd.print("COMEDOURO:");
  lcd.setCursor(0, 2);
  lcd.print("CatController");
  delay(5000); // Aguarda por 5 segundos
  // Anexa os servos da tampa e comida
  servoTampa.attach(9);
  servoComida.attach(10);
  // Define o ângulo da tampa e comida como fechado
  servoTampa.write(anguloTampaFechada);
  servoComida.write(anguloTampaFechada);
  Serial.begin(9600); // Inicializa a comunicação serial com uma taxa de 9600 bps
  lcd.clear();
}

void loop() {
  // Lê a distância a partir do sensor
  int distancia = lerDistanciaUltrassonico(pinoDistancia);

  if (distancia > 100) {
    // Desligar os LEDs e ligar os necessarios
    digitalWrite(pinoAzul, LOW);
    digitalWrite(pinoVerde, LOW);
    digitalWrite(pinoVermelho, LOW);

    digitalWrite(pinoVermelho, HIGH);
    digitalWrite(pinoAzul, HIGH);
    servoTampa.write(anguloTampaAberta);
    servoComida.write(anguloTampaAberta);
    lcd.clear();
    lcd.print("Tampa aberta");
    tempoDecorrido = 0; // Zera o tempo decorrido
    ultimoTempoAbertura = millis(); // Armazena o tempo atual
    delay(3000); // Aguarda por 3 segundos
  } else {
    digitalWrite(pinoAzul, LOW); // Desliga o LED azul
    digitalWrite(pinoVermelho, LOW); // Desliga o LED vermelho
    servoTampa.write(anguloTampaFechada); // Fecha a tampa
    servoComida.write(anguloTampaFechada); // Fecha a comida

    tempoDecorrido = millis() - ultimoTempoAbertura; // Calcula o tempo decorrido
    minutos = (tempoDecorrido / 60000) % 60; // Calcula os minutos
    segundos = (tempoDecorrido / 1000) % 60; // Calcula os segundos

    lcd.setCursor(0, 0);
    lcd.print("Tampa fechada: ");
    lcd.setCursor(0, 1);
    lcd.print(minutos);
    lcd.print(" min ");
    lcd.print(segundos);
    lcd.print(" s");

    // Define a cor dos LEDs conforme o tempo passar
    if (minutos < 1) {
      digitalWrite(pinoVerde, HIGH);
    } else if (minutos >= 1 && minutos <= 2) {
      digitalWrite(pinoVerde, LOW);
      digitalWrite(pinoAzul, HIGH);
    } else {
      digitalWrite(pinoAzul, LOW);
      digitalWrite(pinoVermelho, HIGH);
    }
  }
}

// Calcular o CM a partir do tempo
int lerDistanciaUltrassonico(int pinoTrigger) {
  pinMode(pinoTrigger, OUTPUT); // Configura o pino de trigger como saída
  digitalWrite(pinoTrigger, LOW);
  delayMicroseconds(2); // Aguarda 2 microssegundos
  digitalWrite(pinoTrigger, HIGH);
  delayMicroseconds(10); // Aguarda 10 microssegundos
  digitalWrite(pinoTrigger, LOW);
  pinMode(pinoDistancia, INPUT);


  long duracao = pulseIn(pinoDistancia, HIGH);

  // Converter o tempo em centímetros
  int distancia = duracao / 58; // Aproximadamente 58 μs por centímetro

  return distancia;
}
