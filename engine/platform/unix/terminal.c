#define _POSIX_C_SOURCE 200809L

#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

static struct termios orig_termios;

static uint16_t cached_width = 0;
static uint16_t cached_height = 0;

static volatile sig_atomic_t size_updated = 1;

// Хендлер сигнала. Максимально тупой и быстрый.
static void handle_winch(int sig) {
    (void)sig;
    size_updated = 1;
}

static void update_cached_size(void) {
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

void ce2_platform_terminal_init(void) {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
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
        exit(1);
    }

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_winch;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGWINCH, &sa, NULL);
}

void ce2_platform_terminal_shutdown(void) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void ce2_platform_terminal_get_size(uint16_t* width, uint16_t* height) {

    if (!width || !height) return;

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

    *width = cached_width;
    *height = cached_height;
}