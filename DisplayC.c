#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "ws2812.pio.h"

//Definição das constantes
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ENDERECO 0x3C

#define BOTAO_A 5
#define BOTAO_B 6
#define LED_VERDE 11
#define LED_VERMELHO 13
#define DEBOUNCE_DELAY 200 // Tempo de debounce em ms
#define WS2812_PIN 7
#define NUM_PIXELS 25
#define IS_RGBW false

// Estrutura do display
ssd1306_t ssd;

volatile int resposta_atual = -1;

// Declaração da função de callback
void botao_callback(uint gpio, uint32_t eventos);

// Função para enviar um pixel para a matriz de LEDs
static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

// Função para converter RGB para formato de 32 bits
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(g) << 16) | ((uint32_t)(r) << 8) | (uint32_t)(b);
}

// Função para desligar todos os LEDs da matriz
void clear_led_matrix() {
    for (int i = 0; i < NUM_PIXELS; i++) {
        put_pixel(0); // Define todos os LEDs como apagados (R=0, G=0, B=0)
    }
}

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

// Inicializa o PIO e o controlador WS2812
void inicializar_ws2812() {
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
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
    sleep_ms(2000);
    gpio_put(LED_VERDE, 0);
    gpio_put(LED_VERMELHO, 0);
}

// Perguntas da anamnese e captura de resposta
int perguntas_anamnese() {
    const char *perguntas[][5] = {
        {"Voce sente", "pressao para", "cumprir prazos", "apertados de", "entrega?"},
        {"Seu tempo de", "descanso eh", "insuficiente", "antes da nova", "jornada?"},
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
    int pontuacao_anamnese = 0;
    
    for (int i = 0; i < 10; i++) {
        resposta_atual = -1;
        exibir_pergunta(perguntas[i][0], perguntas[i][1], perguntas[i][2], perguntas[i][3], perguntas[i][4]);
        while (resposta_atual == -1) tight_loop_contents();
        respostas[i] = resposta_atual;
        exibir_resposta(resposta_atual ? "Sim" : "Nao");
        
        // Calcula a pontuação da anamnese
        if (i == 1) {
            if (respostas[i] == 0) pontuacao_anamnese++; // Se a resposta for "Não", soma 0 ponto
        } else {
            if (respostas[i] == 1) pontuacao_anamnese++; // Se a resposta for "Sim", soma 1 ponto
        }
    }
    
    return pontuacao_anamnese;
}

// Função para capturar sinais vitais via serial e exibir no display
int sinais_vitais() {
    char input[16];
    int frequencia_cardiaca, pressao_arterial_sistolica, pressao_arterial_diastolica, frequencia_respiratoria;
    int pontuacao_sinais_vitais = 0;

    // Solicitar a frequência cardíaca e exibir no serial monitor
    printf("Digite a frequencia cardiaca (bpm): ");
    scanf("%d", &frequencia_cardiaca);  
    printf("Frequencia cardiaca: %d bpm\n", frequencia_cardiaca);
    sleep_ms(2000);

    // Verifica se a frequência cardíaca está fora da faixa normal
    if (frequencia_cardiaca < 60 || frequencia_cardiaca > 100) pontuacao_sinais_vitais++;

    // Exibir no display
    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, "Frequencia", 8, 10);
    ssd1306_draw_string(&ssd, "cardiaca:", 8, 20);
    char str[16];  // Vetor maior para armazenar o valor convertido para string
    snprintf(str, sizeof(str), "%d bpm", frequencia_cardiaca);  // Converter o inteiro para string
    ssd1306_draw_string(&ssd, str, 8, 30);  // Exibe o valor no display
    ssd1306_send_data(&ssd);
    sleep_ms(2000);

    // Solicitar e exibir a pressão arterial
    printf("Digite a pressao arterial sistolica (mmHg): ");
    scanf("%d", &pressao_arterial_sistolica);
    printf("Digite a pressao arterial diastolica (mmHg): ");
    scanf("%d", &pressao_arterial_diastolica);
    printf("Pressao arterial: %d x %d mmHg\n", pressao_arterial_sistolica, pressao_arterial_diastolica);
    sleep_ms(2000);

    // Verifica se a pressão arterial está fora da faixa normal
    if (pressao_arterial_sistolica < 100 || pressao_arterial_sistolica > 140 ||
        pressao_arterial_diastolica < 60 || pressao_arterial_diastolica > 90) pontuacao_sinais_vitais++;

    // Exibir no display
    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, "Pressao", 8, 10);
    ssd1306_draw_string(&ssd, "arterial:", 8, 20);
    snprintf(str, sizeof(str), "%d x %d mmHg", pressao_arterial_sistolica, pressao_arterial_diastolica);  // Converter pressão arterial para string
    ssd1306_draw_string(&ssd, str, 8, 30);  // Exibe o valor no display
    ssd1306_send_data(&ssd);
    sleep_ms(2000);

    // Solicitar e exibir a frequência respiratória
    printf("Digite a frequencia respiratoria (mrpm): ");
    scanf("%d", &frequencia_respiratoria);
    printf("Frequencia respiratoria: %d mrpm\n", frequencia_respiratoria);
    sleep_ms(2000);

    // Verifica se a frequência respiratória está fora da faixa normal
    if (frequencia_respiratoria < 12 || frequencia_respiratoria > 20) pontuacao_sinais_vitais++;

    // Exibir no display
    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, "Frequencia", 8, 10);
    ssd1306_draw_string(&ssd, "respiratoria:", 8, 20);
    snprintf(str, sizeof(str), "%d mrpm", frequencia_respiratoria);  // Converter frequência respiratória para string
    ssd1306_draw_string(&ssd, str, 8, 30);  // Exibe o valor no display
    ssd1306_send_data(&ssd);
    sleep_ms(2000);

    return pontuacao_sinais_vitais;
}

//Função da carinha feliz
void exibir_carinha_feliz() {
    // Padrão da carinha feliz na matriz de LED 5x5
    const bool carinha_feliz[NUM_PIXELS] = {
        0, 1, 1, 1, 0,  // Linha 1
        1, 0, 0, 0, 1,  // Linha 2
        0, 0, 0, 0, 0,  // Linha 3
        0, 1, 0, 1, 0,  // Linha 4
        0, 1, 0, 1, 0   // Linha 5
    };

    // Exibe o padrão na matriz de LED
    for (int i = 0; i < NUM_PIXELS; i++) {
        put_pixel(carinha_feliz[i] ? urgb_u32(0, 100, 0) : 0); // Verde para a carinha feliz
    }

    printf("Carinha feliz exibida na matriz de LED.\n");
}

// Função da carinha triste
void exibir_carinha_triste() {
    // Padrão da carinha triste na matriz de LED 5x5
    const bool carinha_triste[NUM_PIXELS] = {
        1, 0, 0, 0, 1,  // Linha 1
        0, 1, 1, 1, 0,  // Linha 2
        0, 0, 0, 0, 0,  // Linha 3
        0, 1, 0, 1, 0,  // Linha 4
        0, 1, 0, 1, 0   // Linha 5
    };

    // Exibe o padrão na matriz de LED
    for (int i = 0; i < NUM_PIXELS; i++) {
        put_pixel(carinha_triste[i] ? urgb_u32(100, 0, 0) : 0); // Vermelho para a carinha triste
    }

    printf("Carinha triste exibida na matriz de LED.\n");
}

// Função para exibir o resultado da avaliação de fadiga
void exibir_resultado_fadiga(int pontuacao_total) {
    ssd1306_fill(&ssd, false);
    ssd1306_rect(&ssd, 3, 3, 122, 58, true, false);
    
    if (pontuacao_total >= 0 && pontuacao_total <= 5) {
        ssd1306_draw_string(&ssd, "Adequado", 8, 20);
        ssd1306_draw_string(&ssd, "para conduzir", 8, 40);
        gpio_put(LED_VERDE, 1);
        gpio_put(LED_VERMELHO, 0);
        exibir_carinha_feliz(); // Exibe carinha feliz na matriz de LED
    } else if (pontuacao_total >= 6 && pontuacao_total <= 9) {
        ssd1306_draw_string(&ssd, "Atencao!", 8, 20);
        ssd1306_draw_string(&ssd, "Recomenda-se", 8, 40);
        ssd1306_draw_string(&ssd, "descanso", 8, 50);
        gpio_put(LED_VERDE, 0);
        gpio_put(LED_VERMELHO, 1);
        exibir_carinha_triste(); // Exibe carinha triste na matriz de LED
    } else if (pontuacao_total >= 10 && pontuacao_total <= 13) {
        ssd1306_draw_string(&ssd, "Alerta!", 8, 20);
        ssd1306_draw_string(&ssd, "Descanso", 8, 40);
        ssd1306_draw_string(&ssd, "imediato", 8, 50);
        gpio_put(LED_VERDE, 0);
        gpio_put(LED_VERMELHO, 1);
        exibir_carinha_triste(); // Exibe carinha triste na matriz de LED
    } else if (pontuacao_total >= 14 && pontuacao_total <= 16) {
        ssd1306_draw_string(&ssd, "Perigo!", 8, 20);
        ssd1306_draw_string(&ssd, "Repouso", 8, 40);
        ssd1306_draw_string(&ssd, "urgente", 8, 50);
        gpio_put(LED_VERDE, 0);
        gpio_put(LED_VERMELHO, 1);
        exibir_carinha_triste(); // Exibe carinha triste na matriz de LED
    }
    
    ssd1306_send_data(&ssd);
    sleep_ms(2000);
    gpio_put(LED_VERDE, 0);
    gpio_put(LED_VERMELHO, 0);
    clear_led_matrix(); // Desliga a matriz de LED
}

int main() {
    stdio_init_all(); // Inicializa a comunicação serial
    inicializar_display();
    inicializar_gpio();
    inicializar_ws2812(); // Inicializa o PIO e o controlador WS2812

    while (true) {
        // Anamnese no display
        ssd1306_fill(&ssd, false);
        ssd1306_draw_string(&ssd, "ANAMNESE", 30, 30);
        ssd1306_send_data(&ssd);
        sleep_ms(2000);

        int pontuacao_anamnese = perguntas_anamnese();
        sleep_ms(2000); // Pausa antes de reiniciar o ciclo

        // Sinais vitais no display
        ssd1306_fill(&ssd, false);
        ssd1306_draw_string(&ssd, "SINAIS VITAIS", 10, 30);
        ssd1306_send_data(&ssd);
        sleep_ms(2000);

        int pontuacao_sinais_vitais = sinais_vitais();

        // Calcula a pontuação total
        int pontuacao_total = pontuacao_anamnese + (pontuacao_sinais_vitais * 2);

        // Exibe o resultado da avaliação de fadiga
        exibir_resultado_fadiga(pontuacao_total);
        sleep_ms(2000);
    }
}