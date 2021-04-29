#include "kernel.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "idt/idt.h"
uint16_t *video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;
uint16_t terminal_make_char(char c, char colour)
{
    return (colour << 8) | c;
}
void terminal_putchar(int x, int y, char c, char color)
{
    video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(c, color);
}

bool terminal_newLine(char c)
{
    if (c == '\n')
    {
        terminal_row += 1;
        terminal_col = 0;
        return true;
    }
    return false;
}
void terminal_writechar(char c, char colour)
{
    bool new_line = terminal_newLine(c);
    if (!new_line)
    {
        terminal_putchar(terminal_col, terminal_row, c, colour);
        terminal_col += 1;
        if (terminal_col >= VGA_WIDTH)
        {
            terminal_col = 0;
            terminal_row += 1;
        }
    }
}
void terminal_initialize()
{
    video_mem = (uint16_t *)(0xB8000);
    for (int y = 0; y < VGA_HEIGHT; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            terminal_putchar(x, y, ' ', 0);
        }
    }
}
size_t strlen(const char *str)
{
    size_t len = 0;
    while (str[len])
    {
        len++;
    }
    return len;
}
void print(const char *str)
{
    size_t len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        terminal_writechar(str[i], 15);
    }
}


void kernel_main()
{
    terminal_initialize();
    print("Hello Lindows\nLinebreak");

    // Initialize interuppts
    idt_init();
}