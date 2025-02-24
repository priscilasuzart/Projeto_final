#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "inc/ssd1306.h"
#include "inc/font.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ENDERECO 0x3C

#define BOTAO_A 5
#define BOTAO_B 6
#define LED_VERDE 11
#define LED_VERMELHO 13
#define DEBOUNCE_DELAY 200 // Tempo de debounce em ms

// Estrutura do display
ssd1306_t ssd;

volatile int resposta_atual = -1;

// Declaração da função de callback
void botao_callback(uint gpio, uint32_t eventos);

// Inicializa o display SSD1306
void inicializar_display() {
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ENDERECO, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
}

// Inicializa botões e LEDs
void inicializar_gpio() {
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);
    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &botao_callback);

    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);
    gpio_set_irq_enabled_with_callback(BOTAO_B, GPIO_IRQ_EDGE_FALL, true, &botao_callback);

    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);

    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
}

// Callback para interrupção dos botões
void botao_callback(uint gpio, uint32_t eventos) {
    static uint32_t ultimo_tempo = 0;
    uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());
    
    if (tempo_atual - ultimo_tempo < DEBOUNCE_DELAY) return;
    ultimo_tempo = tempo_atual;
    
    if (gpio == BOTAO_A) {
        resposta_atual = 1;
        gpio_put(LED_VERDE, 1);
        gpio_put(LED_VERMELHO, 0);
    } else if (gpio == BOTAO_B) {
        resposta_atual = 0;
        gpio_put(LED_VERDE, 0);
        gpio_put(LED_VERMELHO, 1);
    }
}

// Função para exibir perguntas no display
void exibir_pergunta(const char *linha1, const char *linha2, const char *linha3, const char *linha4, const char *linha5) {
    ssd1306_fill(&ssd, false);
    ssd1306_rect(&ssd, 3, 3, 122, 58, true, false);
    
    if (linha1) ssd1306_draw_string(&ssd, linha1, 8, 10);
    if (linha2) ssd1306_draw_string(&ssd, linha2, 8, 20);
    if (linha3) ssd1306_draw_string(&ssd, linha3, 8, 30);
    if (linha4) ssd1306_draw_string(&ssd, linha4, 8, 40);
    if (linha5) ssd1306_draw_string(&ssd, linha5, 8, 50);
    
    ssd1306_send_data(&ssd);
}

// Exibir resposta no display
void exibir_resposta(const char *resposta) {
    ssd1306_fill(&ssd, false);
    ssd1306_rect(&ssd, 3, 3, 122, 58, true, false);
    ssd1306_draw_string(&ssd, "Resposta:", 8, 20);
    ssd1306_draw_string(&ssd, resposta, 8, 40);
    ssd1306_send_data(&ssd);
    sleep_ms(5000);
    gpio_put(LED_VERDE, 0);
    gpio_put(LED_VERMELHO, 0);
}

// Perguntas da anamnese e captura de resposta
void perguntas_anamnese() {
    const char *perguntas[][5] = {
        {"Voce sente", "pressao para", "cumprir prazos", "apertados de", "entrega?"},
        {"Seu tempo de", "descanso eh", "suficiente", "antes da nova", "jornada?"},
        {"Voce sente", "dificuldades", "para dormir?", NULL, NULL},
        {"Voce sente", "sonolencia", "excessiva", "durante", "o dia?"},
        {"Costuma usar", "cafeina ou", "energeticos", "para se manter", "acordado?"},
        {"Sente dores", "musculares ou", "articulares", "relacionadas", "ao trabalho?"},
        {"Voce tem", "problemas de", "visao ou", "dor de cabeca", "frequente?"},
        {"Ja sentiu", "irritabilidade", "ou estresse ou", "ansiedade", "no trabalho?"},
        {"Voce tem", "hipertensao", "ou diabetes", "diagnosticada?", NULL},
        {"Faz uso de", "medicacao", "continua?", NULL, NULL}
    };
    int respostas[10];
    
    for (int i = 0; i < 10; i++) {
        resposta_atual = -1;
        exibir_pergunta(perguntas[i][0], perguntas[i][1], perguntas[i][2], perguntas[i][3], perguntas[i][4]);
        while (resposta_atual == -1) tight_loop_contents();
        respostas[i] = resposta_atual;
        exibir_resposta(resposta_atual ? "Sim" : "Nao");
    }
}

int main() {
    stdio_init_all(); // Inicializa a comunicação serial
    inicializar_display();
    inicializar_gpio();

    while (true) {
        // Anamnese no display
        ssd1306_fill(&ssd, false);
        ssd1306_draw_string(&ssd, "ANAMNESE", 30, 30);
        ssd1306_send_data(&ssd);
        sleep_ms(3000);

        perguntas_anamnese();
        sleep_ms(5000); // Pausa antes de reiniciar o ciclo
    }
}
