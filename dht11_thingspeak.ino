/* Inclusão das bibliotecas necessárias para a aplicação. */
#include <DHT.h> // Biblioteca responsável pela interpretação do Sensor DHT11.
#include <WiFi.h> // Responsável pela conexão com WiFi.
#include <ThingSpeak.h> // Conexão com ThingSpeak para envio dos dados.

// Definições de Variáveis Importantes ao Longo do Programa
#define DHT_SENSOR_PIN  21 // ESP32 Pino GIOP21 do ESP32 conectado ao sensor DHT11.
#define DHT_SENSOR_TYPE DHT11 // Tipo de sensor.

/* Configurações inicias para conexão com o WiFi e com a plataforma do ThingSpeak */
const char* ssid = "";
const char* password ="";
/* Configurando sensor DHT11 */
DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

/* Configurando variáveis associadas ao ThingSpeak. */
#define THINGSPEAK_WRITE_INTERVAL 30000 // Configurando intervalo adequado para escrita no ThingSpeak
const char * apiKeyEscrita = ""; // Chave de API responsável por escrever no Canal do ThingSpeak
unsigned long numeroDoCanal = 1906050;
WiFiClient client;
unsigned long ultimoTempoConexao = 0;

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
  ThingSpeak.setField(1, temperatura);
  int umidade = dht_sensor.readHumidity(); // Leitura da umidade.
  ThingSpeak.setField(2, umidade);
  
  if(millis() - ultimoTempoConexao > THINGSPEAK_WRITE_INTERVAL)
  {
    int resultado = ThingSpeak.writeFields(numeroDoCanal, apiKeyEscrita);
    if(resultado == 200)
    {
      Serial.println("Resultados atualizados com sucesso!");
    }
    ultimoTempoConexao = millis();
  }
  
}
