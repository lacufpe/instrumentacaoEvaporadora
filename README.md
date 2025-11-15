# instrumentacaoEvaporadora
Sistema de Instrumentação de uma unidade de teste de evaporação

Parte do projeto Materiais Inovadores e Sustentáveis para Supressão da Evaporação em Reservatórios no Semiárido de Pernambuco (**MAISSER-PE**), coordenado por Prof. Antonino.

O tanque de evaporação tem:

- Um tanque com água, com nível observado, que servirá de medida do quanto evaporou.
- Um túnel de vento, com ventoinha ajustável (FGZ-11) por dimmer (FGK-11) para definir o quanto de vento tem sobre o tanque.
- Um conjunto de lâmpadas para irradiar o tanque (RZ-11), também controlável através de dimmer (RK-11).

A instrumentação do sistema mede:

- A velocidade do vento sobre o tanque (FGT-11) - anemômetro.
- O nível de água sobre o tanque (LT-11) - Sensor de nível hidrostático.
- A temperatura da água em 3 diferentes profundidades (TT-11, TT-12 e TT-13) - Sensores de temperatura Pt100 com poço.
- A temperatura do ar e a humidade sobre o tanque (THT-11) - Sensor de humidade e temperatura SHT85.
- A radiação incidente sobre o tanque (RT-11) - Piranômetro SN300AL.
- Os sensores de saída analógica (FGT-11, LT-11, TT-11, TT-12, TT-13 e RT-11) serão tratados por um ADC externo - dois módulos ADS1220 de 4 canais, 12 bits.
- Tudo isto registrado 1 amostra por segundo por módulo eletrônico no painel (UR-11), implementado por um arduino R4 com shield de conexão por bornes e enviado a um servidor node-red.

# Pinos Utilizados:

CS ADC1: Pino 10
CS ADC2: Pino 9
SPI: MOSI, MISO, SCK (padrão R4) - para os ADCs
I2C: SDA, SCL (padrão R4) - para o SHT85

ADC1: FGT-11, LT-11, TT-11, TT-12
ADC2: TT-13, RT-11