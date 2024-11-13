#define BLYNK_TEMPLATE_ID "TMPL2YoyWbhS7"                        // Variável que recebe ID Blynk
#define BLYNK_TEMPLATE_NAME "HomeOfficeConnection"               // Variável que nome de template Blynk
#define BLYNK_AUTH_TOKEN "AxxRaam8Oza_o-An_gHSWSji9eydGm88"      // Variável que token Blynk

#include <WiFi.h>                       // Biblioteca Wi-Fi
#include <BlynkSimpleEsp32.h>           // Biblioteca Blynk
#include <ESP32Servo.h>                 // Biblioteca servos
#include <DHT.h>                        // Biblioteca DHT22 (sensor de temperatura e umidade)
#include <RTClib.h>                     // Biblioteca RTC DS3231 (real time clock)

// Define as credenciais do WiFi
char auth[] = "AxxRaam8Oza_o-An_gHSWSji9eydGm88";  // Token recebido pelo e-mail (Blynk)
char ssid[] = "XXXXXX";                        // Nome da rede Wi-Fi
char pass[] = "xxxxxx";                  // Senha da rede Wi-Fi

#define DHTPIN 4                         // Define o pino do DHT22 (exemplo: GPIO 4)
#define DHTTYPE DHT22                    // Tipo do sensor (DHT22)
DHT dht(DHTPIN, DHTTYPE);                // Variável que recebe função da biblioteca DHT

Servo servo1;                            // Variável que recebe função da biblioteca servo
Servo servo2;                            // Variável que recebe função da biblioteca servo

RTC_DS3231 rtc;                          // Variável que recebe função da biblioteca RTC
// SDA em GPIO 21, SCL em GPIO 22

int ledPin = 2;                          // Define o pino do LED (GPIO 2)

// Função que atribui controle do LED no pino virtual V0 do Blynk
BLYNK_WRITE(V0) {
  int ledState = param.asInt();
  analogWrite(ledPin, ledState);
}

// Função que atribui controle do servo destinado a encosto no pino virtual V1 do Blynk
BLYNK_WRITE(V1) {
  int position = param.asInt();
  servo1.write(position);
}

// Função que atribui controle do servo destinado a persiana no pino virtual V2 do Blynk
BLYNK_WRITE(V2) {
  int position = param.asInt();
  servo2.write(position);
}

// Função de inicialização e configuração
void setup() {
  Serial.begin(115200);              // Define velocidade e inicialização do monitor serial
  Blynk.begin(auth, ssid, pass);     // Inicializa integração Blynk via internet com token, nome da rede Wi-Fi e senha
  
  servo1.attach(15);                 // Conecta o Servo 1 ao GPIO 15
  servo2.attach(16);                 // Conecta o Servo 2 ao GPIO 16
  
  pinMode(ledPin, OUTPUT);           // Configura o pino do LED como saída

  dht.begin();                       // Inicializa DHT22 (sensor de temperatura e umidade)

// Inicialização do DS3231 (real time clock)
  if (!rtc.begin()) {                                 // Se não for possível iniciá-lo, retorna erro
    Serial.println("Erro ao inicializar DS3231!");
    while (1);
  }
  if (rtc.lostPower()) {                              // Se for possível iniciá-lo, retorna data e hora
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

// Função para enviar dados de temperatura e umidade ao Blynk
void sendSensorData() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

// Verifica se a leitura foi bem-sucedida
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Falha na leitura do sensor DHT!");
    return;
  }

// Envia para o Blynk (pino V3 para temperatura, pino V4 para umidade, ambos GPIO 4)
  Blynk.virtualWrite(V3, temperature);
  Blynk.virtualWrite(V4, humidity);
}

// Função de looping para rodar Blynk e sensor de temperatura e umidade com delay de 2000 segundos de leitura
void loop() {
  Blynk.run();
  sendSensorData();
  delay(2000);                    // Intervalo para leitura do sensor temperatura e umidade
}
