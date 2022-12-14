#include <SDL2/SDL.h>
#include <vector>
#include <random>
#include <iostream>
#include <ctime>
#include <fmt/core.h>

#define N 0
#define S 1 
#define W 2 
#define E 3

class Maze {
    std::size_t ROWS;
    std::size_t COLS;
    std::size_t start;
    std::size_t top;

    std::vector<bool> maze;
    std::vector<int> dirs { N, S, W, E };
    std::vector<int> DX { 0, 0, -1, 1 };
    std::vector<int> DY { -1, 1, 0, 0  };
    std::vector<std::size_t> stack;
    std::vector<std::size_t> path;
private:
    // drawing variables
    int spaceBetween;
    int sWidth;
    int sHeight;
    SDL_Renderer* renderer;
public:
    void setVisited(std::size_t row, std::size_t col) {
        std::size_t actualIndex = getIndex(row, col);
        maze[actualIndex] = true;
    }

    std::size_t getIndex(std::size_t row, std::size_t col) {
        std::size_t actualIndex = col + row * COLS;
        return actualIndex;
    }

    std::size_t getRow(std::size_t index) {
        return int(index / ROWS);
    }

    std::size_t getCol(std::size_t index) {
        return index % COLS;
    }

    void pickRandomStart() {
        srand(time(NULL));
        // std::size_t row = rand() % ROWS;
        // std::size_t col = rand() % COLS;
        // start = getIndex(row, col);
        // push(start);
        // path.emplace_back(start);
        // setVisited(row, col);
        start = 0;
        push(start);
        path.emplace_back(start);
        setVisited(0, 0);
    }

    bool cellFree(std::size_t row, std::size_t col) {
        return maze[getIndex(row, col)] == false;
    }

    void carvePath(std::size_t row, std::size_t col) {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(dirs.begin(), dirs.end(), g);
        for (size_t i = 0; i < 4; i++) {
            int dir = dirs[i]; 
            std::size_t nrow = DY[dir] + row;
            std::size_t ncol = DX[dir] + col;
            if (nrow < ROWS && nrow >= 0 && ncol < COLS && ncol >= 0 && cellFree(nrow, ncol)) {
                setVisited(nrow, ncol);
                push(getIndex(nrow, ncol));
                path.emplace_back(getIndex(nrow, ncol));
                return;
            }
        }
        pop();
    }

    void push(std::size_t index) {
        stack[top] = index;
        top++;
    }

    void pop() {
        top--;
    }
public:
    Maze(std::size_t r, std::size_t c, int space, SDL_Renderer* renderer): 
        ROWS(r), COLS(c), maze(r * c), top(0), start(0), spaceBetween(space), renderer(renderer) {
        path.reserve(r * c);
        stack.reserve(r * c);
        srand(time(NULL));
        for (size_t i = 0; i < r * c; i++)
        {
            maze[i] = false;
        }
        
    }
public:
    void generate() {
        pickRandomStart();
        while(top > 0) {
            std::size_t actualTop = top - 1; 
            carvePath(getRow(stack[actualTop]), getCol(stack[actualTop]));
        }
    }

    void printSteps() {
        for (const auto& index: path) {
            std::size_t row = getRow(index);
            std::size_t col = getCol(index);
            fmt::print("[{},{}],\n", row, col);
        }
    }

    void drawGrid(int w_width, int w_height) {
        sWidth = (w_width - (spaceBetween * (COLS + 1))) / COLS;
        sHeight = (w_height - (spaceBetween * (ROWS + 1))) / ROWS;
        int x { 0 };
        int y { 0 };
        int coordX = spaceBetween;
        int coordY = spaceBetween;
        while (x < COLS) {
            while (y < ROWS) {
                coordY = spaceBetween + (spaceBetween * y) + (sHeight* y);
                drawRect(coordX, coordY, sWidth, sHeight); 
                y++;
            }
            y = 0;
            x++;
            coordX = spaceBetween + (spaceBetween * x) + (sWidth * x);
        }
    }

    void drawMaze() {
        for (size_t i = 0; i < path.size() - 1; i++) {
            // get the first cell
            std::size_t firstIndex = path[i];
            std::size_t secondIndex = path[i + 1];

            std::size_t firstRow = getRow(firstIndex);
            std::size_t firstCol = getCol(firstIndex);
            std::size_t secondRow = getRow(secondIndex);
            std::size_t secondCol = getCol(secondIndex);

            std::size_t lowRow =  (firstRow < secondRow) ? firstRow : secondRow;
            std::size_t lowCol =  (firstCol < secondCol) ? firstCol: secondCol;

            // compute the difference between rows
            std::size_t diffRow = secondRow - firstRow;
            int passWidth;
            int passHeight;
            int passX;
            int passY;
            if (diffRow != 0) {
                passWidth = sWidth;
                passHeight = spaceBetween;
                passX = spaceBetween + (spaceBetween * lowCol) + (lowCol * sWidth);
                passY = spaceBetween + sHeight + (spaceBetween * lowRow) + (lowRow * sHeight);
            } else {
                passWidth = spaceBetween;
                passHeight = sHeight;
                passX = spaceBetween + sWidth + (spaceBetween * lowCol) + (lowCol * sWidth);
                passY = spaceBetween + (spaceBetween * lowRow) + (lowRow * sHeight);
            }
            drawRect(passX, passY, passWidth, passHeight);
        }
    }

    void drawRect(int x, int y, int width, int height) {
        SDL_Rect rect = { x, y, width, height };
        SDL_RenderFillRect(renderer, &rect);
    }

};
