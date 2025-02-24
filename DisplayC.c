#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

int main()
{
  // I2C Initialisation. Using it at 400Khz.
  i2c_init(I2C_PORT, 400 * 1000);

  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_pull_up(I2C_SDA); // Pull up the data line
  gpio_pull_up(I2C_SCL); // Pull up the clock line
  ssd1306_t ssd; // Inicializa a estrutura do display
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
  ssd1306_config(&ssd); // Configura o display
  ssd1306_send_data(&ssd); // Envia os dados para o display

  // Limpa o display. O display inicia com todos os pixels apagados.
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);


  stdio_init_all(); // Inicializa a comunicação serial

  // Variáveis para armazenar os dados
  int frequencia_cardiaca = 0;
  int pressao_arterial = 0;
  int frequencia_respiratoria = 0;


  bool cor = true;
  while (true)
  {
    cor = !cor;

    // Atualiza o conteúdo do display -- ANAMNESE
    ssd1306_fill(&ssd, !cor); // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(&ssd, "ANAMNESE", 30, 30); // Desenha a primeira pergunta
    ssd1306_send_data(&ssd); // Atualiza o display
    sleep_ms(5000); // Aguardar 5 segundos


    // Atualiza o conteúdo do display com a primeira pergunta
    ssd1306_fill(&ssd, !cor); // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(&ssd, "Voce sente", 8, 10); // Desenha a primeira pergunta
    ssd1306_draw_string(&ssd, "pressao para", 8, 20); // Desenha a primeira pergunta
    ssd1306_draw_string(&ssd, "cumprir prazos", 8, 30); // Desenha a primeira pergunta
    ssd1306_draw_string(&ssd, "apertados de", 8, 40); // Desenha a primeira pergunta
    ssd1306_draw_string(&ssd, "entrega?", 8, 50); // Desenha a primeira pergunta  
    ssd1306_send_data(&ssd); // Atualiza o display
    sleep_ms(5000); // Aguardar 5 segundos

    // Atualiza o conteúdo do display com a segunda pergunta
    ssd1306_fill(&ssd, !cor); // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(&ssd, "Seu tempo de", 8, 10); // Desenha a segunda pergunta
    ssd1306_draw_string(&ssd, "descanso eh", 8, 20); // Desenha a segunda pergunta
    ssd1306_draw_string(&ssd, "suficiente", 8, 30); // Desenha a segunda pergunta
    ssd1306_draw_string(&ssd, "antes da nova", 8, 40); // Desenha a segunda pergunta
    ssd1306_draw_string(&ssd, "jornada?", 8, 50); // Desenha a segunda pergunta   
    ssd1306_send_data(&ssd); // Atualiza o display

    sleep_ms(5000); // Aguardar 5 segundos antes de repetir

     // Atualiza o conteúdo do display com a terceira pergunta
     ssd1306_fill(&ssd, !cor); // Limpa o display
     ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
     ssd1306_draw_string(&ssd, "Voce sente", 8, 10); // Desenha a terceira pergunta
     ssd1306_draw_string(&ssd, "dificuldades", 8, 20); // Desenha a terceira pergunta
     ssd1306_draw_string(&ssd, "para dormir?", 8, 30); // Desenha a terceira pergunta
     ssd1306_send_data(&ssd); // Atualiza o display
 
     sleep_ms(5000); // Aguardar 5 segundos antes de repetir

     // Atualiza o conteúdo do display com a quarta pergunta
     ssd1306_fill(&ssd, !cor); // Limpa o display
     ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
     ssd1306_draw_string(&ssd, "Voce sente", 8, 10); // Desenha a quarta pergunta
     ssd1306_draw_string(&ssd, "sonolencia", 8, 20); // Desenha a quarta pergunta
     ssd1306_draw_string(&ssd, "excessiva", 8, 30); // Desenha a quarta pergunta
     ssd1306_draw_string(&ssd, "durante", 8, 40); // Desenha a quarta pergunta
     ssd1306_draw_string(&ssd, "o dia?", 8, 50); // Desenha a quarta pergunta
     ssd1306_send_data(&ssd); // Atualiza o display
 
     sleep_ms(5000); // Aguardar 5 segundos antes de repetir

     // Atualiza o conteúdo do display com a quinta pergunta
     ssd1306_fill(&ssd, !cor); // Limpa o display
     ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
     ssd1306_draw_string(&ssd, "Costuma usar", 8, 10); // Desenha a quinta pergunta
     ssd1306_draw_string(&ssd, "cafeina ou ", 8, 20); // Desenha a quinta pergunta
     ssd1306_draw_string(&ssd, "energeticos", 8, 30); // Desenha a quinta pergunta
     ssd1306_draw_string(&ssd, "para se manter", 8, 40); // Desenha a quinta pergunta
     ssd1306_draw_string(&ssd, "acordado?", 8, 50); // Desenha a quinta pergunta
     ssd1306_send_data(&ssd); // Atualiza o display
 
     sleep_ms(5000); // Aguardar 5 segundos antes de repetir

     // Atualiza o conteúdo do display com a sexta pergunta
     ssd1306_fill(&ssd, !cor); // Limpa o display
     ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
     ssd1306_draw_string(&ssd, "Sente dores", 8, 10); // Desenha a sexta pergunta
     ssd1306_draw_string(&ssd, "musculares ou ", 8, 20); // Desenha a sexta pergunta
     ssd1306_draw_string(&ssd, "articulares", 8, 30); // Desenha a sexta pergunta
     ssd1306_draw_string(&ssd, "relacionadas", 8, 40); // Desenha a sexta pergunta
     ssd1306_draw_string(&ssd, "ao trabalho?", 8, 50); // Desenha a sexta pergunta
     ssd1306_send_data(&ssd); // Atualiza o display
 
     sleep_ms(5000); // Aguardar 5 segundos antes de repetir

     
     // Atualiza o conteúdo do display com a sétima pergunta
     ssd1306_fill(&ssd, !cor); // Limpa o display
     ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
     ssd1306_draw_string(&ssd, "Voce tem ", 8, 10); // Desenha a sétima pergunta
     ssd1306_draw_string(&ssd, "problemas de", 8, 20); // Desenha a sétima pergunta
     ssd1306_draw_string(&ssd, "visao ou", 8, 30); // Desenha a sétima pergunta
     ssd1306_draw_string(&ssd, "dor de cabeca", 8, 40); // Desenha a sétima pergunta
     ssd1306_draw_string(&ssd, "frequente?", 8, 50); // Desenha a sétima pergunta
     ssd1306_send_data(&ssd); // Atualiza o display
 
     sleep_ms(5000); // Aguardar 5 segundos antes de repetir   
     
    // Atualiza o conteúdo do display com a oitava pergunta
    ssd1306_fill(&ssd, !cor); // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(&ssd, "Ja sentiu", 8, 10); // Desenha a oitava pergunta
    ssd1306_draw_string(&ssd, "irritabilidade", 8, 20); // Desenha a oitava pergunta
    ssd1306_draw_string(&ssd, "ou estresse ou", 8, 30); // Desenha a oitava pergunta
    ssd1306_draw_string(&ssd, "ansiedade", 8, 40); // Desenha a oitava pergunta
    ssd1306_draw_string(&ssd, "no trabalho?", 8, 50); // Desenha a oitava pergunta
    ssd1306_send_data(&ssd); // Atualiza o display

    sleep_ms(5000); // Aguardar 5 segundos antes de repetir   

    // Atualiza o conteúdo do display com a nona pergunta
    ssd1306_fill(&ssd, !cor); // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(&ssd, "Voce tem", 8, 10); // Desenha a nona pergunta
    ssd1306_draw_string(&ssd, "hipertensao", 8, 20); // Desenha a nona pergunta
    ssd1306_draw_string(&ssd, "ou diabetes", 8, 30); // Desenha a nona pergunta
    ssd1306_draw_string(&ssd, "diagnosticada?", 8, 40); // Desenha a nona pergunta
    ssd1306_send_data(&ssd); // Atualiza o display

    sleep_ms(5000); // Aguardar 5 segundos antes de repetir
    
    // Atualiza o conteúdo do display com a décima pergunta
    ssd1306_fill(&ssd, !cor); // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(&ssd, "Faz uso de", 8, 10); // Desenha a décima pergunta
    ssd1306_draw_string(&ssd, "medicacao", 8, 20); // Desenha a décima pergunta
    ssd1306_draw_string(&ssd, "continua?", 8, 30); // Desenha a décima pergunta
    ssd1306_send_data(&ssd); // Atualiza o display

    sleep_ms(5000); // Aguardar 5 segundos antes de repetir

    // Atualiza o conteúdo do display -- SINAIS VITAIS
    ssd1306_fill(&ssd, !cor); // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(&ssd, "SINAIS VITAIS", 15, 30); // Desenha a primeira pergunta
    ssd1306_send_data(&ssd); // Atualiza o display
    sleep_ms(5000); // Aguardar 5 segundos

    // Pergunta 1: Frequência Cardíaca
    printf("Insira o número de batimentos cardíacos por minuto (frequência cardíaca): ");
    scanf("%d", &frequencia_cardiaca);

    // Exibe a pergunta e a resposta no display
    ssd1306_fill(&ssd, !cor); // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(&ssd, "Frequencia", 8, 20);
    ssd1306_draw_string(&ssd, "Cardiaca:", 8, 30);
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%d bpm", frequencia_cardiaca); // Formata a resposta
    ssd1306_draw_string(&ssd, buffer, 8, 40);
    ssd1306_send_data(&ssd); // Atualiza o display
    sleep_ms(5000); // Aguardar 5 segundos

    // Pergunta 2: Pressão Arterial
    printf("Insira o valor da pressão arterial aferida: ");
    scanf("%d", &pressao_arterial);

    // Exibe a pergunta e a resposta no display
    ssd1306_fill(&ssd, !cor); // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(&ssd, "Pressao", 8, 20);
    ssd1306_draw_string(&ssd, "Arterial:", 8, 30);
    snprintf(buffer, sizeof(buffer), "%d mmHg", pressao_arterial); // Formata a resposta
    ssd1306_draw_string(&ssd, buffer, 8, 40);
    ssd1306_send_data(&ssd); // Atualiza o display
    sleep_ms(5000); // Aguardar 5 segundos

    // Pergunta 3: Frequência Respiratória
    printf("Insira o número de respirações por minuto (frequência respiratória): ");
    scanf("%d", &frequencia_respiratoria);

    // Exibe a pergunta e a resposta no display
    ssd1306_fill(&ssd, !cor); // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(&ssd, "Frequencia", 8, 10);
    ssd1306_draw_string(&ssd, "Respiratoria:", 8, 20);
    snprintf(buffer, sizeof(buffer), "%d mrpm", frequencia_respiratoria); // Formata a resposta
    ssd1306_draw_string(&ssd, buffer, 8, 40);
    ssd1306_send_data(&ssd); // Atualiza o display
    sleep_ms(5000); // Aguardar 5 segundos

    // Exibe o resumo das informações no Serial Monitor
    printf("\nResumo dos sinais vitais:\n");
    printf("Frequencia Cardiaca: %d bpm\n", frequencia_cardiaca);
    printf("Pressao Arterial: %d mmHg\n", pressao_arterial);
    printf("Frequencia Respiratoria: %d rpm\n", frequencia_respiratoria);

    sleep_ms(5000); // Aguardar antes de reiniciar



  }
}