#include <vector>
#include <cstdlib>
#include "helper_functions.h"
#include "screen.h"
#include "keyboard.h"

class Life_game {
private:
    vector<vector<int> > matrix; // matrix for game cels
    int width, height; // screen width and height
public:
    Life_game(int width, int height) : width(width), height(height) {
        vector<int> matrix_part; // matrix part (width) for matrix setup
        for (int i = 0; i < height + 1; i++) {
            matrix_part.clear(); // reset matrix part
            for (int j = 0; j < width + 1; j++) {
                matrix_part.push_back(0); // push inactive sell
            }
            matrix.push_back(matrix_part); // add width of sells to matrix
        }
    }

    void set_cell(int x, int y, int state) {
        if (x < 0 || x >= width || y < 0 || y >= height) // check if x and y in matrix
            return;
        matrix[y][x] = state; // set cell state
    }

    int get_cell(int x, int y) {
        if (x < 0 || x >= width || y < 0 || y >= height) // check if x and y in matrix
            return 0;
        return matrix[y][x]; // return sell state
    }

    void clear() {
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
                matrix[j][i] = 0;  // reset matrix i, j
            }
        }
    }

    void print_matrix(Screen &scr, bool running = false) {
        for (int y = 0; y < height - 1; y++) {
            for (int x = 0; x < width - 1; x++) {
                if (x < 0 || x >= width || y < 0 || y >= height)
                    continue;
                if (!running) { // check if editor mode is enabled
                    if (matrix[y][x] == 1) { // check if cell is active
                        scr.set_char(x, y, '@'); // set character to '@'
                        int n = count_live_neighbors(x, y); // count neighbors
                        if (n >= 2 && n <= 3) { // check if the cell remains
                            n = 255; // set r and b to 255
                        } else {
                            n = 155; // set r and b to 155
                        }
                        scr.set_fg_color(x, y, n, 0, n); // set color of the cell
                    } else { // check if the cell is inactive
                        int a = count_live_neighbors(x, y); // count neighbors
                        if (a == 3) {
                            scr.set_char(x, y, '='); // set character to '=' if the cell is will appear
                        } else {
                            scr.set_char(x, y, '_');// set character to '_' if the cell is will be 0
                        }
                        int n = 60 + ((y % 2 + x) % 2) * 30; // color for chess board coloring
                        scr.set_fg_color(x, y, n , n, n); // set color to sell
                    }
                } else { // check if editor mode is disabled
                    if (matrix[y][x] == 1) { // check if cell is active
                        scr.set_char(x, y, '@'); // set character to '@'
                        scr.set_fg_color(x, y, 255, 0, 255); // set color to 255, 0, 255
                    }
                }
            }
        }
    }

    int count_live_neighbors(int x, int y) {
        // check if cell in matrix
        if (x < 0 || x >= width || y < 0 || y >= height)
            return 0; // return 0 if cell not in matrix
        int count = 0; // number of neighbors
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) // check if neighbor position is not x, y
                    continue;
                int nx = x + dx; // calculate cell x
                int ny = y + dy; // calculate cell y
                if (nx >= 0 && nx < width && ny >= 0 && ny < height) { // check if cell position in matrix
                    count += matrix[ny][nx]; // add 1 if neighbor is active and 0 if neighbor is inactive
                }
            }
        }
        return count; // return number of neighbors that are active
    }


    void update_matrix() {
        // set up new matrix
        vector< vector<int> > new_matrix;
        vector<int> matrix_part;
        for (int i = 0; i < height; i++) {
            matrix_part.clear();
            for (int j = 0; j < width; j++) {
                matrix_part.push_back(matrix[i][j]);
            }
            new_matrix.push_back(matrix_part);
        }

        // update new matrix
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int neighbors = count_live_neighbors(x, y); // count neighbors
                if (matrix[y][x] == 1 && (neighbors < 2 || neighbors > 3)) { // check if next state of sell is inactive
                    new_matrix[y][x] = 0; // update new matrix sell
                } else if (matrix[y][x] == 0 && neighbors == 3) { // check if next state of sell is active
                    new_matrix[y][x] = 1; // update new matrix sell
                }
            }
        }

        // copy matrix from new matrix
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                matrix[i][j] = new_matrix[i][j];
            }
        }
    }
};


int main() {
    Screen scr; // set up screen
    Keyboard keyboard1; // set up keyboard
    keyboard1.enable_mouse_tracking(); // enable mouse tracking
    Life_game game(scr.width, scr.height); // set up life game with screen width and height

    // set up time for waiting wait_time * 10 ms for updating life game
    int count = 0; // timer
    int wait_time = 10; // max timer wait_time * 10 ms

    bool prst = false; // check if mouse pressed
    bool pressed_type = false; // mouse pressed type: true is adding sell, false is removing sell
    bool game_running = false; // game if not editing mode

    bool running = true; // game running
    while (running) { // check if game running
        scr.clear_matrix(); // clear matrix
        if (game_running) { // check if not editing mode
            count++; // update timer
            if (count >= wait_time) { // check if timer is greater than waiting time
                game.update_matrix(); // update matrix
                count = 0; // reset timer
            }
        }

        game.print_matrix(scr, game_running); // print matrix
        scr.cout_matrix(); // print matrix

        sleepcp(10); // sleep for 10 ms

        vector<MouseButton> mou = keyboard1.get_mouse_events(); // get mouse events
        for (auto & i : mou) {
            int button = i.get_event(); // get mouse button (0 is left pressed, 3 is left unpressed)
            int x = i.get_x(); // get mouse position x
            int y = i.get_y(); // get mouse position y
            if (button == 0) { // check if left mouse button is pressed
                prst = true; // set mouse pressed to true
                if (game.get_cell(x, y) == 0) { // check if cell is inactive
                    pressed_type = true; // set mouse pressed type to adding
                } else {
                    pressed_type = false; // set mouse pressed type to removing
                }
            } else if (button == 3) { // check if left mouse button is unpressed
                prst = false; // set mouse pressed to false
            }
            if (prst && y >= 0 && y < scr.height && x >= 0 && x < scr.width) { // check if x, y in screen coordinates
                game.set_cell(x, y, (int) pressed_type); // set cell state to mouse pressed type
            }
        }

        vector<char> keys = keyboard1.get_keys(); // get pressed keys
        for (auto & key : keys) {
            if (key == ' ') { // check if ' ' is pressed
                game_running = !game_running; // enable or disable edit mode
            } else if (key == 'q') { // check if 'q' is pressed
                running = false; // quit the game
                break;
            } else if (key == 'c') { // check if 'c' is pressed
                game.clear(); // clear game
            } else if (key == 'r') { // check if 'r' is pressed
                for (int i = 0; i < scr.width; i++) {
                    for (int j = 0; j < scr.height; j++) {
                        srand(i+j+rand()); // set random seed
                        game.set_cell(i, j, (int) (rand() % 7 == 6)); // set random state to sell
                    }
                }
            } else if (key == 'd') { // check if 'd' is pressed
                game.update_matrix(); // update game
            } else if (key == 'w') { // check if 'w' is pressed
                wait_time--; // decrease wait time
            } else if (key == 's') { // check if 's' is pressed
                wait_time++; // increase wait time
            } else if (key == '0') { // check if '0' is pressed
                wait_time = 10; // set wait time to default value (100 ms)
            } else if (key == 'f') { // check if 'f' is pressed
                wait_time = 3; // set wait time to fast value (30ms)
            }
        }
    }
    cout << u8"\033[2J\033[1;1H"; // clear console
    keyboard1.disable_mouse_tracking(); // disable mouse tracking
    keyboard1.exit(); // exit keyboard (gives zsh abort :( )

    return 0;
}