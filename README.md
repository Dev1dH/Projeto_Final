# Sistema para verificar o nível em reservatórios

https://github.com/user-attachments/assets/10c5f006-44a4-48c5-af1d-fb6aaae8fcc7



## Periféricos
1 - Buzzer A: conectado a GPIO 21, emite um sinal sonoro ajustado com o PWM.

2 - LED RGB: conectados às GPIOs 13, 12 e 11 corresponde, respectivamente, aos LEDs vermelho, azul e verde.

3 - Matriz de LEDs: conectado a GPIO 7, consiste em um conjunto de 25 LEDs formando uma matriz 5x5 onde podemos representar de maneira visual informações do projeto..

4- Joystick (eixo y): conectado a GPIO 26, consiste em um potenciômetro que se move no eixo y, o seu valor analógico é lido e posteriormente convertido em um sinal digital.

5 - SDA (Serial Data): Conectado a GPIO 14, é responsável por transferir e receber informações do barramento I2C do display OLED.

6 - SCL: (Serial Clock): Conectado a GPIO 15, é responsável por sincronizar o clock da comunicação do display ao barramento I2C.

## Arquivos 

- 'CMakeLists.txt'

## Bibliotecas

1 - “pico/stdlib.h” : Biblioteca específica da Raspberry Pi Pico fornecida pela Pico SDk, é responsável pela manipulação de GPIOs, inicialização do sistema, delays, dentre outros.

2 - “hardware/adc.h”: Biblioteca do Raspberry Pi Pico usada para Conversor Analógico-Digital (ADC), permitindo que seja feita a leitura dos pinos analógicos conectados.

3 - “hardware/i2c.h” : Biblioteca que permite a comunicação com dispositivos i2C, como é o caso do display OLED.

4 - “display/ssd1306.h”: Biblioteca criada para o controle do display OLED SSD1306 via i2c, sendo assim possível a visualização de imagens, textos e figuras geométricas no display.

5 - “display/fonte.h”: Biblioteca que define o conjunto de caracteres para serem exibidos no display SSD1306, como as letras do alfabeto (em maiúsculo) e os números de 0 a 9. 

6 - “matriz/neopixel.h”: Biblioteca usada para controlar os LEDs WS2812 (matriz de LEDs).

7 - “buzzer/buzzer.h”: Biblioteca usada para controlar o buzzer, capaz de emitir sons de diferentes frequências, auxiliada pela biblioteca “hardware/pwm”,  a qual está inserida.

8 - “hardware/pwm” : Biblioteca responsável pela Modulação por Largura de Pulso (PWM) para as saídas GPIOs, permitindo controlar a intensidade de LEDs e buzzers.

9 - “hardware/clocks.h”: Biblioteca que permite configurar e manipular os clocks no microcontrolador RP2040.
