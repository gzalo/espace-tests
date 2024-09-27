#include <stdio.h>
#include <stdint.h>
#include "bmp.h"

static volatile uint8_t framebuffer_int[800*480*3];
static volatile uint32_t *framebuffer_src = (uint32_t*) framebuffer_int;
static volatile uint32_t *framebuffer_dst = (uint32_t*) 0x87D00000;
const int width = 800;
const int height = 480;
int frameIndex=0;

uint32_t seed = 12345;
#define A 1664525
#define C 1013904223
uint32_t lcg() {
    seed = (A * (seed) + C);
    return seed;
}

void copyFrameBuffer(){
    for(int i=0;i<288000;i++)
        framebuffer_dst[i] = framebuffer_src[i];
}

void displayInit(){
    for(int y=0; y<height; y++){
        for(int x=0; x<width; x++){
            int index = 3*(x + y*width);
            framebuffer_int[index] = 0xFF^x^seed;
            framebuffer_int[index+1] = 0x80^y^seed;
            framebuffer_int[index+2] = 0x40^(frameIndex);

            int xMod = (x/4) % BITMAP_WIDTH;
            int yMod = (y/4) % BITMAP_HEIGHT;

            unsigned char byte = bitmap[yMod * ((BITMAP_WIDTH + 7) / 8) + (xMod / 8)];
            int bit = (byte >> (7 - (xMod % 8))) & 1;

            // Set the pixel color based on the bit value
            if (bit) {
                framebuffer_int[index] = 255;     // White
                framebuffer_int[index + 1] = 255;
                framebuffer_int[index + 2] = 255;
            }
        }
    }
    lcg();
    frameIndex+=100;
    copyFrameBuffer();
}

int main(){
    while(1){
        displayInit();
        printf("Hello %2d\n", 42);
    }
}
