# Sistema para verificar o nível em reservatórios

## Periféricos
1 - Buzzer A: conectado a GPIO 21, emite um sinal sonoro ajustado com o PWM..
2 - LED RGB: conectados às GPIOs 13, 12 e 11 corresponde, respectivamente, aos LEDs vermelho, azul e verde.
3 - Matriz de LEDs: conectado a GPIO 7, consiste em um conjunto de 25 LEDs formando uma matriz 5x5 onde podemos representar de maneira visual informações do projeto..
4- Joystick (eixo y): conectado a GPIO 26, consiste em um potenciômetro que se move no eixo y, o seu valor analógico é lido e posteriormente convertido em um sinal digital.
5 - SDA (Serial Data): Conectado a GPIO 14, é responsável por transferir e receber informações do barramento I2C do display OLED.
6 - SCL: (Serial Clock): Conectado a GPIO 15, é responsável por sincronizar o clock da comunicação do display ao barramento I2C.
