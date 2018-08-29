//
// Teste de comunicação com Display OLED
// baseado no controlador SSD1306
// Comunicação via I2C
//

#include <Wire.h>

// Endereço I2C do display
#define DISP_ADDR  0x3C

// Comandos
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

// Tamanho da tela
#define SSD1306_LCDWIDTH                  128
#define SSD1306_LCDHEIGHT                 64

// Iniciação
void setup()
{
  Wire.begin();
  Display_init();
}

// Laço principal
void loop()
{
  int d = 0;
  for (int l = 0; l < 8; l++)
  {
    for (int c = 0; c < 16; c++)
    {
      Display_write (l, c, d);
      if (++d == 10)
        d = 0;
      delay (500);
    }
  }
  delay (10000);
  Display_clear();
}


// Sequência de iniciação do display
byte cmdInit[] =
{
    SSD1306_DISPLAYOFF,
    SSD1306_SETDISPLAYCLOCKDIV, 0x80,
    SSD1306_SETMULTIPLEX, 0x3F,
    SSD1306_SETDISPLAYOFFSET, 0x00,
    SSD1306_SETSTARTLINE | 0x0,
    SSD1306_CHARGEPUMP, 0x14,
    SSD1306_MEMORYMODE, 0x00,
    SSD1306_SEGREMAP | 0x1,
    SSD1306_COMSCANDEC,
    SSD1306_SETCOMPINS, 0x12,
    SSD1306_SETCONTRAST, 0xCF,
    SSD1306_SETPRECHARGE, 0xF1,
    SSD1306_SETVCOMDETECT, 0x40,
    SSD1306_DISPLAYALLON_RESUME,
    SSD1306_NORMALDISPLAY,
    SSD1306_DISPLAYON
};

// Iniciação do display
void Display_init()
{
  Display_sendcmd (cmdInit, sizeof(cmdInit));
  Display_clear();
}

// Limpa o display
void Display_clear()
{
  // Define endereços iniciais e finais de colunas e páginas
  Display_sendcmd (SSD1306_COLUMNADDR);
  Display_sendcmd (0);
  Display_sendcmd (SSD1306_LCDWIDTH-1);
  Display_sendcmd (SSD1306_PAGEADDR);
  Display_sendcmd (0);
  Display_sendcmd (7);

  // Preenche a memória com zeros
  for (uint16_t i=0; i<(SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8); i++) 
  {
      Wire.beginTransmission(DISP_ADDR);
      Wire.write(0x40);  // Co=0, DC = 1
      for (uint8_t x=0; x<16; x++) 
      {
          Wire.write(0);
      }
      Wire.endTransmission();
  }
}

// Desenho dos números de 0 a 9
byte gc[][7] =
{
 {0x3E, 0x61, 0x51, 0x49, 0x45, 0x43, 0x3E} // 0
,{0x40, 0x40, 0x42, 0x7F, 0x40, 0x40, 0x40} // 1
,{0x42, 0x61, 0x51, 0x49, 0x45, 0x42, 0x40} // 2
,{0x22, 0x41, 0x49, 0x49, 0x49, 0x49, 0x36} // 3
,{0x10, 0x18, 0x14, 0x12, 0x11, 0x7F, 0x50} // 4
,{0x27, 0x45, 0x45, 0x45, 0x45, 0x49, 0x31} // 5
,{0x3C, 0x4A, 0x49, 0x49, 0x49, 0x49, 0x32} // 6
,{0x41, 0x21, 0x11, 0x09, 0x05, 0x03, 0x01} // 7
,{0x36, 0x49, 0x49, 0x49, 0x49, 0x49, 0x36} // 8
,{0x26, 0x49, 0x49, 0x49, 0x49, 0x29, 0x1E} // 9
};

// Escreve um caracter na linha l(0 a 7), coluna c(0 a 16)
void Display_write (byte l, byte c, byte car)
{
  byte *pc = gc[car];

  // Endereça o caracter
  Display_sendcmd (SSD1306_COLUMNADDR);
  Display_sendcmd (c*8);
  Display_sendcmd (c*8 + 7);
  Display_sendcmd (SSD1306_PAGEADDR);
  Display_sendcmd (l);
  Display_sendcmd (l);
  
  // Escreve
  Wire.beginTransmission(DISP_ADDR);
  Wire.write(0x40);  // Co=0, DC = 1
  for (uint8_t x = 0;  x < 7; x++) 
  {
    Wire.write(*pc++);
  }
  Wire.endTransmission();
}

// Envia sequência de comandos ao display
void Display_sendcmd (byte *cmd, int nCmds)
{
  Wire.beginTransmission(DISP_ADDR);
  Wire.write (0);  // Co= 0. DC = 0
  for (int i = 0; i < nCmds; i++)
  {
    Wire.write(cmd[i]);
  }
  Wire.endTransmission();
}

// Envia um byte de comando ao display
void Display_sendcmd (byte cmd)
{
  Wire.beginTransmission(DISP_ADDR);
  Wire.write (0);  // Co= 0. DC = 0
  Wire.write(cmd);
  Wire.endTransmission();
}

