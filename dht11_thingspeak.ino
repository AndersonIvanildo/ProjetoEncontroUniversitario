/* Inclusão das bibliotecas necessárias para a aplicação. */
#include <DHT.h> // Biblioteca responsável pela interpretação do Sensor DHT11.
#include <WiFi.h> // Responsável pela conexão com WiFi.
#include <ThingSpeak.h> // Conexão com ThingSpeak para envio dos dados.

// Definições de Variáveis Importantes ao Longo do Programa
#define DHT_SENSOR_PIN  21 // ESP32 Pino GIOP21 do ESP32 conectado ao sensor DHT11.
#define DHT_SENSOR_TYPE DHT11 // Tipo de sensor.

/* Configurações inicias para conexão com o WiFi e com a plataforma do ThingSpeak */
const char* ssid = "brisa-2645429";
const char* password = "besedb6o";

/* Configurando sensor DHT11 */
DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

/* Configurando variáveis associadas ao ThingSpeak. */
#define THINGSPEAK_WRITE_INTERVAL 30000 // Configurando intervalo adequado para escrita no ThingSpeak
const char * apiKeyEscrita = "0TIRZC2V7KC78RVI"; // Chave de API responsável por escrever no Canal do ThingSpeak
unsigned long numeroDoCanal = 1906050;
WiFiClient client;
unsigned long ultimoTempoConexao = 0;

bool enviaDadosThingSpeak(float temperatura, float umidade)
{
  int okTemperatura = ThingSpeak.writeField(numeroDoCanal, 1, temperatura, apiKeyEscrita);
  int okUmidade = ThingSpeak.writeField(numeroDoCanal, 2, umidade, apiKeyEscrita);
  Serial.println(okTemperatura);
  Serial.println(okUmidade);
  if (okTemperatura == 200 && okUmidade == 200)
  {
    return true;
  }
}

void setup()
{
  Serial.begin(115200);
  dht_sensor.begin();
  WiFi.begin(ssid, password);
  ThingSpeak.begin(client);
  
  while(WiFi.status() != WL_CONNECTED) // Status de conexão ao WiFi.
  {
    delay(500);
    Serial.printf("Conectando ao Wifi %s...\n", ssid);  
  }
  Serial.printf("Conectado ao WiFi %s.\n", ssid);
}

void loop()
{ 
  float temperatura = dht_sensor.readTemperature(); // Leitura da temperatura em Celsius
  float umidade = dht_sensor.readHumidity(); // Leitura da umidade.
  if(millis() - ultimoTempoConexao > THINGSPEAK_WRITE_INTERVAL)
  {
    enviaDadosThingSpeak(temperatura, umidade);
  }
}
