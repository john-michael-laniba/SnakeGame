#include <iostream>
#include <vector>
#include <conio.h>   
#include <windows.h> 
#include <utility> // Ensure pair<int, int> works properly

using namespace std;

// Game window size
const int WIDTH = 20;
const int HEIGHT = 15;

// Directions for snake movement
enum Direction { STOP = 0, UP, DOWN, LEFT, RIGHT };
Direction dir = STOP;

// Difficulty Levels
enum Difficulty { EASY, MEDIUM, HARD };
Difficulty difficulty = MEDIUM;

// Snake and food variables
std::vector<std::pair<int, int > > snake; // Explicit std::vector
int foodX, foodY;
bool running = true;
int speed = 100; // Default speed (ms per frame)

// Function to check if food is placed on the snake
bool IsFoodOnSnake(int x, int y) {
    for (size_t i = 0; i < snake.size(); i++) { // Use size_t to avoid warnings
        if (snake[i].first == x && snake[i].second == y)
            return true;
    }
    return false;
}

// Function to place food randomly without colliding with snake
void PlaceFood() {
    do {
        foodX = rand() % (WIDTH - 2) + 1;  // Avoid placing on borders
        foodY = rand() % (HEIGHT - 2) + 1;
    } while (IsFoodOnSnake(foodX, foodY));
}

// Display the start screen
void ShowStartScreen() {
    system("cls");

    cout << "====================================\n";
    cout << "      ?? WELCOME TO SNAKE GAME ??  \n";
    cout << "====================================\n";
    cout << "  ?? [1] Easy   - Slow Snake    \n";
    cout << "  ?? [2] Medium - Normal Speed  \n";
    cout << "  ? [3] Hard   - Fast Snake    \n";
    cout << "  ?? Use W/A/S/D to Control!      \n";
    cout << "  ?? Eat 'F' to Grow!             \n";
    cout << "  ?? Avoid Walls and Yourself!    \n";
    cout << "====================================\n";
    cout << "  ? Press ENTER to Start Game!   \n";
    cout << "  ? Press Q to Quit              \n";
    cout << "====================================\n";

    bool selecting = true;
    while (selecting) {
        if (_kbhit()) {
            switch (_getch()) {
                case '1':
                    difficulty = EASY;
                    speed = 150; // Slowest
                    selecting = false;
                    break;
                case '2':
                    difficulty = MEDIUM;
                    speed = 100; // Normal
                    selecting = false;
                    break;
                case '3':
                    difficulty = HARD;
                    speed = 50; // Fastest
                    selecting = false;
                    break;
                case 13: // ENTER key
                    selecting = false;
                    break;
                case 'q':
                    exit(0);
            }
        }
    }
}

// Initialize the game
void Init() {
    snake.clear();
    snake.push_back(std::make_pair(WIDTH / 2, HEIGHT / 2)); // Use make_pair for compatibility
    PlaceFood();
    dir = RIGHT; // Ensure the snake starts moving
}

// Handle input
void HandleInput() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'w': if (dir != DOWN) dir = UP; break;
            case 's': if (dir != UP) dir = DOWN; break;
            case 'a': if (dir != RIGHT) dir = LEFT; break;
            case 'd': if (dir != LEFT) dir = RIGHT; break;
            case 'q': running = false; break;
        }
    }
}

// Update snake position
void Update() {
    pair<int, int> newHead = snake.front();

    switch (dir) {
        case UP:    newHead.second--; break;
        case DOWN:  newHead.second++; break;
        case LEFT:  newHead.first--; break;
        case RIGHT: newHead.first++; break;
        default: return;
    }

    // Check for wall collisions
    if (newHead.first <= 0 || newHead.first >= WIDTH - 1 || 
        newHead.second <= 0 || newHead.second >= HEIGHT - 1) {
        running = false;
        return;
    }

    // Check for self-collision *after* inserting new head
    for (size_t i = 0; i < snake.size(); i++) {
        if (newHead == snake[i]) {
            running = false;
            return;
        }
    }

    // Add new head
    snake.insert(snake.begin(), newHead);

    // Eat food
    if (newHead.first == foodX && newHead.second == foodY) {
        PlaceFood();
    } else {
        snake.pop_back();
    }
}

// Render the game
void Render() {
    system("cls");

    cout << "====================================\n";
    cout << "            ?? SNAKE GAME ??       \n";
    cout << "====================================\n";

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1) {
                cout << "#"; // Border
            } else if (x == foodX && y == foodY) {
                cout << "F"; // Food
            } else {
                bool isSnake = false;
                for (size_t i = 0; i < snake.size(); i++) {
                    if (snake[i].first == x && snake[i].second == y) {
                        cout << "O"; // Snake body
                        isSnake = true;
                        break;
                    }
                }
                if (!isSnake) cout << " ";
            }
        }
        cout << endl;
    }

    cout << "====================================\n";
    cout << " Score: " << snake.size() - 1 << "\n";
    cout << "====================================\n";
}

// Main game loop
void Run() {
    ShowStartScreen();
    Init();

    while (running) {
        HandleInput();
        Update();
        Render();
        Sleep(speed);
    }

    cout << "\nGame Over! Your Score: " << snake.size() - 1 << "\n";
    system("pause");
}

int main() {
    Run();
    return 0;
}
