/*  ALUNO: DEVID HENRIQUE PEREIRA DOS SANTOS
    MATRÌCULA: 

*/

// Bibliotecas Utilizadas
#include <stdio.h>              //
#include <stdlib.h>             //
#include "pico/stdlib.h"        //    
#include "hardware/adc.h"       //
#include "hardware/i2c.h"       //
#include "display/ssd1306.h"    //
#include "display/font.h"       //
#include "matriz/neopixel.h"    //
#include "buzzer/buzzer.h"      //

// Definação de pinagens
#define ENDERECO 0x3C      // (a fazer)
#define I2C_PORT i2c1      // (a fazer)
#define I2C_SDA    14      // Pinagem do SDA
#define I2C_SCL    15      // Pinagem do SCL  
#define JOYSTICK_X 26      // Pinagem do eixo x do joystick
#define LED_RED    13      // Pinagem do LED vermelho
#define LED_BLUE   12      // Pinagem do LED azul 
#define LED_GREEN  11      // Pinagem do LED verde 
#define BUZZER_A   21      // Pinagem do buzzer A

// Variáveis Globais
ssd1306_t ssd; // Inicializa a estrutura do display
bool cor = true;

// Protópico das Funções Criadas
void level_zero();
void level_um();
void level_dois();
void level_tres();
void level_quatro();
void level_cinco();
void setup_display();
void setup_leds();
void message_display(char c[5],char valor[5], bool cor_20, bool cor_40, bool cor_60, bool cor_80, bool cor_100);
void display_0();
void display_20();
void display_40();
void display_60();
void display_80();
void display_100();

// Função Principal
int main()
{
  setup_display();
  setup_leds();
  pwm_init_buzzer(BUZZER_A );
  adc_init();
  adc_gpio_init(JOYSTICK_X);
  
  // Loop principal
  while (true)
  {

    // Definindo variável do tipo inteiro de 16 bits
    uint16_t adc_value_x;
   
    // Buffer para armazenar a string    
    char str_x[5]; 

    // Seleciona o ADC para eixo X do joystick
    adc_select_input(0);   
    
    // Variável para armazenar o valor lido no ADC do joystick
    adc_value_x = adc_read();  
    
    // Valor mínimo lido no ADC (Joystick na posição central)
    float adc_min = 2048.0; 
    
    // Valor máximo lido no ADC (Joystick no máximo valor do eixo x)   
    float adc_max = 4100.0;    
    
    // Obtém o valor em percentual do nível do nível ADC
    uint16_t level_msg = ((adc_max - adc_value_x)/adc_min)*100;
    
    // Converte o inteiro em string
    sprintf(str_x, "%d", level_msg);  


    if(adc_value_x > 2000 && adc_value_x <=2450){
      
      message_display("TANQUE CHEIO",str_x, cor, cor, cor, cor, cor);
      level_cinco();
      gpio_put(LED_GREEN,true);
    }

    if(adc_value_x >2450 && adc_value_x <=2863){
      message_display("TANQUE CHEIO",str_x, cor, cor, cor, cor, !cor);
      level_quatro();
      gpio_put(LED_GREEN,true);
    }

    if(adc_value_x > 2863 && adc_value_x<=3272){
      message_display("TANQUE MEDIO",str_x, cor, cor, cor, !cor, !cor);
     
      level_tres();
      gpio_put(LED_GREEN,true);
      gpio_put(LED_RED,true);
    }

    if(adc_value_x >3272 && adc_value_x<=3670){
      message_display("TANQUE MEDIO",str_x, cor, cor, !cor, !cor, !cor);
      
      level_dois();
           
    }

    if(adc_value_x >3670 && adc_value_x < 4070){
      message_display("TANQUE BAIXO",str_x, cor, !cor, !cor, !cor, !cor); 
      
      level_um();          
    }

    if(adc_value_x >= 4070){
     
      message_display("TANQUE VAZIO",str_x, !cor, !cor, !cor, !cor, !cor);
      level_zero();
      
      gpio_put(LED_RED, true);
      gpio_put(LED_RED, true);
      sleep_ms(500);
      gpio_put(LED_RED, false);
      sleep_ms(500);

      beep(BUZZER_A , 1000);
    }
    
  }

  return 0;
}


void setup_leds(){
  
  stdio_init_all();
  gpio_init(LED_RED);                   // Inicializa a GPIO do LED vermelhor
  gpio_init(LED_BLUE);                  // Inicializa a GPIO do LED azul    
  gpio_init(LED_GREEN);                 // Inicializa a GPIO do LED verde
  gpio_set_dir(LED_RED, GPIO_OUT);      // Define o LED vermelho como uma saída GPIO
  gpio_set_dir(LED_BLUE, GPIO_OUT);     // Define o LED azul como uma saída GPIO
  gpio_set_dir(LED_GREEN, GPIO_OUT);    // Define o LED verde com uma saída GPIO
  npInit(LED_PIN);                      // Inicializa a matriz de LEDs
  npClear();

}

void message_display(char c[5], char valor[5], bool cor_20, bool cor_40, bool cor_60, bool cor_80, bool cor_100){

  ssd1306_fill(&ssd, !cor);                          // Limpa o display
  ssd1306_rect(&ssd, 3, 3, 124, 60, cor, !cor);      // Desenha um retângulo externo
  ssd1306_line(&ssd, 3, 14, 123, 14, cor);           // Desenha uma linha horizontal
  ssd1306_draw_string(&ssd, c, 16, 6);               // Escreve uma mensagem no display
  ssd1306_draw_string(&ssd, valor, 20, 32);          // Desenha uma string
  ssd1306_rect(&ssd, 15, 66, 12, 7, cor, cor_20);    // Retângulo tamanho 20%
  ssd1306_rect(&ssd, 25, 66, 20, 7, cor, cor_40);    // Retângulo tamanho 40%
  ssd1306_rect(&ssd, 35, 66, 36, 7, cor, cor_60);    // Retângulo tamanho 60%
  ssd1306_rect(&ssd, 45, 66, 48, 7, cor, cor_80);    // Retângulo tamanho 80%
  ssd1306_rect(&ssd, 55, 66, 60, 7, cor, cor_100);   // Retângulo tamanho 100%
  ssd1306_send_data(&ssd);                           // Atualiza o display
}

void setup_display(){
  
  i2c_init(I2C_PORT, 400 * 1000);
  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);  // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);  // Set the GPIO pin function to I2C
  gpio_pull_up(I2C_SDA);                      // Pull up the data line
  gpio_pull_up(I2C_SCL);                      // Pull up the clock line
  
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, ENDERECO, I2C_PORT); // Inicializa o display
  ssd1306_config(&ssd);                                         // Configura o display
  ssd1306_send_data(&ssd);                                      // Envia os dados para o display

  // Limpa o display. O display inicia com todos os pixels apagados.
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);
}

void level_zero(){
  npSetLED(0,0,0,0);
  npWrite();
  npClear();
}

void level_um(){
  npSetLED(0,0,0,10);
  npWrite();
  npClear();
}

void level_dois(){
  npSetLED(0,0,0,10);
  npSetLED(1,0,0,50);
  npSetLED(8,0,0,50);
  npWrite();
  npClear();
}

void level_tres(){
  npSetLED(0,0,0,10);
  npSetLED(1,0,0,50);
  npSetLED(8,0,0,50);
  npSetLED(2,0,0,10);
  npSetLED(7,0,0,50);
  npSetLED(12,0,0,50);
  npWrite();
  npClear();
}

void level_quatro(){
  npSetLED(0,0,0,10);
  npSetLED(1,0,0,50);
  npSetLED(8,0,0,50);
  npSetLED(2,0,0,10);
  npSetLED(7,0,0,50);
  npSetLED(12,0,0,50);
  npSetLED(3,0,0,50);
  npSetLED(6,0,0,50);
  npSetLED(13,0,0,10);
  npSetLED(16,0,0,50);
  npWrite();
  npClear();
}

void level_cinco(){
  npSetLED(0,0,0,10);
  npSetLED(1,0,0,50);
  npSetLED(8,0,0,50);
  npSetLED(2,0,0,10);
  npSetLED(7,0,0,50);
  npSetLED(12,0,0,50);
  npSetLED(3,0,0,50);
  npSetLED(6,0,0,50);
  npSetLED(13,0,0,10);
  npSetLED(16,0,0,50);
  npSetLED(4,0,0,50);
  npSetLED(5,0,0,50);
  npSetLED(14,0,0,10);
  npSetLED(15,0,0,50);
  npSetLED(24,0,0,50);
  npWrite();
  npClear();
}
