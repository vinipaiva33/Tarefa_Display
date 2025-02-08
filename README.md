# Projeto: Controle de LEDs, Display e Comunicação Serial com BitDogLab

## Descrição Geral

Este projeto tem como objetivo explorar o uso de interfaces de comunicação serial no microcontrolador RP2040 da placa BitDogLab. A implementação envolve o controle de LEDs comuns, LEDs endereçáveis WS2812, entrada de caracteres via UART e manipulação de botões com interrupções e debounce. Além disso, um display OLED SSD1306 é utilizado para exibir informações ao usuário.

## Objetivos

- Compreender a comunicação serial em microcontroladores.
- Implementar e utilizar os protocolos UART e I2C.
- Controlar LEDs comuns e LEDs endereçáveis WS2812.
- Implementar interrupções e debounce para botões.
- Desenvolver um projeto funcional que combine hardware e software.

## Componentes Utilizados

- **Microcontrolador**: RP2040 (BitDogLab)
- **Matriz de LEDs WS2812**: Conectada à GPIO 7
- **LED RGB**: Conectado às GPIOs 11, 12 e 13
- **Botão A**: Conectado à GPIO 5
- **Botão B**: Conectado à GPIO 6
- **Display OLED SSD1306**: Conectado via I2C (GPIO 14 e 15)

## Funcionalidades Implementadas

### 1. Modificação da Biblioteca `font.h`
- Foram adicionados caracteres minúsculos personalizados para exibição no display SSD1306.

### 2. Entrada de Caracteres via PC
- Utilizando o Serial Monitor do VS Code, os caracteres digitados são exibidos no display OLED.
- Se um número de 0 a 9 for digitado, o símbolo correspondente será exibido na matriz 5x5 de LEDs WS2812.

### 3. Controle do Botão A
- Pressionar o Botão A alterna o estado do LED Verde.
- O estado do LED é indicado no display SSD1306 e no monitor serial.

### 4. Controle do Botão B
- Pressionar o Botão B alterna o estado do LED Azul.
- O estado do LED é indicado no display SSD1306 e no monitor serial.

## Requisitos Técnicos Utilizados

1. **Interrupções**: Todas as funcionalidades dos botões são implementadas utilizando rotinas de interrupção (IRQ).
2. **Debounce via Software**: Foi implementada uma rotina de debounce para evitar leituras falsas dos botões.
3. **Controle de LEDs**: O projeto inclui LEDs comuns e WS2812, demonstrando domínio de diferentes tipos de controle.
4. **Uso do Display SSD1306**: Exibição de caracteres maiúsculos e minúsculos, bem como símbolos gráficos na matriz de LEDs.
5. **Comunicação UART**: O programa recebe entrada de caracteres via UART e os exibe no display.
6. **Organização do Código**: O código foi estruturado de forma modular e comentada para facilitar o entendimento.

## Como Executar o Projeto

### 1. Compilação
- Utilize o VS Code com a extensão do Raspberry Pi Pico e o SDK configurado.
- Compile o código e gere um arquivo `.uf2` para upload na placa.

### 2. Transferência para o BitDogLab
- Conecte a placa ao PC e entre no modo BOOTSEL.
- Transfira o arquivo `.uf2` gerado para a unidade correspondente ao RP2040.

### 3. Teste e Depuração
- Abra o Serial Monitor no VS Code para visualizar os logs e testar a entrada de caracteres.
- Pressione os botões A e B para testar a interação com os LEDs e a exibição no display OLED.
- Digite números no Serial Monitor e observe a atualização da matriz de LEDs WS2812.
- OBS: Os testes também podem ser feitos no diagram.json com a simulação via WOKWI

## Estrutura do Projeto

- `projetodisplay.c`: Código principal, contendo inicialização dos pinos, interrupções e laço principal.
- `ssd1306.h/.c`: Biblioteca para comunicação com o display OLED via I2C.
- `ws2812.pio.h`: Biblioteca para controle dos LEDs endereçáveis WS2812.
- `font.h`: Fonte customizada para exibição de caracteres no display.
- `diagram.json': Simualdor WOKWI para testes.

## Vídeo de Demonstração

(ESPAÇO PARA LINK)


	
