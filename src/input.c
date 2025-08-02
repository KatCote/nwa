#include <rit.h>
#include <settings.h>

volatile int cursor_pos = 0;

void* input_thread(void* arg)
{
    MEVENT event;
    mousemask(ALL_MOUSE_EVENTS, NULL);
    
    while (running) {
        int ch = getch();

        if (ch == 'q' || ch == 'Q') {
            running = 0;
        } else if (ch == KEY_MOUSE) {
            if (getmouse(&event) == OK) {
                if (event.y <=4 && event.y >= 2) {
                    for (int i = 0; i < 6; ++i) {
                        int x = 1 + i * 5;
                        if (event.x >= x && event.x <= x+3) {
                            cursor_pos = i*2 + (event.x-x-1)/2;
                            break;
                        }
                    }
                }
            }
        } else if (ch == KEY_BACKSPACE || ch == 127) {
            if (cursor_pos > 0 && cursor_pos < 12) {
                src_mac[cursor_pos] = '*';
                cursor_pos--;
            } else if (cursor_pos == 0) {
                src_mac[cursor_pos] = '*';
            }
        } else if (ch == '\n') {
            // Завершить ввод
            running = 0;
        } else if (isxdigit(ch)) {
            src_mac[cursor_pos] = toupper(ch);
            if (cursor_pos < 11) cursor_pos++;
        } else if (ch == ' ') {
            capture_pause = capture_pause ? 0 : 1;
        } else { continue; }
    }
    return NULL;
}