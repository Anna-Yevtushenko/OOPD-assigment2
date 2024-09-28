#include <iostream>
#include <vector>
#include <string>

using namespace std;
const int BOARD_WIDTH = 80;
const int BOARD_HEIGHT = 25;

class Figure {
protected:
    static int nextID; 
    int id;
public:
    Figure() : id(nextID++) {}
    virtual void draw(std::vector<std::vector<char>>& grid) const = 0;
    virtual string getType() const = 0; 
    virtual string getParameters() const = 0;
    int getID() const { return id; } 
    virtual ~Figure() = default; 
};

int Figure::nextID = 1; 

class Triangle : public Figure {
private:
    int x, y;
    int height;
public:
    Triangle(int x, int y, int height) : x(x), y(y), height(height) {}

    void draw(std::vector<std::vector<char>>& grid) const override {
        for (int i = 0; i < height; ++i) {
            int leftMost = x - i;
            int rightMost = x + i;
            int posY = y + i;

            if (posY < BOARD_HEIGHT) {
                if (leftMost >= 0 && leftMost < BOARD_WIDTH)
                    grid[posY][leftMost] = '*';
                if (rightMost >= 0 && rightMost < BOARD_WIDTH && leftMost != rightMost)
                    grid[posY][rightMost] = '*';
            }
        }
        for (int j = 0; j < 2 * height - 1; ++j) {
            int baseX = x - height + 1 + j;
            int baseY = y + height - 1;
            if (baseX >= 0 && baseX < BOARD_WIDTH && baseY < BOARD_HEIGHT) {
                grid[baseY][baseX] = '*';
            }
        }
    }

    string getType() const override {
        return "triangle";
    }

    string getParameters() const override {
        return "x: " + to_string(x) + ", y: " + to_string(y) + ", height: " + to_string(height);
    }
};

class Rectangle : public Figure {
private:
    int x, y;
    int width;
    int height;
public:
    Rectangle(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}

    void draw(std::vector<std::vector<char>>& grid) const override {
        for (int j = x; j < x + width; ++j) {
            if (y >= 0 && y < BOARD_HEIGHT && j >= 0 && j < BOARD_WIDTH) {
                grid[y][j] = '*';
            }
        }

        for (int j = x; j < x + width; ++j) {
            if (y + height >= 0 && y + height < BOARD_HEIGHT && j >= 0 && j < BOARD_WIDTH) {
                grid[y + height][j] = '*';
            }
        }

        for (int i = y; i <= y + height; ++i) {
            if (i >= 0 && i < BOARD_HEIGHT && x >= 0 && x < BOARD_WIDTH) {
                grid[i][x] = '*';
            }
        }

        for (int i = y; i <= y + height; ++i) {
            if (i >= 0 && i < BOARD_HEIGHT && x + width >= 0 && x + width < BOARD_WIDTH) {
                grid[i][x + width] = '*';
            }
        }
    }

    string getType() const override {
        return "rectangle";
    }

    string getParameters() const override {
        return "x: " + to_string(x) + ", y: " + to_string(y) + ", width: " + to_string(width) + ", height: " + to_string(height);
    }
};

class Circle : public Figure {
private:
    int x_center, y_center;
    int radius;

public:
    Circle(int x, int y, int r) : x_center(x), y_center(y), radius(r) {}
    void draw(std::vector<std::vector<char>>& grid) const override {
        int r_squared = radius * radius;
        for (int i = 0; i < BOARD_HEIGHT; ++i) {
            for (int j = 0; j < BOARD_WIDTH; ++j) {
                int dx = j - x_center;
                int dy = i - y_center;
                int dist_squared = dx * dx + dy * dy;
                if (abs(dist_squared - r_squared) <= 1) {
                    grid[i][j] = '*';
                }
            }
        }
    }

    string getType() const override {
        return "circle";
    }

    string getParameters() const override {
        return "x_center: " + to_string(x_center) + ", y_center: " + to_string(y_center) + ", radius: " + to_string(radius);
    }
};

class Board {
private:
    vector<std::vector<char>> grid;
    vector<Figure*> figures;
public:
    Board() : grid(BOARD_HEIGHT, vector<char>(BOARD_WIDTH, ' ')) {}

    void print() {
        for (auto& row : grid) {
            for (char c : row) {
                cout << c;
            }
            cout << "\n";
        }
    }

    void addFigure(Figure* figure) {
        figures.push_back(figure);
        figure->draw(grid);
    }

    void clear() {
        for (auto& row : grid) {
            fill(row.begin(), row.end(), ' ');
        }
    }

    void list() const {
        if (figures.empty()) {
            cout << "No figures\n";
            return;
        }

        for (const auto& figure : figures) {
            cout  << figure->getID() << "  " << figure->getType()
                << ", " << figure->getParameters() << '\n';
        }
    }
};

int main() {
    vector<Figure*> figures;
    Board board;

    Circle* circle = new Circle(40, 12, 5);
    Circle* circle1 = new Circle(10, 8, 5);

    figures.push_back(circle);
    figures.push_back(circle1);

    for (const auto& figure : figures) {
        board.addFigure(figure);
    }
    board.print();
    board.list();

    for (Figure* figure : figures) {
        delete figure;
    }
    return 0;
}
