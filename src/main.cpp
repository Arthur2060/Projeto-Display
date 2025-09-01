//--------------------------------INTERNET DAS COISAS - PROFº LUCAS FELFOLDI------------------------------------
/*
AUTOR       : LUCAS FELFOLDI
TURMA       :
AULA        : 6
PROJETO     : Código Base com função float_map
VERSÃO      : 1.0
DATA        :
COMENTARIOS :
*/
//_____________________________________________________________________________________________________________

// INCLUI BIBLIOTECAS------------------------------------------------------------------------------------------
#include <Arduino.h>      // Inclui bibliotecas de compatibilidade de funções do Arduino
#include <esp_task_wdt.h> // Biblioteca que permite manipular o watchdog no segundo núcleo
#include <LiquidCrystal_I2C.h>
TaskHandle_t Task1; // Declara tarefa que será executada no segundo núcleo (dual core)
//_____________________________________________________________________________________________________________

// DEFINE PINOS DO HARDWARE------------------------------------------------------------------------------------
// Saidas Digitais (LEDs)
#define LED_1 19
#define LED_2 18
#define LED_3 5
#define LED_4 4
#define LED_5 2

// Entradas Digitais (Botões)
#define BT_1 35
#define BT_2 32
#define BT_3 27
#define BT_4 25
#define BT_5 26

// Entradas Analógicas (Sensores)
#define POT 33
#define LDR 34

// Outras Definções de Hardware

//_____________________________________________________________________________________________________________
// DECLARA VARIÁVEIS E CONSTANTES GLOBAIS ----------------------------------------------------------------------
float ldr_value = 0;
int contador = 0;
unsigned long tempo_atual = 0;
unsigned long tempo_decorrido = 0;
//_____________________________________________________________________________________________________________

// INICIALIZA OBJETOS ----------------------------------------------------------------------------------------
LiquidCrystal_I2C lcd(0x27, 16, 2); // (Endereço em colunas, linhas)
//_____________________________________________________________________________________________________________

// DECLARA FUNÇÕES -------------------------------------------------------------------------------------------
float float_map(float x, float in_min, float in_max, float out_min, float out_max);
void loop_2();
void Task1code(void *pvParameters);

// TELAS
void tela1();
void tela2();
void tela3();

//_____________________________________________________________________________________________________________

// CONFIGURAÇÃO DO SISTEMA -------------------------------------------------------------------------------------
void setup()

{
  Serial.begin(9600); // Inicia comunicação Serial USB a 9600 bits por segundo (bps)

  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Configura LEDS como saídas (OUTPUT)
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  pinMode(LED_5, OUTPUT);

  // Configura BOTÕES como entradas (INPUT)
  pinMode(BT_1, INPUT);
  pinMode(BT_2, INPUT);
  pinMode(BT_3, INPUT);
  pinMode(BT_4, INPUT);
  pinMode(BT_5, INPUT);

  // xTaskCreatePinnedToCore(Task1code, "Task1", 10000, NULL, 0, &Task1, 0);

  xTaskCreatePinnedToCore(
      Task1code, // 1. A função que contém o código da tarefa
      "Task1",   // 2. Um nome descritivo para a tarefa
      10000,     // 3. O tamanho da memória "stack" para a tarefa
      NULL,      // 4. Parâmetros a serem passados para a tarefa (nenhum neste caso)
      0,         // 5. A prioridade da tarefa
      &Task1,    // 6. Um "handle" para controlar a tarefa depois
      0          // 7. O núcleo (core) da CPU onde a tarefa deve rodar
  );
}

//_____________________________________________________________________________________________________________

// LAÇO PRINCIPAL ----------------------------------------------------------------------------------------------

void loop()
{
  // TAREFAS RÁPIDAS

  // ENTRADA DE DADOS

  // PROCESSAMENTO DE DADOS

  // SAIDA DE DADOS

  tempo_atual = millis();

  if ((tempo_atual - tempo_decorrido) > 2000)
  {
    contador++;
    lcd.clear();
    tempo_decorrido = tempo_atual;
  }

    switch (contador)
  {
  case 1:
    tela1();
    break;
  case 2:
    tela2();
    break;
  case 3:
    tela3();
    break;
  default:
    contador = 0;
  }
}

//_____________________________________________________________________________________________________________

// LAÇO SECUNDARIO ----------------------------------------------------------------------------------------------

void loop_2()
{
  // TAREFAS LENTAS, COMO COMUNICAÇÕES COM OUTROS DISPOSITIVOS
  ldr_value = analogRead(LDR);
  ldr_value = float_map(ldr_value, 0, 4095, 100, 0);
}

//_____________________________________________________________________________________________________________

//-----------------------------------------------------------------------------------------------------------------------------------------------------------
// NÃO ALTERAR ESSA FUNÇÃO!
void Task1code(void *pvParameters)
{
  esp_task_wdt_init(3000, false);
  while (1)
  {
    loop_2();
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------

float float_map(float x, float in_min, float in_max, float out_min, float out_max)
{
  // Função que realiza proporções/ regra de três com variaveis REAIS
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void tela1()
{
  lcd.setCursor(0, 0);
  lcd.printf("%.1f%%", ldr_value);

  lcd.setCursor(0, 1);
  lcd.print("Luminosidade:");
  delay(1000);
}

void tela2()
{
  delay(500);
  lcd.setCursor(0, 0);
  lcd.print("+----ARTHUR----+");
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print("+---SENAI-SP---+");
}

void tela3()
{
  lcd.setCursor(0, 0);
  lcd.print("      IOT E     ");
  lcd.setCursor(0, 1);
  lcd.print("     BONITAO    ");
}