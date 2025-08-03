#include <nwa.h>
#include <rit.h>
#include <settings.h>

volatile char src_mac[12] = {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'};
volatile char dstr_mac[12] = {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'};
volatile char src_ip[12] = {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'};
volatile char dstr_ip[12] = {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'};

int main()
{
    int retcode = EXIT_FAILURE;

    retcode = init();
    if (retcode != EXIT_SUCCESS) { goto exit; }

    retcode = create_threads();
    if (retcode != EXIT_SUCCESS) { goto exit; }

exit:

    deinit();

    if (retcode == EXIT_SUCCESS)
    { printf("Program exited successfully.\n"); }
    else
    { fprintf(stderr, "Program exited with an error. Code: %d\n", retcode); }

    return retcode;   
}

int create_threads()
{
    pthread_t render_tid, input_tid, capture_tid;

    if (pthread_create(&render_tid, NULL, render_thread, NULL) != 0) {
        perror("Failed to create render thread");
        return EXIT_FAILURE;
    }

    if (pthread_create(&input_tid, NULL, input_thread, NULL) != 0) {
        perror("Failed to create input thread");
        pthread_cancel(render_tid);
        return EXIT_FAILURE;
    }

    if (pthread_create(&capture_tid, NULL, capture_thread, NULL) != 0) {
        perror("Failed to create capture thread");
        pthread_cancel(render_tid);
        pthread_cancel(input_tid);
        return EXIT_FAILURE;
    }

    pthread_join(render_tid, NULL);
    pthread_join(capture_tid, NULL);
    pthread_join(input_tid, NULL);

    return EXIT_SUCCESS;
}

int init()
{
    int retcode = EXIT_FAILURE;
    
    setlocale(LC_ALL, "");

    initscr();

    if (has_colors())
    {
        start_color();  
        use_default_colors(); 

        init_color(11, 900, 900, 900);
        init_pair(1, 11, -1);

        init_color(12, 400, 400, 400);
        init_pair(2, 12, -1);

        init_color(13, 500, 500, 500);
        init_pair(3, 13, -1);

        init_color(14, 180, 180, 180);
        init_color(15, 700, 700, 700);
        init_pair(4, 15, 14);

        init_color(16, 200, 200, 200);
        init_color(17, 900, 900, 900);
        init_pair(5, 17, 16);

        /* Protocols */

        init_color(18, 228*4, 0, 0);
        init_color(19, 85*4, 0, 0);
        init_pair(6, 18, 19); // IPv4

        init_color(20, 0, 228*4, 0);
        init_color(21, 0, 85*4, 0);
        init_pair(7, 20, 21); // ARP

        init_color(22, 0, 0, 228*4);
        init_color(23, 0, 0, 85*4);
        init_pair(8, 22, 23); // IPv6

        init_color(24, 85*4, 0, 85*4);
        init_color(25, 228*4, 0, 228*4);
        init_pair(9, 25, 24); // Unknown

        init_color(26, 85*4, 85*4, 0);
        init_color(27, 228*4, 228*4, 0);
        init_pair(10, 27, 26); // VLAN

        init_color(28, 400, 0, 1000);
        init_color(29, 240, 0, 600);
        init_pair(11, 28, 29); // UDP

        init_color(30, 1000, 0, 600);
        init_color(31, 400, 0, 240);
        init_pair(12, 30, 31); // UDP

        init_color(32, 0, 1000, 800);
        init_color(33, 0, 600, 500);
        init_pair(13, 32, 33); // IGMP

        init_color(34, 500, 1000, 0);
        init_color(35, 400, 600, 0);
        init_pair(14, 34, 35); // ICMP
    }
	else
	{
        mvprintw(1, 1, "Term colors not supported. Exiting.");
    }

    cbreak(); 
    noecho();
    keypad(stdscr, TRUE); 
	curs_set(0);

    retcode = EXIT_SUCCESS;
    return retcode;
}

void deinit()
{
    endwin();
}