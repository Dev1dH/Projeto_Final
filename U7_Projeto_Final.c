/*  
    EMBARCATECH -  PROJETO FINAL UNIDADE 7
    ALUNO: DEVID HENRIQUE PEREIRA DOS SANTOS
    MATRÌCULA: TIC370100380

    Sistema para verificação do nível em tanques/reservatórios
    com alerta visual e sonoro.

*/

// Bibliotecas Utilizadas
#include <stdio.h>              //
#include "pico/stdlib.h"        //    
#include "hardware/adc.h"       //
#include "hardware/i2c.h"       //
#include "display/ssd1306.h"    //
#include "display/font.h"       //
#include "matriz/neopixel.h"    //
#include "buzzer/buzzer.h"      //

// Definação de pinagens

#define I2C_PORT  i2c1        // (a fazer)
#define UART_ID   uart0       // Seleciona a UART0
#define ENDERECO   0x3C       // (a fazer)
#define BAUD_RATE 115200      // Define a taxa de transmissão
#define UART_TX     0         // Pino GPIO usado para TX
#define UART_RX     1         // Pino GPIO usado para RX
#define I2C_SDA    14         // Pinagem do SDA
#define I2C_SCL    15         // Pinagem do SCL  
#define JOYSTICK_Y 26         // Pinagem do eixo x do joystick
#define LED_RED    13         // Pinagem do LED vermelho
#define LED_BLUE   12         // Pinagem do LED azul 
#define LED_GREEN  11         // Pinagem do LED verde 
#define BUZZER_A   21         // Pinagem do buzzer A

// Variáveis Globais
ssd1306_t ssd;    // Inicializa a estrutura do display

bool cor = true;// Variável booleana para define cor dos LEDs e display  

// Protópico das funções Criadas
void level_zero();
void level_um();
void level_dois();
void level_tres();
void level_quatro();
void level_cinco();
void setup_display();
void setup_leds();
void led_alerta();
void leds_turn_on(bool light_red, bool light_blue, bool light_green);
void message_display(char c[5],char valor[5], bool cor_20, bool cor_40, bool cor_60, bool cor_80, bool cor_100);

// Função Principal
int main()
{
  
  setup_display();                       // Configura o display
  setup_leds();                          // Configura os LEDs
  pwm_init_buzzer(BUZZER_A );            // Inicializa o buzzer A no PWM
  adc_init();                            // Inicializa o conversor analógico-digital
  adc_gpio_init(JOYSTICK_Y);             // Define o eixo x do joystick na conversão analógico-digital
  uart_init(UART_ID, BAUD_RATE);                // Inicializa a UART
  gpio_set_function(UART_TX, GPIO_FUNC_UART);   // Configura o pino 0 para TX
  gpio_set_function(UART_RX, GPIO_FUNC_UART);   // Configura o pino 1 para RX

  // Loop principal
  while (true)
  {

    // Definindo variável do tipo inteiro de 16 bits
    uint16_t adc_value_y;
   
    // Buffer para armazenar a string    
    char str_y[5]; 

    // Seleciona o ADC para eixo X do joystick
    adc_select_input(0);   
    
    // Variável para armazenar o valor lido no ADC do joystick
    adc_value_y = adc_read();  
    
    /*Valor mínimo lido no ADC (Joystick na posição central)
    valor é um pouco menor que 2048 para garantir que variações 
    para menos não atrapalhem a medição*/
    float adc_min = 2035.0; 
    
    /*Valor máximo lido no ADC (Joystick no máximo valor do eixo x)   
    valor um pouco maior que 4096 para garantir que varições para mais
    não atrapalhem a leitura*/
    float adc_max = 4100.0;    
    
    // Obtém o valor em percentual do nível do nível ADC
    uint16_t level_msg = ((adc_max - adc_value_y)/adc_min)*100;
    
    // Converte o inteiro em string
    sprintf(str_y, "%d", level_msg);  
    
    // Exibe no monitor serial o valor atual do ADC como teste de validação
    printf("ADC do joystick: %d\n", adc_value_y);
    
    // Executa uma função com base no valor do adc
    if(adc_value_y > 2000 && adc_value_y <=2450){
      // Imprime a mensagem "TANQUE CHEIO" no display e o nível em % do nível do reservatório
      message_display("TANQUE CHEIO",str_y, cor, cor, cor, cor, cor);
      level_cinco();                  // Nível cinco na matriz de LEDs
      leds_turn_on(!cor, !cor, cor);  // Luz verde ligada
    }

    else if(adc_value_y >2450 && adc_value_y <=2863){
      // Imprime a mensagem "TANQUE CHEIO" no display e o nível em % do nível do reservatório
      message_display("TANQUE CHEIO",str_y, cor, cor, cor, cor, !cor);
      level_quatro();                 // Nível quatro na matriz de LEDs
      leds_turn_on(!cor, !cor, cor);  // Luz verde ligada
    }

    else if(adc_value_y > 2863 && adc_value_y<=3272){
      // Imprime a mensagem "TANQUE MEDIO" no display e o nível em % do nível do reservatório
      message_display("TANQUE MEDIO",str_y, cor, cor, cor, !cor, !cor);
      level_tres();                 // Nível três na matriz de LEDs
      leds_turn_on(cor, !cor, cor); // Luz amarela ligada
    }

    else if(adc_value_y >3272 && adc_value_y<=3670){
      // Imprime a mensagem "TANQUE MEDIO" no display e o nível em % do nível do reservatório
      message_display("TANQUE MEDIO",str_y, cor, cor, !cor, !cor, !cor);
      level_dois();                 // Nível dois na matriz de LEDs
      leds_turn_on(cor, !cor, cor); // Luz amarela ligada
    }

    else if(adc_value_y >3670 && adc_value_y < 4070){
      // Imprime a mensagem "TANQUE BAIXO" no display e o nível em % do nível do reservatório
      message_display("TANQUE BAIXO",str_y, cor, !cor, !cor, !cor, !cor); 
      level_um();                     // Nível um na matriz de LEDs
      leds_turn_on(cor, !cor, !cor);  // Luz vermelha ligado      
    }

    else{
      // Imprime a mensagem "TANQUE VAZIO" no display e o nível em % do nível do reservatório
      message_display("TANQUE VAZIO",str_y, !cor, !cor, !cor, !cor, !cor);
      level_zero();         // Nivel zero na matriz de LEDs
      led_alerta();         // Alerta do pisca LED vermelho
      beep(BUZZER_A, 1000); // Alerta sonoro do buzzer a cada 1000 ms (1s)
    }
    
  }

  return 0;
}

// Função que define se os LEDs RGB estão acesos ou apagados
void leds_turn_on(bool light_red, bool light_blue, bool light_green){
  gpio_put(LED_RED, light_red);
  gpio_put(LED_BLUE, light_blue);
  gpio_put(LED_GREEN, light_green);
}

// Função para configurar os LEDs
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

// Função para piscar o LED vermelho em alerta
void led_alerta(){
      gpio_put(LED_RED, true);    // LED vermelho aceso
      sleep_ms(500);              // Espera por 500 ms
      gpio_put(LED_RED, false);   // LED vermelho apagado
      sleep_ms(500);              // Espera por 500 ms
}

// Função para configurar o layout do display
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

// Função para configurar o display no i2c
void setup_display(){
  
  i2c_init(I2C_PORT, 400 * 1000);
  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);  // Configura o pino do SDA na função I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);  // Configura o pino da SCL na função I2C
  gpio_pull_up(I2C_SDA);                      // Configura a SDA como um pull up 
  gpio_pull_up(I2C_SCL);                      // Configura a SCL como um pull up
  
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, ENDERECO, I2C_PORT); // Inicializa o display
  ssd1306_config(&ssd);                                         // Configura o display
  ssd1306_send_data(&ssd);                                      // Envia os dados para o display

  // Limpa o display. O display inicia com todos os pixels apagados.
  ssd1306_fill(&ssd, false);
  // Envia os dados para o display
  ssd1306_send_data(&ssd);
}

// Função para exibir o nível zero na matriz de LEDs
void level_zero(){
  npSetLED(0,0,0,0);
  npWrite();
  npClear();
}

// Função para exibir o nível um na matriz de LEDs
void level_um(){
  npSetLED(0,0,0,10);
  npWrite();
  npClear();
}

// Função para exibir o nível dois na matriz de LEDs
void level_dois(){
  npSetLED(0,0,0,10);
  npSetLED(1,0,0,10);
  npSetLED(8,0,0,10);
  npWrite();
  npClear();
}

// Função para exibir o nível três na matriz de LEDs
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

// Função para exibir o nível quatro na matriz de LEDs
void level_quatro(){
  npSetLED(0,0,0,10);
  npSetLED(1,0,0,10);
  npSetLED(8,0,0,10);
  npSetLED(2,0,0,10);
  npSetLED(7,0,0,10);
  npSetLED(12,0,0,10);
  npSetLED(3,0,0,10);
  npSetLED(6,0,0,10);
  npSetLED(13,0,0,10);
  npSetLED(16,0,0,10);
  npWrite();
  npClear();
}

// Função para exibir o nível cinco na matriz de LEDs
void level_cinco(){
  npSetLED(0,0,0,10);
  npSetLED(1,0,0,10);
  npSetLED(8,0,0,10);
  npSetLED(2,0,0,10);
  npSetLED(7,0,0,10);
  npSetLED(12,0,0,10);
  npSetLED(3,0,0,10);
  npSetLED(6,0,0,10);
  npSetLED(13,0,0,10);
  npSetLED(16,0,0,10);
  npSetLED(4,0,0,10);
  npSetLED(5,0,0,10);
  npSetLED(14,0,0,10);
  npSetLED(15,0,0,10);
  npSetLED(24,0,0,10);
  npWrite();
  npClear();
}
