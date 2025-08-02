#ifndef RIT_H_GUARD
#define RIT_H_GUARD  

/* I N C L U D E S */

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>

/* P R O T O T Y P E S */

int create_threads();
void* input_thread(void* arg);
void* render_thread(void* arg);
void* capture_thread(void* arg);

/* E X T E R N */

extern volatile int running;
extern volatile int cursor_pos;

/* D E F I N E S */

#define SETTINGS_WIN_WIDTH 31
#define TERM_MIN_WIDTH 80
#define TERM_MIN_HEIGHT 24

#endif