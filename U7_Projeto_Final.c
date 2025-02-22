#include <stdio.h>              //
#include <stdlib.h>             //
#include "pico/stdlib.h"        //    
#include "hardware/adc.h"       //
#include "hardware/i2c.h"       //
#include "display/ssd1306.h"    //
#include "display/font.h"       //
#include "matriz/neopixel.h"    //
#include "buzzer/buzzer.h"      //

#define I2C_PORT i2c1      // (a fazer)
#define I2C_SDA 14         // Pinagem do SDA
#define I2C_SCL 15         // Pinagem do SCL  
#define endereco 0x3C      // (a fazer)
#define JOYSTICK_X_PIN 26  // Pinagem do eixo x do joystick

#define LED_RED 13         // Pinagem do LED vermelho
#define LED_BLUE 12        // Pinagem do LED azul 
#define LED_GREEN 11       // Pinagem do LED verde 
#define BUZZER_PIN 27      // Pinagem do buzzer 

ssd1306_t ssd; // Inicializa a estrutura do display
bool cor = true;

//protópico das funções criadas
void level_zero();
void level_um();
void level_dois();
void level_tres();
void level_quatro();
void level_cinco();
void setup_display();
void setup_leds();
void message_display(char c[5]);

int main()
{

  stdio_init_all();
  setup_display();
  setup_leds();
  pwm_init_buzzer(BUZZER_PIN);
  adc_init();
  adc_gpio_init(JOYSTICK_X_PIN);
  
  uint16_t adc_value_x;
  uint16_t adc_value_y;  
  
  char str_x[5];  // Buffer para armazenar a string
  char str_y[5];  // Buffer para armazenar a string  
  char str_nivel[5];
  
  while (true)
  {
    adc_select_input(0); // Seleciona o ADC para eixo X. O pino 26 como entrada analógica
    adc_value_x = adc_read();
    adc_select_input(1); // Seleciona o ADC para eixo Y. O pino 27 como entrada analógica
    adc_value_y = adc_read();    
    
    uint16_t level_msg = (adc_value_x )/ 5000;

    //sprintf(str_x, "%d", adc_value_x);  // Converte o inteiro em string
    sprintf(str_y, "%d", adc_value_y);  // Converte o inteiro em string
    sprintf(str_nivel, "%d", level_msg); // Converto o inteiro do nível atual sobre o nivel total em string


    if(adc_value_x > 2000 && adc_value_x <=2450){
      
      message_display("TANQUE CHEIO");
      level_cinco();
      npClear();
      gpio_put(LED_GREEN,true);

      
    }

    if(adc_value_x >2450 && adc_value_x <=2863){
      level_quatro();
      npClear();
      gpio_put(LED_GREEN,true);

      message_display("TANQUE CHEIO");
    }

    if(adc_value_x > 2863 && adc_value_x<=3272){
      level_tres();
      npClear();
      gpio_put(LED_GREEN,true);
      gpio_put(LED_RED,true);

      message_display("TANQUE MEDIO");
    }

    if(adc_value_x >3272 && adc_value_x<=3670){
      level_dois();
      npClear();
      gpio_put(LED_GREEN,true);
      gpio_put(LED_RED,true);
      sleep_ms(500);

      message_display("TANQUE MEDIO");
    }

    if(adc_value_x >3670 && adc_value_x < 4070){
      level_um();
      npClear();
      gpio_put(LED_RED, true);
      sleep_ms(500);

      message_display("TANQUE BAIXO");     
    }

    if(adc_value_x >= 4070){
      level_zero();
      npClear();
      gpio_put(LED_RED, true);

      message_display("TANQUE VAZIO");
      gpio_put(LED_RED, true);
      sleep_ms(500);
      beep(BUZZER_PIN, 500);
    }
  
    sleep_ms(100);
  }
}

void setup_leds(){
  
  gpio_init(LED_RED);
  gpio_init(LED_BLUE);
  gpio_init(LED_GREEN);
  gpio_set_dir(LED_RED, GPIO_OUT);
  gpio_set_dir(LED_BLUE, GPIO_OUT);
  gpio_set_dir(LED_GREEN, GPIO_OUT);

  npInit(LED_PIN);
  npClear();

}

void message_display(char c[5]){

ssd1306_fill(&ssd, !cor); // Limpa o display
ssd1306_rect(&ssd, 3, 3, 124, 60, cor, !cor); // Desenha um retângulo
ssd1306_line(&ssd, 3, 18, 123, 18, cor); // Desenha uma linha
ssd1306_line(&ssd, 64, 20, 64, 64, cor); // Desenha uma linha vertical
ssd1306_draw_string(&ssd, c, 16, 6); // Desenha uma string
ssd1306_send_data(&ssd); // Atualiza o display

}

void setup_display(){
  
  i2c_init(I2C_PORT, 400 * 1000);
  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_pull_up(I2C_SDA); // Pull up the data line
  gpio_pull_up(I2C_SCL); // Pull up the clock line
  
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
  ssd1306_config(&ssd); // Configura o display
  ssd1306_send_data(&ssd); // Envia os dados para o display

  // Limpa o display. O display inicia com todos os pixels apagados.
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);
}

void level_zero(){
  npSetLED(0,0,0,0);
  npWrite();
  

}

void level_um(){

  npSetLED(0,0,0,10);
    
  npWrite();

}

void level_dois(){
  npSetLED(0,0,0,10);
  npSetLED(1,0,0,50);
  npSetLED(8,0,0,50);
  npWrite();
}

void level_tres(){
  npSetLED(0,0,0,10);
  npSetLED(1,0,0,50);
  npSetLED(8,0,0,50);
  npSetLED(2,0,0,10);
  npSetLED(7,0,0,50);
  npSetLED(12,0,0,50);
  
  npWrite();
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
}