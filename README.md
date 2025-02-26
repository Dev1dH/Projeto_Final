# Sistema para verificar o nível em reservatórios


O vídeo completo de demonstração no youtube: https://youtu.be/bWMproWeIz

https://github.com/user-attachments/assets/10c5f006-44a4-48c5-af1d-fb6aaae8fcc7

## Periféricos
1 - **Buzzer A**: conectado a GPIO 21, emite um sinal sonoro ajustado com o PWM.

2 - **LED RGB**: conectados às GPIOs 13, 12 e 11 corresponde, respectivamente, aos LEDs vermelho, azul e verde.

3 - **Matriz de LEDs**: conectado a GPIO 7, consiste em um conjunto de 25 LEDs formando uma matriz 5x5 onde podemos representar de maneira visual informações do projeto..

4- **Joystick (eixo y)**: conectado a GPIO 26, consiste em um potenciômetro que se move no eixo y, o seu valor analógico é lido e posteriormente convertido em um sinal digital.

5 - **SDA (Serial Data)**: Conectado a GPIO 14, é responsável por transferir e receber informações do barramento I2C do display OLED.

6 - **SCL: (Serial Clock)**: Conectado a GPIO 15, é responsável por sincronizar o clock da comunicação do display ao barramento I2C.

## Arquivos 

- `CMakeLists.txt`: arquivo responsável pela compilação e configuração do projeto.
- `U7_Projeto_Final.c`: arquivo executável que carrega o programa principal.
- `pico_sdk_import.cmake`: arquivo responsável por importar e configurar a Pico SDK.
- `ws2818b.pio`: arquivo responsável pela configuração do arquivo .pio da matriz de LEDs. 

## Bibliotecas

1 - “pico/stdlib.h”: Biblioteca específica da Raspberry Pi Pico fornecida pela Pico SDk, é responsável pela manipulação de GPIOs, inicialização do sistema, delays, dentre outros.

2 - “hardware/adc.h”: Biblioteca do Raspberry Pi Pico usada para Conversor Analógico-Digital (ADC), permitindo que seja feita a leitura dos pinos analógicos conectados.

3 - “hardware/i2c.h” : Biblioteca que permite a comunicação com dispositivos i2C, como é o caso do display OLED.

4 - “display/ssd1306.h”: Biblioteca criada para o controle do display OLED SSD1306 via i2c, sendo assim possível a visualização de imagens, textos e figuras geométricas no display.

5 - “display/fonte.h”: Biblioteca que define o conjunto de caracteres para serem exibidos no display SSD1306, como as letras do alfabeto (em maiúsculo) e os números de 0 a 9. 

6 - “matriz/neopixel.h”: Biblioteca usada para controlar os LEDs WS2812 (matriz de LEDs).

7 - “buzzer/buzzer.h”: Biblioteca usada para controlar o buzzer, capaz de emitir sons de diferentes frequências, auxiliada pela biblioteca “hardware/pwm”,  a qual está inserida.

8 - “hardware/pwm” : Biblioteca responsável pela Modulação por Largura de Pulso (PWM) para as saídas GPIOs, permitindo controlar a intensidade de LEDs e buzzers.

9 - “hardware/clocks.h”: Biblioteca que permite configurar e manipular os clocks no microcontrolador RP2040.

## Blocos de funções

```markdown
```C
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
```

```markdown
```C
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
