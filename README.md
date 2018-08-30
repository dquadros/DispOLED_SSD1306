# DispOLED_SSD1306
Experiências com display OLED monocromático 128x64 com controlador SSD1306

Arduino

* DispOLED_I2C: comunicação I2C
* DispOLED_SPI3: comunicação "3wire SPI"
* DispOLED_SPI4: comunicação "4wire SPI"

Configuração dos displays com 6 ou 7 pinos:
* 4wire SPI: padrão: resistor em R3 e R4. R1, R2 e R8 vazios
* 3wire SPI: resistor em R2 e R4. R1, R3 e R8 vazios
* I2C: resistor em R1 e R4. Curto em R8. R2 e R3 vazios

