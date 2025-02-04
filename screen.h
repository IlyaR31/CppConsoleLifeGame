#ifndef screen
#define screen

#include <string>
#include <vector>
#include <thread>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include "helper_functions.h"

using namespace std;

class Screen{
private:
    char** matrix;
    struct winsize w;
    string** matrix_fg_colors;
    string** matrix_bg_colors;
    vector<char> chars;

    
    char c;
    void enable_cursor();
    void disable_cursor();
    void move_cursor(int x, int y);
public:
    int width, height;
    
    Screen();

    ~Screen();

    void clear_matrix();

    void set_char(int x, int y, char character);

    void set_fg_color(int x, int y, int color);

    void set_fg_color(int x, int y, int r, int g, int b);

    void set_bg_color(int x, int y, int color);

    void set_bg_color(int x, int y, int r, int g, int b);
    
    void cout_matrix();
};

#endif