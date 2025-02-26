# Projeto_final
Unidade 7 - Capítulo 1 - Projeto de Sistema Embarcado
  
Título: Sistema de Monitoramento de Fadiga para Motoristas de Caminhão de Transporte Rodoviário de Cargas  
  
Este projeto consiste em um sistema embarcado para monitoramento de fadiga em motoristas de caminhão de transporte rodoviário de cargas, utilizando o microcontrolador RP2040 e componentes como botões, LEDs RGB, matriz de LEDs WS2812, display SSD1306 e comunicação serial. O sistema coleta dados de anamnese e sinais vitais, calcula uma pontuação de fadiga e emite alertas visuais para indicar o nível de risco.  

# Componentes necessários  
• Microcontrolador RP2040: Controla todo o sistema e gerencia a comunicação entre os dispositivos  
• Botões (A e B): Capturam as respostas do usuário durante a anamnese  
• LEDs RGB: Indicam visualmente as respostas do usuário (verde para "Sim" e vermelho para "Não")  
• Display SSD1306 (128x64): Exibe as perguntas da anamnese, as respostas e os sinais vitais  
• Matriz de LEDs WS2812 (5x5): Exibe um ícone (carinha feliz ou triste) indicando o nível de fadiga do motorista  
• Comunicação Serial (UART): Recebe os sinais vitais digitados pelo usuário no PC e exibe os resultados no display  

# Pinagem utilizada  
GPIO 5:	Botão A - Entrada do usuário (Sim)  
GPIO 6: Botão B - Entrada do usuário (Não)  
GPIO 11: LED RGB (Verde) - Indicador visual (Sim)  
GPIO 13: LED RGB (Vermelho) -	Indicador visual (Não)  
GPIO 7: Matriz WS2812 -	Controle dos LEDs endereçáveis  
GPIO 14: I2C SDA - Comunicação com o display  
GPIO 15: I2C SCL - Comunicação com o display  

# Funcionalidades
O sistema realiza as seguintes funcionalidades:  
1) Anamnese:  
• Exibe perguntas no display SSD1306.  
• Captura respostas via botões A e B.  
• Armazena as respostas para avaliação de fadiga.  
  
2) Sinais Vitais:  
• Recebe dados de frequência cardíaca, pressão arterial e frequência respiratória via comunicação serial (UART).  
• Exibe os valores no display SSD1306.  
• Armazena os valores para avaliação de fadiga.  
  
3) Avaliação de Fadiga:  
• Calcula uma pontuação de fadiga com base nas respostas da anamnese e nos sinais vitais.  
• Exibe o resultado no display e na matriz de LEDs (carinha feliz/triste).

# Instruções de uso  
1) O sistema inicia exibindo a mensagem "ANAMNESE" no display.  
2) O usuário responde às perguntas da anamnese utilizando os botões A (Sim) e B (Não).  
3) Após a anamnese, o sistema solicita os sinais vitais via comunicação serial.  
4) O sistema calcula a pontuação de fadiga e exibe o resultado no display e na matriz de LEDs.

# Link do vídeo  


# Desenvolvedora  
Priscila Pereira Suzart de Carvalho
