/* Universidade Federal do Paraná (UFPR) - DELT
 * Disciplina: Sistemas Operacionais Embarcados
 * Prática 4: Integração IoT com Plataforma Blynk e FreeRTOS
 * Professor: Thiago José da Luz, PhD 
 * Data: 28/03/2026
 * * OBJETIVO DIDÁTICO: 
 * Implementar telemetria e controle bidirecional via nuvem (IoT), utilizando 
 * Mutexes para sincronizar o acesso ao barramento do LCD enquanto os dados 
 * são enviados para a plataforma Blynk através de Tasks paralelas em núcleos distintos.
 */

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <LiquidCrystal.h>
#include <DHTesp.h>

#include <WiFi.h> //para usar o Wifi

//Parâmetros do BlYnk (blynk.cloud/)
#define BLYNK_TEMPLATE_ID "TMPL21s8Ruer0"
#define BLYNK_TEMPLATE_NAME "Prática 4 WOKWI"
#define BLYNK_AUTH_TOKEN "hAmPel-TtNa6FsTA2W9YoYskjM2vyUnj"
#include <BlynkSimpleEsp32.h> // biblioteca do Blynk
BlynkTimer timer; // timer para execução periódica do 

// Parâmetros da Rede
char auth[] = BLYNK_AUTH_TOKEN; //Token de autorização da plataforma Blink
char ssid[] = "Wokwi-GUEST"; // nome da rede wifi virtual do WokWi
char pass[] = ""; // senha

#// Definições de Hardware
#define slider 34     // Entrada Analógica (Potenciômetro)
#define temp 15       // Pino de dados DHT22
#define PinLed 32     // LED de status
#define DHT_PIN temp

// Barramento Paralelo do LCD (Modo 4 bits)
#define RS 19
#define E 18
#define D4 5
#define D5 17
#define D6 16
#define D7 4
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

DHTesp dhtSensor;

// Protótipos das Tasks
void TaskSlider(void *pvParameters); 
void TaskTemp(void *pvParameters);

SemaphoreHandle_t Semaforo; // Handler para o Mutex

void setup() {

  Serial.begin(115200);
   // Task 1: Pinned ao Core 0 - Foco em leitura de entrada analógica
  xTaskCreatePinnedToCore(TaskSlider, "Slider", 10000, NULL, 1, NULL,0);

  // Task 2: Pinned ao Core 1 - Foco em leitura de sensor digital (DHT22)
  xTaskCreatePinnedToCore(TaskTemp, "Temperatura", 10000, NULL, 1, NULL,1);

  // Inicialização do Mutex para proteção do LCD
  if ( Semaforo == NULL )  // Verifique se o Semáforo Serial ainda não foi criado
  {
    Semaforo = xSemaphoreCreateMutex();  // Crie um semáforo mutex que 	
    				// usaremos para gerenciar o LCD e o Blynk
    if ( ( Semaforo ) != NULL )
      xSemaphoreGive( ( Semaforo ) );  // Inicializa o recurso como disponível
  }
  // Inicialização de Periféricos
  lcd.begin(16, 2);
  lcd.clear();
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  pinMode(PinLed, OUTPUT);

  // Conexão com a infraestrutura Blynk
  Blynk.begin(auth, ssid, pass); // inicia o Blink

}

void loop() {
    /* O loop principal gerencia a pilha de protocolos e manutenção da conexão */
    Blynk.run(); // função necessária do Blynk
    timer.run(); // reativa o timer caso já tenha disparado
}

/* TASK SLIDER (Core 0): Gerencia a leitura do potenciômetro e escreve na Linha 0 */
void TaskSlider(void* pvParameters) {
  (void) pvParameters;
  vTaskDelay(500/portTICK_PERIOD_MS); // Offset inicial para setup

  // Seção Crítica: Proteção da inicialização do cabeçalho
  if ( xSemaphoreTake( Semaforo, ( TickType_t ) 5 ) == pdTRUE )  {
    lcd.setCursor(0, 0);
    lcd.print("Potenc.:");
    xSemaphoreGive( Semaforo ); // Agora libere a Porta Serial para outros
  }      
  

  for(;;){
    
      // leia a entrada analógica no pino A0
      int sensor = analogRead(slider);
      // Converte o valor para percentual
      sensor = map(sensor, 0,4096,0,100);

      // Tenta obter acesso exclusivo ao LCD por até 5 ticks
      if ( xSemaphoreTake( Semaforo, ( TickType_t ) 5 ) == pdTRUE )  {
        // Conseguimos obter o semáforo e agora podemos acessar o recurso compartilhado.
        // Queremos ter a Porta Serial só para nós, pois demora para imprimir
        lcd.setCursor(8, 0);
        lcd.print(sensor);
        lcd.print("% ");

        // Envio para o Virtual Pin V1 do Blynk
        Blynk.virtualWrite(V1, sensor); // (pino virtual do blink, valor para enviar)
        xSemaphoreGive( Semaforo ); // Agora libere a Porta Serial para outros
      }      
      
      vTaskDelay(500/portTICK_PERIOD_MS); // Bloqueio da Task por 500ms
  }
}

/* TASK TEMP (Core 1): Gerencia o sensor DHT22 e escreve na Linha 1 */
void TaskTemp(void* pvParameters) {
  (void) pvParameters;
  vTaskDelay(500/portTICK_PERIOD_MS); // Offset inicial para setup

  // Seção Crítica: Proteção da inicialização do cabeçalho
  if ( xSemaphoreTake( Semaforo, ( TickType_t ) 5 ) == pdTRUE )  {
      lcd.setCursor(0, 1);
      lcd.print("Temp.:");
      xSemaphoreGive( Semaforo ); // Agora libere a Porta Serial para outros
   }      
  

  int sensor;

  for(;;){
    // leia a entrada analógica no pino A0
    // escreva na tela o valor lido
    TempAndHumidity  data = dhtSensor.getTempAndHumidity();

    // Sincronização entre núcleos para acesso ao periférico paralelo
    if ( xSemaphoreTake( Semaforo, ( TickType_t ) 5 ) == pdTRUE )  {
        // Conseguimos obter o semáforo e agora podemos acessar o recurso compartilhado..
        // Queremos ter a Porta Serial só para nós, pois demora para imprimir
        lcd.setCursor(8, 1);
        lcd.print(String(data.temperature, 2));
        lcd.write(0xDF); // Símbolo de grau
        lcd.print("C");

        // Envio para o Virtual Pin V2 do Blynk
        Blynk.virtualWrite(V2, data.temperature); 
        xSemaphoreGive( Semaforo ); // Agora libere a Porta Serial para outros        
    }      
    
    vTaskDelay(500/portTICK_PERIOD_MS); // Bloqueio da Task por 500ms
  }
}


/* Callback de Atuação Remota: Controle via Cloud (V0) */
BLYNK_WRITE(V0) { // função para receber dados e escrever no pino (V0, pino virtual do blink)
  int buttonState = param.asInt(); // estado do botão
  if (buttonState == 0) {
    digitalWrite(PinLed, LOW);
    Serial.println("Comando Remoto: Led OFF");
    Blynk.logEvent ("led_event","Led desligado"); //envia evento
      } else {
    digitalWrite(PinLed, HIGH);
    Serial.println("Comando Remoto: Led ON");
    Blynk.logEvent ("led_event","Led ligado"); //envia evento
  }
}