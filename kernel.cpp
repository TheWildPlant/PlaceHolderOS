#include <cstdint>

#define VGA_WIDTH 320
#define VGA_HEIGHT 200

extern "C" void set_video_mode() {
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

void print_string(const char* str, int row, int col) {
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

struct Window {
    int x, y;
    int width, height;
    uint8_t* buffer;
};

Window create_window(int x, int y, int width, int height) {
    Window window;
    window.x = x;
    window.y = y;
    window.width = width;
    window.height = height;
    window.buffer = new uint8_t[width * height];
    return window;
}

void move_window(Window* window, int new_x, int new_y) {
    window->x = new_x;
    window->y = new_y;
}

void resize_window(Window* window, int new_width, int new_height) {
    window->width = new_width;
    window->height = new_height;
    delete[] window->buffer;
    window->buffer = new uint8_t[new_width * new_height];
}

void close_window(Window* window) {
    delete[] window->buffer;
}

void draw_window(Window* window) {
    for (int i = 0; i < window->width; i++) {
        for (int j = 0; j < window->height; j++) {
            draw_pixel(window->x + i, window->y + j, window->buffer[j * window->width + i]);
        }
    }
}

extern "C" void kernel_main() {
    set_video_mode();
    clear_screen();
    print_string("Welcome to GnoxOS", 0, 0);
    print_string("1. Option 1", 2, 0);
    print_string("2. Option 2", 3, 0);
    print_string("3. Option 3", 4, 0);
    print_string("Select an option: ", 6, 0);

    Window window = create_window(50, 50, 100, 50);
    draw_rectangle(0, 0, 100, 50, 0x0F); // Draw a white rectangle in the window buffer
    draw_window(&window);
    while (1) {
        // Keep the kernel running
    }
}