# Exemplo simples de uso de Display OLED
# baseado no controlador SSD1306
# Comunicacao via I2C

import Adafruit_SSD1306

from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont

import time


#Iniciacoes de display
disp = Adafruit_SSD1306.SSD1306_128_64(rst=None, i2c_address=0x3c, i2c_bus=1)
disp.begin()
disp.clear()
disp.display()
width = disp.width
height = disp.height
image = Image.new('1', (width, height)) #imagem binaria (somente 1's e 0's)
draw = ImageDraw.Draw(image)

#Fonte para os caracteres
font = ImageFont.load_default()

# Escreve os digitos
d = 0
while(True):
    try:
        for l in range(8):
            for c in range(16):
                draw.text((c*8, l*8), str(d), font=font, fill=255)
                disp.image(image)
                disp.display()
                time.sleep(0.5)
                d = (d+1) % 10
        time.sleep(10)
        draw.rectangle((0,0,width,height), outline=0, fill=0)
        disp.clear()
        disp.display()
    except KeyboardInterrupt:
        exit(1)
