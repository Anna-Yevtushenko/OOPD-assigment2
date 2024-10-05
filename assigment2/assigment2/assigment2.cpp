#include <iostream>
#include <vector>
#include <string>
#include <fstream> 


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
    virtual void remove(std::vector<std::vector<char>>& grid) const = 0;
    virtual bool isWithinBoard() const = 0;
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
    bool isWithinBoard() const override {
        return (x - height >= 0 && x + height < BOARD_WIDTH &&
            y >= 0 && y + height < BOARD_HEIGHT);
    }

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
    void remove(std::vector<std::vector<char>>& grid) const override {
        for (int i = 0; i < height; ++i) {
            int leftMost = x - i;
            int rightMost = x + i;
            int posY = y + i;

            if (posY < BOARD_HEIGHT) {
                if (leftMost >= 0 && leftMost < BOARD_WIDTH)
                    grid[posY][leftMost] = ' ';  
                if (rightMost >= 0 && rightMost < BOARD_WIDTH && leftMost != rightMost)
                    grid[posY][rightMost] = ' ';  
            }
        }
        for (int j = 0; j < 2 * height - 1; ++j) {
            int baseX = x - height + 1 + j;
            int baseY = y + height - 1;
            if (baseX >= 0 && baseX < BOARD_WIDTH && baseY < BOARD_HEIGHT) {
                grid[baseY][baseX] = ' ';  
            }
        }
    }

    int getX() {
        return x;
    }

    int getY() {
        return y;
    }

    int getHeight() {
        return height;
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

    bool isWithinBoard() const override {
        return (x - height >= 0 && x + height < BOARD_WIDTH &&
            y >= 0 && y + height < BOARD_HEIGHT);
    }

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


    void remove(std::vector<std::vector<char>>& grid) const override {
        for (int j = x; j < x + width; ++j) {
            if (y >= 0 && y < BOARD_HEIGHT && j >= 0 && j < BOARD_WIDTH) {
                grid[y][j] = ' ';  
            }
        }

        for (int j = x; j < x + width; ++j) {
            if (y + height >= 0 && y + height < BOARD_HEIGHT && j >= 0 && j < BOARD_WIDTH) {
                grid[y + height][j] = ' ';
            }
        }

        for (int i = y; i <= y + height; ++i) {
            if (i >= 0 && i < BOARD_HEIGHT && x >= 0 && x < BOARD_WIDTH) {
                grid[i][x] = ' ';
            }
        }

        for (int i = y; i <= y + height; ++i) {
            if (i >= 0 && i < BOARD_HEIGHT && x + width >= 0 && x + width < BOARD_WIDTH) {
                grid[i][x + width] = ' ';
            }
        }
    }

    int getX() {
        return x;
    }

    int getY() {
        return y;
    }

    int getWidth() {
        return height;
    }

    int getHeight() {
        return height;
    }




    string getType() const override {
        return "rectangle";
    }

    string getParameters() const override {
        return "x: " + to_string(x) + ", y: " + to_string(y) + ", width: " + to_string(width) + ", height: " + to_string(height);
    }
};

class Square : public Figure {
private:
    int x, y;
    int size;
public:
    Square(int x, int y, int size) : x(x), y(y), size(size) {}

    bool isWithinBoard() const override {
        return (x >= 0 && x + size < BOARD_WIDTH &&
            y >= 0 && y + size < BOARD_HEIGHT);
    }

    void draw(std::vector<std::vector<char>>& grid) const override {
        for (int j = x; j < x + size; ++j) {
            if (y >= 0 && y < BOARD_HEIGHT && j >= 0 && j < BOARD_WIDTH) {
                grid[y][j] = '*';
            }
        }
        for (int i = y; i <= y + size; ++i) {
            if (i >= 0 && i < BOARD_HEIGHT && x >= 0 && x < BOARD_WIDTH) {
                grid[i][x] = '*';
            }
        }

        for (int j = x; j < x + size; ++j) {
            if (y + size >= 0 && y + size < BOARD_HEIGHT && j >= 0 && j < BOARD_WIDTH) {
                grid[y + size][j] = '*';
            }
        }
        
        for (int i = y; i <= y + size; ++i) {
            if (i >= 0 && i < BOARD_HEIGHT && x + size >= 0 && x + size < BOARD_WIDTH) {
                grid[i][x + size] = '*';
            }
        }
    }

    void remove(std::vector<std::vector<char>>& grid) const override {
        for (int j = x; j < x + size; ++j) {
            if (y >= 0 && y < BOARD_HEIGHT && j >= 0 && j < BOARD_WIDTH) {
                grid[y][j] = ' ';
            }
        }
        for (int j = x; j < x + size; ++j) {
            if (y + size >= 0 && y + size < BOARD_HEIGHT && j >= 0 && j < BOARD_WIDTH) {
                grid[y + size][j] = ' ';
            }
        }
        for (int i = y; i <= y + size; ++i) {
            if (i >= 0 && i < BOARD_HEIGHT && x >= 0 && x < BOARD_WIDTH) {
                grid[i][x] = ' ';
            }
        }
        for (int i = y; i <= y + size; ++i) {
            if (i >= 0 && i < BOARD_HEIGHT && x + size >= 0 && x + size < BOARD_WIDTH) {
                grid[i][x + size] = ' ';
            }
        }
    }

    string getType() const override {
        return "square";
    }

    string getParameters() const override {
        return "x: " + to_string(x) + ", y: " + to_string(y) + ", size: " + to_string(size);
    }
};







class Circle : public Figure {
private:
    int x_center, y_center;
    int radius;

public:
    Circle(int x, int y, int r) : x_center(x), y_center(y), radius(r) {}

    bool isWithinBoard() const override {
        return (x_center - radius >= 0 && x_center + radius < BOARD_WIDTH &&
            y_center - radius >= 0 && y_center + radius < BOARD_HEIGHT);
    }
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

    void remove(std::vector<std::vector<char>>& grid) const override {
        int r_squared = radius * radius;
        for (int i = 0; i < BOARD_HEIGHT; ++i) {
            for (int j = 0; j < BOARD_WIDTH; ++j) {
                int dx = j - x_center;
                int dy = i - y_center;
                int dist_squared = dx * dx + dy * dy;
                if (abs(dist_squared - r_squared) <= 1) {
                    grid[i][j] = ' '; 
                }
            }
        }
    }
    int getX() {
        return x_center;
    }

    int getY() {
        return y_center;
    }

    int getRadius(){
        return radius;
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
    Board() : grid(BOARD_HEIGHT, vector<char>(BOARD_WIDTH, ' ')) {
    }

    void print() {

        cout << '-' << string(BOARD_WIDTH, '-') << '-' << "\n";


        for (auto& row : grid) {
            cout << '|';
            for (char c : row) {
                cout << c;
            }
            cout << '|' << "\n";
        }


        cout << '-' << string(BOARD_WIDTH, '-') << '-' << "\n";
    }


    void addFigure(Figure* figure) {
        
        if (!figure->isWithinBoard()) {
            cout << figure->getType() << " is too big for the board! It will not be added.\n";
            return;
        }

        figures.push_back(figure);
        figure->draw(grid);
        cout << figure->getType() << " added successfully.\n";
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
            cout << figure->getID() << "  " << figure->getType()
                << ", " << figure->getParameters() << '\n';
        }
    }

    void shapes() const {
        cout << "All available shapes and their parameters: \n";
        cout << "Example for triangle, x: 10, y: 12, height: 5\n";
        cout << "Example for circle, x_center: 40, y_center: 12, radius: 5\n";
        cout << "Example for rectangle, x: 15, y: 5, width: 10, height: 5\n";
    }


    void undo() {
        if (!figures.empty()) {
            figures.back()->remove(grid);

            delete figures.back();
            figures.pop_back();

            cout << "Undo: Last shape removed from the board.\n";
        } else {
            cout << "No figures to undo.\n";
        }
    }


    void save(const string& filepath)const {
        ofstream file(filepath);

        if (!file.is_open()) {
            cout << "Error opening";
            return;
        }

        file << figures.size() << '\n';

        for (auto& figure : figures) {
            if (figure->getType() == "circle") {
                Circle* circle = dynamic_cast<Circle*>(figure);  
                file << "circle " << circle->getX() << " " << circle->getY() << " " << circle->getRadius() << '\n';
            }
            else if (figure->getType() == "rectangle") {
                Rectangle* rect = dynamic_cast<Rectangle*>(figure);
                file << "rectangle " << rect->getX() << " " << rect->getY() << " " << rect->getWidth() << " " << rect->getHeight() << '\n';
            }
            else if (figure->getType() == "triangle") {
                Triangle* triangle = dynamic_cast<Triangle*>(figure);
                file << "triangle " << triangle->getX() << " " << triangle->getY() << " " << triangle->getHeight() << '\n';
            }


        }
        
        file.close();
        cout << "Board saved to " << filepath << "\n";

    }


    void load(const string& filepath) {
        ifstream file(filepath);
        if (!file.is_open()) {
            cout << "Error opening file.\n";
            return;
        }

        clear();
        for (auto& figure : figures) {
            delete figure;
        }
        figures.clear();

        size_t figureCount;
        file >> figureCount; 
        file.ignore();  

        for (size_t i = 0; i < figureCount; ++i) {
            string type;
            file >> type;

            
            cout << "Reading shape type: " << type << "\n";

            if (type == "circle") {
                int x, y, r;
                string ignore;

               
                getline(file, ignore, ':'); 
                file >> x;
                getline(file, ignore, ':'); 
                file >> y;
                getline(file, ignore, ':');  
                file >> r;

                Circle* circle = new Circle(x, y, r);
                addFigure(circle);
            }
            else if (type == "rectangle") {
                int x, y, width, height;
                string ignore;

                getline(file, ignore, ':');  
                file >> x;
                getline(file, ignore, ':');  
                file >> y;
                getline(file, ignore, ':');  
                file >> width;
                getline(file, ignore, ':');  
                file >> height;

                Rectangle* rect = new Rectangle(x, y, width, height);
                addFigure(rect);
            }
            else {
                cout << "Unknown shape type: " << type << "\n";
                return;
            }

            file.ignore();  
        }

        file.close();
        cout << "Board loaded from " << filepath << "\n";
    }

};


class Line : public Figure {

    int x_f, x_s, y_f, y_s;

    Line(int x_f, int x_s, int y_f, int y_s) : x_f(x_f), x_s(x_s), y_f(y_f), y_s(y_s) {
    }

};

    



int main() {
    vector<Figure*> figures;
    Board board;

    Circle* circle = new Circle(40, 12, 5);
    Rectangle* rect = new Rectangle(15, 5, 10, 6);
    Square* square = new Square(5, 5, 14);
    Circle* largeCircle = new Circle(40, 12, 50);  

    
    if (circle->isWithinBoard()) {
        figures.push_back(circle);  
    } else {
        cout << "Circle is too big for the board! It will not be added.\n";
    }

    if (rect->isWithinBoard()) {
        figures.push_back(rect); 
    } else {
        cout << "Rectangle is too big for the board! It will not be added.\n";
    }

    if (square->isWithinBoard()) {
        figures.push_back(square); 
    } else {
        cout << "Square is too big for the board! It will not be added.\n";
    }

    if (largeCircle->isWithinBoard()) {
        figures.push_back(square); 
    } else {
        cout << "Square is too big for the board! It will not be added.\n";
    }

  
    for (const auto& figure : figures) {
        board.addFigure(figure);
    }

    board.print();

    board.load("board.txt");
    board.print();


    for (Figure* figure : figures) {
        delete figure;
    }

    return 0;
}
