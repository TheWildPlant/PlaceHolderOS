#include <stdint.h>

#define VGA_WIDTH 320
#define VGA_HEIGHT 200

void set_video_mode() {
    asm volatile (
        "mov $0x13, %ax\n"
        "int $0x10\n"
    );
}

void draw_pixel(int x, int y, uint8_t color) {
    uint8_t* VideoMemory = (uint8_t*)0xA0000;
    VideoMemory[y * VGA_WIDTH + x] = color;
}

void draw_rectangle(int x, int y, int width, int height, uint8_t color) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            draw_pixel(x + i, y + j, color);
        }
    }
}

void print_string(char* str, int row, int col) {
    unsigned short* VideoMemory = (unsigned short*)0xB8000;
    int offset = row * 80 + col;
    for (int i = 0; str[i] != '\0'; i++) {
        VideoMemory[offset + i] = (VideoMemory[offset + i] & 0xFF00) | str[i];
    }
}

void clear_screen() {
    unsigned short* VideoMemory = (unsigned short*)0xB8000;
    for (int i = 0; i < 80 * 25; i++) {
        VideoMemory[i] = (VideoMemory[i] & 0xFF00) | ' ';
    }
}

void kernel_main() {
    set_video_mode();
    draw_rectangle(50, 50, 100, 50, 0x0F); // Draw a white rectangle
    while (1) {
        // Keep the kernel running
    }
}