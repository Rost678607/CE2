#define _POSIX_C_SOURCE 200809L

#include "engine/platform/terminal.h"

#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

static struct termios orig_termios;
static uint16_t cached_width = 80;
static uint16_t cached_height = 24;
static volatile sig_atomic_t size_updated = 1;

static void handle_winch(int sig) {
    (void)sig;
    size_updated = 1;
}

void ce2_platform_terminal_init() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
        perror("tcgetattr");
        exit(1); 
    }

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |=  (CS8);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        perror("tcsetattr");
        exit(1);
    }

    const char *init_seq = "\x1b[?25l"; 
    write(STDOUT_FILENO, init_seq, strlen(init_seq));

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_winch;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGWINCH, &sa, NULL);
    
    size_updated = 1; 
}

void ce2_platform_terminal_shutdown() {
    const char *end_seq = "\x1b[?25h\x1b[0m";
    write(STDOUT_FILENO, end_seq, strlen(end_seq));

    const char *clear_cmd = "\x1b[2J\x1b[H";
    write(STDOUT_FILENO, clear_cmd, 7);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void ce2_platform_terminal_get_size(uint16_t* width, uint16_t* height) {
    if (size_updated) {
        struct winsize ws;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
            cached_width = 80;
            cached_height = 24;
        } else {
            cached_width = (uint16_t)ws.ws_col;
            cached_height = (uint16_t)ws.ws_row;
        }
        size_updated = 0;
    }
    if (width) *width = cached_width;
    if (height) *height = cached_height;
}

void ce2_platform_terminal_write(const char *buffer, uint16_t width, uint16_t height) {
    if (!buffer) return;

    write(STDOUT_FILENO, "\x1b[H", 3);      //  Перемещение курсора в начало
    
    write(STDOUT_FILENO, buffer, width * height);
}

void ce2_platform_terminal_clear() {
    const char* clear_cmd = "\x1b[2J\x1b[H";
    write(STDOUT_FILENO, clear_cmd, 7);
}