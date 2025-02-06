
//Inclusão de bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"
#include "ssd1306.h"
#include "font.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C


//Iniciando variáveis do
int numero = 0;
static volatile uint a = 0;

#define BUTTON_A 5
#define BUTTON_B 6
#define LED_AZUL 12
#define LED_VERDE 11
#define WS2812_PIN 7

#define NUM_PIXELS 25
bool IS_RGBW = false;
bool led_blue_active = false;
bool led_green_active = false;

// Inicializa a estrutura do display
ssd1306_t ssd; 
bool cor = true; 


// Variável global para armazenar a cor (Entre 0 e 255 para intensidade)
uint8_t led_r = 10; // Intensidade do vermelho
uint8_t led_g = 10; // Intensidade do verde
uint8_t led_b = 0; // Intensidade do azul


void pinosInit() //Incializa os pinos
{
    // Inicializa o pino do LED RGB e configura pro GPIO11 E GPIO12
    gpio_init(LED_AZUL);
    gpio_set_dir(LED_AZUL, GPIO_OUT);

    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);

    // Configurar o pino do botão A e B e habilitar o pull-up interno
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
}


// Buffer para armazenar quais LEDs estão ligados matriz 5x5
bool led_buffer[10][NUM_PIXELS] = {
    {0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0,},

    {0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0,},

    {0, 1, 1, 1, 0, 
    0, 1, 0, 0, 0, 
    0, 1, 1, 1, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0,},

    {0, 1, 1, 1, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0,},

    {0, 1, 0, 0, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 0, 1, 0,},

    {0, 1, 1, 1, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 1, 0, 0, 0, 
    0, 1, 1, 1, 0,},

    {0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 1, 0, 0, 0, 
    0, 1, 1, 1, 0,},

    {0, 1, 0, 0, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 0, 0, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0,},

    {0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0,},

    {0, 1, 1, 1, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0,},
};


static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}


void set_one_led(uint8_t r, uint8_t g, uint8_t b, int numero)
{
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (led_buffer[numero][i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
}


void pio (){
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
}


bool debounce(volatile uint32_t *last_time, uint32_t debounce_time) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    if (current_time - *last_time > debounce_time) {
        *last_time = current_time;
        return true;
    }
    return false;
}


void display(const char *text, int x, int y){
    
    // Atualiza o conteúdo do display com animações
        ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
        ssd1306_draw_string(&ssd, text, x, y); // Desenha uma string 
        ssd1306_send_data(&ssd); // Atualiza o display
}

void display_limpo(){
    ssd1306_fill(&ssd, !cor); // Limpa o display
}

void gpio_irq_handler(uint gpio, uint32_t events)
{
    // Obtém o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    static volatile uint32_t last_timeA = 0; // Armazena o tempo do último evento (em microssegundos)
    static volatile uint32_t last_timeB = 0; // Armazena o tempo do último evento (em microssegundos)



    // Verifica se passou tempo suficiente desde o último evento

     if (gpio == BUTTON_A && debounce(&last_timeA, 200000))
    {
        if (led_green_active == false){
         // Debouncing de 200ms
            last_timeA = current_time;
            printf("A: %d\n", a); // Para controle quando se usa o monitor serial para verificar se há bouncing
            a++;
            led_green_active = !led_green_active;
            gpio_put(LED_VERDE, led_green_active);
            display_limpo();
            display("Verde", 45, 20);
            display("Ligado", 40, 30);
        }

        else{
            // Debouncing de 200ms
            last_timeA = current_time;
            printf("A: %d\n", a); // Para controle quando se usa o monitor serial para verificar se há bouncing
            a++;
            led_green_active = !led_green_active;
            gpio_put(LED_VERDE, led_green_active);
            display_limpo();
            display("Verde", 45, 20);
            display("Desligado", 35, 30);

        }
    }
    
    else if (gpio == BUTTON_B && debounce(&last_timeA, 200000))
    {
            
            if (led_blue_active == false){
            last_timeA = current_time;
            printf("A: %d\n", a); // Para controle quando se usa o monitor serial para verificar se há bouncing
            a++;
            led_blue_active = !led_blue_active;
            gpio_put(LED_AZUL, led_blue_active);
            display_limpo();
            display("Azul", 50, 20);
            display("Ligado", 40, 30);
            }

            else{
            last_timeA = current_time;
            printf("A: %d\n", a); // Para controle quando se usa o monitor serial para verificar se há bouncing
            a++;
            led_blue_active = !led_blue_active;
            gpio_put(LED_AZUL, led_blue_active);
            display_limpo();
            display("Azul", 50, 20);
            display("Desligado", 35, 30);
            }
    }
}


void initI2C(){
        // I2C Initialisation. Using it at 400Khz.
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



int main()
{
    pio();
    stdio_init_all();
    pinosInit();
    initI2C();

//Led com interrupção
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

     
    while(true){
        sleep_ms(1000);
        display_limpo();
        display("Aguardando", 25, 20);
        display("Comando", 35, 30);
        

        printf("Digite um numero: \n");
        if (stdio_usb_connected())
            { // Certifica-se de que o USB está conectado
                char tecla;
                if (scanf("%c", &tecla) == 1)
                { // Lê caractere da entrada padrão
                    printf("Recebido: '%c'\n",tecla);

        
        switch (tecla) {
                    case '1': 
                        set_one_led(led_r,led_g,led_b,1);
                        display_limpo();
                        display("Comando 1", 28, 30);
                        break;
                    case '2': 
                        set_one_led(led_r,led_g,led_b,2);
                        display_limpo();
                        display("Comando 2", 28, 30);
                        break;
                    case '3': 
                        set_one_led(led_r,led_g,led_b,3);
                        display_limpo();
                        display("Comando 3", 28, 30);
                        break;
                    case '4':
                        set_one_led(led_r,led_g,led_b,4);
                        display_limpo();
                        display("Comando 4", 28, 30);
                        break;
                    case '5':
                        set_one_led(led_r,led_g,led_b,5);
                        display_limpo();
                        display("Comando 5", 28, 30);
                        break;
                    case '6':
                        set_one_led(led_r,led_g,led_b,6);
                        display_limpo();
                        display("Comando 6", 28, 30);
                        break;
                    case '7':
                        set_one_led(led_r,led_g,led_b,7);
                        display_limpo();
                        display("Comando 7", 28, 30);
                        break;
                    case '8':
                        set_one_led(led_r,led_g,led_b,8);
                        display_limpo();
                        display("Comando 8", 28, 30);
                        break;
                    case '9':
                        set_one_led(led_r,led_g,led_b,9);
                        display_limpo();
                        display("Comando 9", 28, 30);
                        break;
                    case '0':
                        set_one_led(led_r,led_g,led_b,0);
                        display_limpo();
                        display("Comando 0", 28, 30);
                        break;
            }
            }
        sleep_ms(1000);
            }
        }
    return 0;
}