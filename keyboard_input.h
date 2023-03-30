#include<stdio.h>
#include <termios.h>            //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>             //STDIN_FILENO

#define CLEAR_SCREEN system("clear");


static struct termios old_config, new_config;

void fix_keyboard() {

    tcgetattr( STDIN_FILENO, &old_config);
    new_config = old_config;
    new_config.c_lflag &= ~(ICANON| ECHO);
    tcsetattr( STDIN_FILENO, TCSANOW, &new_config);
}

void restore_keyboard() {
    tcsetattr( STDIN_FILENO, TCSANOW, &old_config);
}
