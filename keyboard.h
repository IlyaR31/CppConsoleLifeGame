#ifndef keyboard
#define keyboard

#include <vector>
#include <termios.h>
#include <thread>
#include <iostream>

using namespace std;

class MouseButton{
private:
    int x, y;
    int event;
public:
    MouseButton(int x, int y, int event);
    int get_x();
    int get_y();
    int get_event();
};

class Keyboard{
private:
    vector<char> keys_pressed;
    vector<MouseButton> mouse_events;
    thread th1;
    bool exited;
    bool mouse_enabled;
    struct termios tty, savetty;

    void clear_keys();
public:
    Keyboard();
    ~Keyboard();
    vector<char> get_keys();
    vector<MouseButton> get_mouse_events();
    void keyboard_listener();

    // void mouse_listener();
    void exit();
    void enable_mouse_tracking();
    void disable_mouse_tracking();
};

#endif