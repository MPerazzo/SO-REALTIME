#include <io.h>
#include <naiveConsole.h>
#include <bga.h>

/* Functions to set up the VESA video mode */ 
/* Source: http://wiki.osdev.org/Bochs_VBE_Extensions */
void BgaWriteRegister(unsigned short IndexValue, unsigned short DataValue) {

    write_port_word(VBE_DISPI_IOPORT_INDEX, IndexValue);
    write_port_word(VBE_DISPI_IOPORT_DATA, DataValue);
}
 
unsigned short BgaReadRegister(unsigned short IndexValue) {

    write_port_word(VBE_DISPI_IOPORT_INDEX, IndexValue);
    return read_port_word(VBE_DISPI_IOPORT_DATA);
}
 
int BgaIsAvailable(void) {    

    return (BgaReadRegister(VBE_DISPI_INDEX_ID) == VBE_DISPI_ID4 ? 1 : 0);
}
 
 
void BgaSetBank(unsigned short BankNumber) {

    BgaWriteRegister(VBE_DISPI_INDEX_BANK, BankNumber);
    return;
}

void BgaGetCapabilities() {

    BgaWriteRegister(VBE_DISPI_INDEX_ENABLE,VBE_DISPI_GETCAPS);
    ncPrintDec(BgaReadRegister(VBE_DISPI_INDEX_XRES));
    ncNewline();
    ncPrintDec(BgaReadRegister(VBE_DISPI_INDEX_YRES));
    ncNewline();
    ncPrintDec(BgaReadRegister(VBE_DISPI_INDEX_BPP));
    ncNewline();

}

static unsigned int width = DEFAULT_WIDTH;
static unsigned int height = DEFAULT_HEIGHT;
static unsigned int bpp = DEFAULT_BPP;

void BgaSetVideoMode(unsigned int Width, unsigned int Height, unsigned int BitDepth, int UseLinearFrameBuffer, int ClearVideoMemory) {    

    BgaWriteRegister(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);
    BgaWriteRegister(VBE_DISPI_INDEX_XRES, Width);
    BgaWriteRegister(VBE_DISPI_INDEX_YRES, Height);
    BgaWriteRegister(VBE_DISPI_INDEX_BPP, BitDepth);
    BgaWriteRegister(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_ENABLED |  
          (UseLinearFrameBuffer ? VBE_DISPI_LFB_ENABLED : 0) | 
                 (ClearVideoMemory ? 0 : VBE_DISPI_NOCLEARMEM));

    width = Width;
    height = Height;
    bpp = BitDepth;    
}


/* Functions to manipulate the screen's pixels */

/* 
  Direccion que apunta a la direccion donde empieza el lfb (linear frame buffer)
  Hardcodeada del bootloader de Pure64. Para obtener esta direccion seteamos el modo vesa en 1
  y modificamos el codigo para que no inicialice en modo video (solo setea la direccion del buffer)
*/
uint8_t ** addressAddress = (uint8_t **)(0x0000000000005C00 + 40);

int BgaDrawPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b) {
    uint8_t * linearBuffer = *addressAddress;
    uint32_t pos = y * width * 3 + x * 3;

    if (pos >= width * height * 3) {
        return 0;
    }

    linearBuffer[pos++] = b;
    linearBuffer[pos++] = g;
    linearBuffer[pos] = r;

    return 1;
}

void BgaDrawRect(uint8_t r, uint8_t g, uint8_t b, uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    int i, j;

    for (i = x; i < x + w; i++) {
        for (j = y; j < y + h; j++) {
            if (!BgaDrawPixel(i, j, r, g, b)) {     // termina si algun pixel esta fuera de la pantalla
                return;
            }
        }
    }
}

void BgaFillScreen(uint8_t r, uint8_t g, uint8_t b) {
    uint8_t * linearBuffer = *addressAddress;
    int i;

    for (i = 0; i < width * height * 3;) {
        linearBuffer[i++] = b;
        linearBuffer[i++] = g;
        linearBuffer[i++] = r;
    }

}
