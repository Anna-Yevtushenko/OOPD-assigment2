#include <iostream>
#include <vector>
#include <string>
#include <fstream> 


using namespace std;
const int BOARD_WIDTH = 80;
const int BOARD_HEIGHT = 25;

enum Command {
    DRAW,
    LIST,
    SHAPES,
    ADD,
    UNDO,
    CLEAR,
    SAVE,
    LOAD,
    EXIT,
    INVALID
};


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
    virtual bool isEqual(const Figure& other) const = 0;
    virtual bool isLessThanBoard() const = 0;
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
    bool isLessThanBoard() const override {
        
        if ( height < BOARD_HEIGHT) {
            return true; 
        } else {
            cout << "The triangle's height exceeds the board's height.\n";
            return false; 
        }
    }
    bool isEqual(const Figure& other) const override {
        if (other.getType() != "triangle") return false;
        const Triangle& otherTriangle = dynamic_cast<const Triangle&>(other);
        return x == otherTriangle.x && y == otherTriangle.y && height == otherTriangle.height;
    }

    bool isWithinBoard() const override {
       
        if (y < 0 || y > BOARD_HEIGHT || x  < 0 || x > BOARD_WIDTH) {
            cout << "The triangle is out of board bounds.\n";
            return false;
        }
        return true;
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

    bool isLessThanBoard() const override {
        
        if (width > BOARD_WIDTH || height > BOARD_HEIGHT) {
            cout << "The rectangle exceeds the board's dimensions (width or height).\n";
            return false;
        }

        return true;
    }

    bool isEqual(const Figure& other) const override {
        if (other.getType() != "rectangle") return false;
        const Rectangle& otherRectangle = dynamic_cast<const Rectangle&>(other);
        return x == otherRectangle.x && y == otherRectangle.y && width == otherRectangle.width && height == otherRectangle.height;
    }

    bool isWithinBoard() const override {
        
        if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT) {
            cout << "The rectangle's starting point is out of board bounds.\n";
            return false;
        }
        return true;
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

    bool isLessThanBoard() const override {
        
        if (size > BOARD_HEIGHT) {
            cout << "The side of the square exceeds the board's height.\n";
            return false;
        }

        return true;
    }

    bool isEqual(const Figure& other) const override {
        if (other.getType() != "square") return false;
        const Square& otherSquare = dynamic_cast<const Square&>(other);
        return x == otherSquare.x && y == otherSquare.y && size == otherSquare.size;
    }


    bool isWithinBoard() const override {
        
        if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT) {
            cout << "The square's starting point is out of board bounds.\n";
            return false;
        }
        return true;
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

    int getX() {
        return x;
    }

    int getY() {
        return y;
    }

    int getSize() {
        return size;
    }
};





class Circle : public Figure {
private:
    int x_center, y_center;
    int radius;

public:
    Circle(int x, int y, int r) : x_center(x), y_center(y), radius(r) {}

    bool isLessThanBoard() const override {

        if (radius > BOARD_HEIGHT) {
            cout << "The radius of the circle exceeds the board's height.\n";
            return false;
        }
        return true;
    }

    bool isEqual(const Figure& other) const override {
        if (other.getType() != "circle") 
            return false;
        const Circle& otherCircle = dynamic_cast<const Circle&>(other);
        return x_center == otherCircle.x_center && y_center == otherCircle.y_center && radius == otherCircle.radius;
    }


    bool isWithinBoard() const override {
        if (x_center < 0 || x_center >= BOARD_WIDTH || y_center < 0 || y_center >= BOARD_HEIGHT) {
            cout << "The circle's center is out of board bounds.\n";
            return false;
        }
        return true;
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


        for (const auto& existingFigure : figures) {
            if (figure->isEqual(*existingFigure)) {
                cout << "This figure is already exist.\n";
                
                return;
            }
        }
        
        if (!figure->isLessThanBoard()) {
            cout << figure->getType() << " is too big for the board\n";
            return;
        }
        if (!figure->isWithinBoard()) {
            cout << figure->getType() << " is out of board bounds and cannot be added.\n";
            
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
        cout << "Example for square, x: 20, y: 10, size: 5\n";
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

        file << figures.size() << '\n';//number of figures on the board

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
            else if (figure->getType() == "square") {
                Square* square = dynamic_cast<Square*>(figure);
                file << "square " << square->getX() << " " << square->getY() << " " << square->getSize() << '\n';
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

            if (type.empty()) {
                continue;
            }

            if (type == "circle") {
                int x, y, r;
                file >> x >> y >> r;

                Circle* circle = new Circle(x, y, r);
                if (!circle->isLessThanBoard() || !circle->isWithinBoard()) {
                    cout << "Circle is out of bounds or too big and cannot be loaded.\n";
                    delete circle;
                } else {
                    addFigure(circle);
                }
            } else if (type == "rectangle") {
                int x, y, width, height;
                file >> x >> y >> width >> height;

                Rectangle* rect = new Rectangle(x, y, width, height);
                if (!rect->isLessThanBoard() || !rect->isWithinBoard()) {
                    cout << "Rectangle is out of bounds or too big and cannot be loaded.\n";
                    delete rect;
                } else {
                    addFigure(rect);
                }
            } else if (type == "square") {
                int x, y, size;
                file >> x >> y >> size;

                Square* square = new Square(x, y, size);
                if (!square->isLessThanBoard() || !square->isWithinBoard()) {
                    cout << "Square is out of bounds or too big and cannot be loaded.\n";
                    delete square;
                } else {
                    addFigure(square);
                }
            } else if (type == "triangle") {
                int x, y, height;
                file >> x >> y >> height;

                Triangle* triangle = new Triangle(x, y, height);
                if (!triangle->isLessThanBoard() || !triangle->isWithinBoard()) {
                    cout << "Triangle is out of bounds or too big and cannot be loaded.\n";
                    delete triangle;
                } else {
                    addFigure(triangle);
                }
            } else {
                cout << "Unknown shape type: " << type << "\n";
                return;
            }

            file.ignore();  
        }

        file.close();
        cout << "Board loaded from " << filepath << "\n";
    }


    void addShape() {
        string shapeType;
        cout << "Enter shape type (circle, rectangle, square, triangle): ";
        cin >> shapeType;

        if (shapeType == "circle") {  
            int x, y, r;
            cout << "Enter x_center, y_center, and radius: ";
            cin >> x >> y >> r;
            Circle* circle = new Circle(x, y, r);
            addFigure(circle);  
        } else if (shapeType == "rectangle") { 
            int x, y, width, height;
            cout << "Enter top-left x, y, width, and height for the rectangle: ";
            cin >> x >> y >> width >> height;
            Rectangle* rect = new Rectangle(x, y, width, height);
            addFigure(rect); 
        } else if (shapeType == "square") {  
            int x, y, size;
            cout << "Enter top-left x, y, and size for the square: ";
            cin >> x >> y >> size;
            Square* square = new Square(x, y, size);
            addFigure(square);  
        } else if (shapeType == "triangle") {  
            int x, y, height;
            cout << "Enter base x, y, and height for the triangle: ";
            cin >> x >> y >> height;
            Triangle* triangle = new Triangle(x, y, height);
            addFigure(triangle);  
        } else {
            cout << "Invalid shape type. Please enter circle, rectangle, square, or triangle.\n";
        }
    }
};




void showCommand() {
    cout << "Enter the command:\n";
    cout << "1. draw\n";
    cout << "2. list\n";
    cout << "3. shapes\n";
    cout << "4. add\n";
    cout << "5. undo\n";
    cout << "6. clear\n";
    cout << "7. save <file>\n";
    cout << "8. load <file>\n";
    cout << "9. exit\n";
}


Command getCommandFromInput(const string& input) {
    if (input == "draw") return DRAW;
    if (input == "list") return LIST;
    if (input == "shapes") return SHAPES;
    if (input == "add") return ADD;
    if (input == "undo") return UNDO;
    if (input == "clear") return CLEAR;
    if (input == "save") return SAVE;
    if (input == "load") return LOAD;
    if (input == "exit") return EXIT;
    return INVALID;
}



void run(Board& board) {
    string input;
    while (true) {
        cout << "> ";
        cin >> input;
        Command cmd = getCommandFromInput(input);

        switch (cmd) {
        case DRAW:
            board.print();
            break;
        case LIST:
            board.list();
            break;
        case SHAPES:
            board.shapes();
            break;
        case ADD:
            board.addShape();
            break;
        case UNDO:
            board.undo();
            break;
        case CLEAR:
            board.clear();
            break;
        case SAVE: {
                string filepath;
                cin >> filepath;
                board.save(filepath);
                break;
        }
        case LOAD: {
            string filepath;
            cin >> filepath;
            board.load(filepath);
            break;
        }
        case EXIT:
            return; 
        case INVALID:
        default:
            cout << "Invalid command. Please try again.\n";
            showCommand();
            break;
        }
    }
}



int main() {
    Board board;
    string input;
    showCommand();
    run(board);
   

    return 0;
}
