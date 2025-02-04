#include <vector>
#include <termios.h>
#include <thread>
#include "keyboard.h"

MouseButton::MouseButton(int x, int y, int event): x(x), y(y), event(event){}
int MouseButton::get_x() {return x;}
int MouseButton::get_y() {return y;}
int MouseButton::get_event() {return event;}


void Keyboard::clear_keys(){
    keys_pressed.clear();
}
    

Keyboard::Keyboard() {
    mouse_enabled = false;
    tcgetattr(0, &tty);
    savetty = tty;
    tty.c_lflag &= ~(ISIG | ICANON | ECHO | CBRK);
    tty.c_cc[VMIN] = 1;
    tcsetattr(0, TCSAFLUSH, &tty);
    th1 = thread(&Keyboard::keyboard_listener, this);
}
Keyboard::~Keyboard() {
    tcsetattr(0, TCSANOW, &savetty);
    if (!exited) th1.join();
}

void Keyboard::keyboard_listener(){
    while (!exited){
        char c = getchar();
        if (c == '\033'){
            string seq = "";
            seq += (char)getchar();
            seq += (char)getchar();
            if (seq == "[M"){
                int button = (int)getchar() - 32;
                int x = (int)getchar() - 33;
                int y = (int)getchar() - 32;
                mouse_events.push_back(MouseButton(x, y, button));
            }
        } else {
            keys_pressed.push_back(c);
        }
    }
}

// void Keyboard::mouse_listener(){
//     while (!exited) {
//         int data = stdin->_read();
//         // if data == b"\033":
//         //     seq = sys.stdin.buffer.read(2)
//         //     if seq == b"[M":
//         //         # event = data[3:]
//         //         # if len(event) >= 3:
//         //         event = sys.stdin.buffer.read(3)
//         //         button = event[0] - 32
//         //         col = event[2] - 32 - 1
//         //         row = event[1] - 32 - 1
//         //         if {"values": (to_name(button), (row, col*2)), "type": "MOUSE"} not in events:
//         //             events.append({"values": (to_name(button), (row, col*2)), "type": "MOUSE"})
//     }
// }

vector<char> Keyboard::get_keys(){
    vector<char> copy;
    copy.swap(keys_pressed);
    clear_keys();
    return copy;
}

vector<MouseButton> Keyboard::get_mouse_events(){
    vector<MouseButton> copy;
    copy.swap(mouse_events);
    mouse_events.clear();
    return copy;
}

void Keyboard::exit(){
    exited = true;
    if (mouse_enabled){
        disable_mouse_tracking();
    }
    Keyboard::~Keyboard();
}

void Keyboard::enable_mouse_tracking(){
    printf("\033[?1003h");
    mouse_enabled = true;
}

void Keyboard::disable_mouse_tracking(){
    printf("\033[?1003l");
    mouse_enabled = false;
}
