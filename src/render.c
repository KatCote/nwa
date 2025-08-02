#include <rit.h>
#include <charlib.h>
#include <settings.h>
#include <capture.h>

volatile int running = 1;

void* render_thread(void* arg)
{
    WINDOW *left_win = NULL, *right_win = NULL;
    int prev_height = 0, prev_width = 0;

    while (running) {
        int term_height, term_width;
        getmaxyx(stdscr, term_height, term_width);

        if (term_width < TERM_MIN_WIDTH || term_height < TERM_MIN_HEIGHT) {
            clear();
            mvprintw(0, 0, "Терминал слишком мал! Минимум: %dx%d", TERM_MIN_WIDTH, TERM_MIN_HEIGHT);
            mvprintw(0, 0, "Текущий размер: %dx%d", term_width, term_height);
            refresh();
            usleep(50000);
            continue;
        }

        if (term_height != prev_height || term_width != prev_width) {
            if (left_win) delwin(left_win);
            if (right_win) delwin(right_win);

            int left_width = SETTINGS_WIN_WIDTH;
            int right_width = term_width - left_width;

            left_win = newwin(term_height, left_width, 0, 0);
            right_win = newwin(term_height, right_width, 0, left_width);

            prev_height = term_height;
            prev_width = term_width;
        }

        werase(left_win);
        werase(right_win);

        wattron(left_win, COLOR_PAIR(2));

        // --- Левое окно ---
        werase(left_win);
        wborder(left_win, 0, 0, 0, 0, 0, 0, 0, 0);
        mvwaddstr(left_win, 0, 0, s_tlr);
        mvwaddstr(left_win, 0, SETTINGS_WIN_WIDTH-1, s_trr);
        mvwaddstr(left_win, term_height-1, 0, s_blr);
        mvwaddstr(left_win, term_height-1, SETTINGS_WIN_WIDTH-1, s_brr);

        wattroff(left_win, COLOR_PAIR(2));

        /* S E N D E R */

        wattron(left_win, COLOR_PAIR(3));

        mvwprintw(left_win, 0, 2, "┘SRC MAC⬎└");

        mvwprintw(left_win, 1, 1,
            s_tlr s_h s_h s_trr s_s
            s_tlr s_h s_h s_trr s_s
            s_tlr s_h s_h s_trr s_s
            s_tlr s_h s_h s_trr s_s
            s_tlr s_h s_h s_trr s_s
            s_tlr s_h s_h s_trr
        );

        mvwprintw(left_win, 2, 1,
            s_v s_s s_s s_v s_dd
            s_v s_s s_s s_v s_dd
            s_v s_s s_s s_v s_dd
            s_v s_s s_s s_v s_dd
            s_v s_s s_s s_v s_dd
            s_v s_s s_s s_v
        );

        mvwprintw(left_win, 3, 1,
            s_blr s_h s_h s_brr s_s
            s_blr s_h s_h s_brr s_s
            s_blr s_h s_h s_brr s_s
            s_blr s_h s_h s_brr s_s
            s_blr s_h s_h s_brr s_s
            s_blr s_h s_h s_brr
        );

        wattroff(left_win, COLOR_PAIR(3));

        wattron(left_win, COLOR_PAIR(1));
        for (int i = 0; i < 12; i+=2) { mvwprintw(left_win, 2, 2 + ((i * 2.5)), "%c%c", src_mac[i], src_mac[i+1]); }
        wattroff(left_win, COLOR_PAIR(1));

        wattron(left_win, COLOR_PAIR(2));
        mvwprintw(left_win, 4, 0, "├─────────────────────────────┤");
        wattroff(left_win, COLOR_PAIR(2));

        /* R E C E I V E R */

        wattron(left_win, COLOR_PAIR(3));

        mvwprintw(left_win, 4, 2, "┘DST MAC⬎└");

        mvwprintw(left_win, 5, 1,
            s_tlr s_h s_h s_trr s_s
            s_tlr s_h s_h s_trr s_s
            s_tlr s_h s_h s_trr s_s
            s_tlr s_h s_h s_trr s_s
            s_tlr s_h s_h s_trr s_s
            s_tlr s_h s_h s_trr
        );

        mvwprintw(left_win, 6, 1,
            s_v s_s s_s s_v s_dd
            s_v s_s s_s s_v s_dd
            s_v s_s s_s s_v s_dd
            s_v s_s s_s s_v s_dd
            s_v s_s s_s s_v s_dd
            s_v s_s s_s s_v
        );

        mvwprintw(left_win, 7, 1,
            s_blr s_h s_h s_brr s_s
            s_blr s_h s_h s_brr s_s
            s_blr s_h s_h s_brr s_s
            s_blr s_h s_h s_brr s_s
            s_blr s_h s_h s_brr s_s
            s_blr s_h s_h s_brr
        );

        wattroff(left_win, COLOR_PAIR(3));

        wattron(left_win, COLOR_PAIR(1));
        for (int i = 0; i < 12; i+=2) { mvwprintw(left_win, 6, 2 + ((i * 2.5)), "%c%c", dstr_mac[i], dstr_mac[i+1]); }
        wattroff(left_win, COLOR_PAIR(1));

        wattron(left_win, COLOR_PAIR(2));
        mvwprintw(left_win, 8, 0, "├─────────────────────────────┤");
        wattroff(left_win, COLOR_PAIR(2));

        // --- Правое окно ---
        werase(right_win);

        wattron(right_win, COLOR_PAIR(2));

        wborder(right_win, 0, 0, 0, 0, 0, 0, 0, 0);
        mvwaddstr(right_win, 0, 0, s_tlr);
        mvwaddstr(right_win, 0, term_width - SETTINGS_WIN_WIDTH - 1, s_trr);
        mvwaddstr(right_win, term_height-1, 0, s_blr);
        mvwaddstr(right_win, term_height-1, term_width - SETTINGS_WIN_WIDTH - 1, s_brr);

        int max_lines = term_height - 2;
        int lines_to_show = packet_count < max_lines ? packet_count : max_lines;
        int first_line = max_lines - lines_to_show + 1;

        for (int i = 0; i < lines_to_show; ++i) {
            int idx = (packet_head - 1 - i + MAX_PACKETS) % MAX_PACKETS;

            if (idx % 2 == 0) { wattron(right_win, COLOR_PAIR(4)); } else { wattron(right_win, COLOR_PAIR(5)); }
            
            for (int j = 0; j < (term_width - SETTINGS_WIN_WIDTH - 2); j++)
            { mvwaddch(right_win, first_line + lines_to_show - 1 - i, j + 1, ' '); }
            
            mvwprintw(right_win, first_line + lines_to_show - 1 - i, 3, "%s", packet_list[idx]);
            
            wattron(right_win, A_BOLD);

            if (packet_list[idx][0] == 0)
            {
                wattron(right_win, COLOR_PAIR(9));
                mvwprintw(right_win, first_line + lines_to_show - 1 - i, 1, "UKWN");
                wattroff(right_win, COLOR_PAIR(9));
            }
            else if (packet_list[idx][0] == 1)
            {
                wattron(right_win, COLOR_PAIR(6));
                mvwprintw(right_win, first_line + lines_to_show - 1 - i, 1, "IPv4");
                wattroff(right_win, COLOR_PAIR(6));
            }
            else if (packet_list[idx][0] == 2)
            {
                wattron(right_win, COLOR_PAIR(7));
                mvwprintw(right_win, first_line + lines_to_show - 1 - i, 1, "ARP ");
                wattroff(right_win, COLOR_PAIR(7));
            }
            else if (packet_list[idx][0] == 3)
            {
                wattron(right_win, COLOR_PAIR(8));
                mvwprintw(right_win, first_line + lines_to_show - 1 - i, 1, "IPv6");
                wattroff(right_win, COLOR_PAIR(8));
            }

            wattroff(right_win, A_BOLD);

            if (idx % 2 == 0) { wattroff(right_win, COLOR_PAIR(4)); } else { wattroff(right_win, COLOR_PAIR(5)); }
        }

        wrefresh(left_win);
        wrefresh(right_win);

        wattroff(right_win, COLOR_PAIR(2));

        usleep(500);
    }

    if (left_win) delwin(left_win);
    if (right_win) delwin(right_win);

    return NULL;
}