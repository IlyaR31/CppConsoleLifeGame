#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <printf.h>
#include <string>
#include <vector>
#include "helper_functions.h"
#include "screen.h"
using namespace std;


void Screen::enable_cursor(){
    printf("\033[?25h");
}
void Screen::disable_cursor(){
    printf("\033[?25l");
}
void Screen::move_cursor(int x, int y){
    printf("\033[%d;%dH", x, y);
}

Screen::Screen() {
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    width = w.ws_col;
    height = w.ws_row;
    matrix = new char*[height];
    for (int i = 0; i < height; i++) {
        matrix[i] = new char[width];
    }

    matrix_fg_colors = new string*[height];
    for (int i = 0; i < height; i++) {
        matrix_fg_colors[i] = new string[width];
    }

    matrix_bg_colors = new string*[w.ws_col];
    for (int i = 0; i < height; i++) {
        matrix_bg_colors[i] = new string[width];
    }
    clear_matrix();
    disable_cursor();
}


Screen::~Screen() {
    for (int i = 0; i < height; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    for (int i = 0; i < height; i++) {
        delete[] matrix_fg_colors[i];
    }
    delete[] matrix_bg_colors;
    for (int i = 0; i < height; i++) {
        delete[] matrix_bg_colors[i];
    }
    delete[] matrix_bg_colors;
    enable_cursor();
}

void Screen::clear_matrix(){
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            matrix[y][x] = ' ';
            matrix_fg_colors[y][x] = "";
            matrix_bg_colors[y][x] = "";
        }
    }
}

void Screen::set_char(int x, int y, char character){
    if (x < 0 || y < 0 || x >= width || y >= height) return;
    matrix[y][x] = character;
}

void Screen::set_fg_color(int x, int y, int color){
    if (x < 0 || y < 0 || x >= width || y >= height) return;
    matrix_fg_colors[y][x] = "\033[" + to_string(color) + "m";
}

void Screen::set_fg_color(int x, int y, int r, int g, int b){
    if (x < 0 || y < 0 || x >= width || y >= height) return;
    matrix_fg_colors[y][x] = "\033[38;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + ";" + "m";
}

void Screen::set_bg_color(int x, int y, int color){
    if (x < 0 || y < 0 || x >= width || y >= height) return;
    matrix_bg_colors[y][x] = "\033[" + to_string(color + 10) + "m";
}

void Screen::set_bg_color(int x, int y, int r, int g, int b){
    if (x < 0 || y < 0 || x >= width || y >= height) return;
    matrix_bg_colors[y][x] = "\033[48;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + ";" + "m";
}

void Screen::cout_matrix(){
    string result = "";
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            result += matrix_bg_colors[y][x] + matrix_fg_colors[y][x] + matrix[y][x] + "\033[0m";
        }
        result += "\n";
    }
    // move_cursor(0, 0);
    cout << "\033[H" + result; //\033
}