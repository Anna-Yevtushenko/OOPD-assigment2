#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

//#include <SFML/Graphics.hpp>

using namespace std;
const int BOARD_WIDTH = 80;
const int BOARD_HEIGHT = 25;

class Point {
    char sign;
    std::string color;

public:
    Point(char sign = ' ', std::string color = "white") {
        this->sign = sign;
        this->color = color;
    }

    char getSign() const {
        return sign;
    }

    std::string getColor() const {
        return color;
    }

    void setSign(char sign) {
        this->sign = sign;
    }

    void setColor(std::string color) {
        this->color = color;
    }
};


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
    SELECT,
    REMOVE,
    PAINT,
    EDIT,
    MOVE,
    
    INVALID
};


void setColor(const string& color) {
    if (color == "red") {
        cout << "\033[31m";  
    } else if (color == "green") {
        cout << "\033[32m"; 
    } else if (color == "blue") {
        cout << "\033[34m"; 
    } else if (color == "yellow") {
        cout << "\033[33m";  
    } else {
        cout << "\033[0m";   
    }
}


void resetColor() {
    cout << "\033[0m";  
}


class Figure {
protected:
    string color;   
    bool isFilled; 
    static int nextID;
    int id;

public:
    Figure(string color = "white", bool isFilled = true)
        : id(nextID++), color(color), isFilled(isFilled) {}
   
    virtual void draw(std::vector<std::vector<Point>>& grid) const = 0;
    virtual string getType() const = 0;
    virtual string getParameters() const = 0;

    virtual void remove(std::vector<std::vector<Point>>& grid) const = 0;
    virtual bool isEqual(const Figure& other) const = 0;
    virtual bool isLessThanBoard() const = 0;
    virtual bool isWithinBoard() const = 0;
    virtual bool isInside(int x, int y) const = 0;
    virtual void edit(const std::vector<int>& param) = 0; 
    virtual void move(int newX, int newY) = 0; 

  

    int getID() const {
        return id;
    }

    
    string getColor() const {  
        return color;
    }

    
    bool isFigureFilled() const {  
        return isFilled;
    }

    void paint(const string& newColor) {
        if (newColor == "red" || newColor == "green" || newColor == "blue" || newColor == "yellow") {
            color = newColor;
            cout << "Figure ID " << id << " color changed to " << newColor << ".\n";
        } else {
            cout << "Error: Invalid color. Allowed colors are red, green, blue, or yellow.\n";
        }
    }

    virtual ~Figure() = default;
};

int Figure::nextID = 1;  


class Triangle : public Figure {
private:
    int x, y;
    int height;


public:
    
    Triangle(int x, int y, int height, string color, bool isFilled)
        : Figure(color, isFilled), x(x), y(y), height(height) {}



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


    void draw(std::vector<std::vector<Point>>& grid) const override {
        if (isFilled) {
            
            for (int i = 0; i < height; ++i) {
                for (int j = x - i; j <= x + i; ++j) {
                    int posY = y + i;
                    if (posY >= 0 && posY < BOARD_HEIGHT && j >= 0 && j < BOARD_WIDTH) {
                        grid[posY][j] = '*';  
                        grid[posY][j].setColor(color);
                    }
                }
            }
        }
        
        else {
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
    }



    void remove(std::vector<std::vector<Point>>& grid) const override {
        if (isFilled) {
            
            for (int i = 0; i < height; ++i) {
                for (int j = x - i; j <= x + i; ++j) {
                    int posY = y + i;
                    if (posY >= 0 && posY < BOARD_HEIGHT && j >= 0 && j < BOARD_WIDTH) {
                        grid[posY][j] = ' ';  
                        grid[i][j].setColor("white");
                    }
                }
            }
        } else {
          
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
    }

    bool isInside(int x, int y) const override {
        if (y < this->y || y >= this->y + height) {
            return false;
        }
        int halfBase = y - this->y;
        if (isFilled) {
            return (x >= this->x - halfBase && x <= this->x + halfBase); // Inside the filled triangle
        } else {
            return (x == this->x - halfBase || x == this->x + halfBase || y == this->y + height - 1); // On the frame of the triangle
        }
    }


    void edit(const vector<int>& param) override {
        if (param.size() == 1) {
            int newHeight = param[0];
            if (newHeight > 0 && newHeight <= BOARD_HEIGHT) {
                height = newHeight;
            }
            else {
                cout << "Invalid height value: too small or too big";
            }
        }
        else {
            cout << "Incorrect argument count , enter new  height of a triangle ";
        }

    }

    void move(int newX, int newY) override {
        x = newX;
        y = newY;
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
        return "x: " + to_string(x) + ", y: " + to_string(y) + ", height: " + to_string(height) +  ", color: " + color + ", type: " + (isFilled ? "fill" : "frame");
    }
};





class Rectangle : public Figure {
private:
    int x, y;
    int width;
    int height;

public:
  
    Rectangle(int x, int y, int width, int height, string color, bool isFilled) 
        : Figure(color, isFilled), x(x), y(y), width(width), height(height) {}

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


    void draw(std::vector<std::vector<Point>>& grid) const override {
        if (isFilled) {
            for (int i = y; i <= y + height; ++i) {
                for (int j = x; j < x + width; ++j) {
                    if (i >= 0 && i < BOARD_HEIGHT && j >= 0 && j < BOARD_WIDTH) {
                        grid[i][j].setSign('*');
                        grid[i][j].setColor(color);
                    }
                }
            }
        } else {
            for (int j = x; j < x + width; ++j) {
                if (y >= 0 && y < BOARD_HEIGHT && j >= 0 && j < BOARD_WIDTH) {
                    grid[y][j].setSign('*');
                    grid[y][j].setColor(color);
                }
                if (y + height >= 0 && y + height < BOARD_HEIGHT && j >= 0 && j < BOARD_WIDTH) {
                    grid[y + height][j].setSign('*');
                    grid[y + height][j].setColor(color);
                }
            }
            for (int i = y; i <= y + height; ++i) {
                if (i >= 0 && i < BOARD_HEIGHT && x >= 0 && x < BOARD_WIDTH) {
                    grid[i][x].setSign('*');
                    grid[i][x].setColor(color);
                }
                if (i >= 0 && i < BOARD_HEIGHT && x + width >= 0 && x + width < BOARD_WIDTH) {
                    grid[i][x + width].setSign('*');
                    grid[i][x + width].setColor(color);
                }
            }
        }

        std::cout << "Rectangle drawn with color: " << getColor() << " and "
            << (isFilled ? "filled" : "frame") << " mode.\n";
    }

    void remove(std::vector<std::vector<Point>>& grid) const override {
        if (isFilled) {
           
            for (int i = y; i <= y + height; ++i) {
                for (int j = x; j < x + width; ++j) {
                    if (i >= 0 && i < BOARD_HEIGHT && j >= 0 && j < BOARD_WIDTH) {
                        grid[i][j] = ' '; 
                        grid[i][j].setColor("white");
                    }
                }
            }
        } else {
           
            for (int j = x; j < x + width; ++j) {
                if (y >= 0 && y < BOARD_HEIGHT && j >= 0 && j < BOARD_WIDTH) {
                    grid[y][j] = ' ';  
                }
                if (y + height >= 0 && y + height < BOARD_HEIGHT && j >= 0 && j < BOARD_WIDTH) {
                    grid[y + height][j] = ' ';  
                    
                }
            }

            for (int i = y; i <= y + height; ++i) {
                if (i >= 0 && i < BOARD_HEIGHT && x >= 0 && x < BOARD_WIDTH) {
                    grid[i][x] = ' ';  
                }
                if (i >= 0 && i < BOARD_HEIGHT && x + width >= 0 && x + width < BOARD_WIDTH) {
                    grid[i][x + width] = ' ';  
                   
                    
                }
            }
        }
    }

    
    bool isInside(int x, int y) const override {
        if (isFilled) {
            return (x >= this->x && x <= this->x + width && y >= this->y && y <= this->y + height);
        } else {
            return ((x == this->x || x == this->x + width) && (y >= this->y && y <= this->y + height)) ||
                ((y == this->y || y == this->y + height) && (x >= this->x && x <= this->x + width));
        }
    }

    void edit(const vector<int>& param)override {
        if (param.size() == 2) {
            int newWidth = param[0];
            int newHeight = param[1];
            if (newWidth > 0 && newWidth <= BOARD_WIDTH && newHeight>0 && newHeight <= BOARD_HEIGHT) {
                width = newWidth;
                height = newHeight;
            }
            else {
                cout << "Invalid width or height values: too small or too big";
            }
        }
        else {
            cout << "Incorrect argument count , enter new width and height";
        }

    }

    void move(int newX, int newY) override {
        x = newX;
        y = newY;
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
        return "x: " + to_string(x) + ", y: " + to_string(y) + ", width: " + to_string(width) + ", height: " + to_string(height) +  ", color: " + color + ", type: " + (isFilled ? "fill" : "frame");
    }

};





class Square : public Figure {
private:
    int x, y;
    int size;
public:

    Square(int x, int y, int size, string color, bool isFilled) 
        : Figure(color, isFilled), x(x), y(y), size(size) {}

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


    void draw(std::vector<std::vector<Point>>& grid) const override {
        if (isFilled) {
            for (int i = y; i <= y + size; ++i) {
                for (int j = x; j < x + size; ++j) {
                    if (i >= 0 && i < BOARD_HEIGHT && j >= 0 && j < BOARD_WIDTH) {
                        grid[i][j].setSign('*');
                        grid[i][j].setColor(color);
                    }
                }
            }
        }
        else {
            for (int j = x; j < x + size; ++j) {
                if (y >= 0 && y < BOARD_HEIGHT && j >= 0 && j < BOARD_WIDTH) {
                    grid[y][j].setSign('*');
                    grid[y][j].setColor(color);
                }
                if (y + size >= 0 && y + size < BOARD_HEIGHT && j >= 0 && j < BOARD_WIDTH) {
                    grid[y + size][j].setSign('*');
                    grid[y + size][j].setColor(color);
                }
            }

            for (int i = y; i <= y + size; ++i) {
                if (i >= 0 && i < BOARD_HEIGHT && x >= 0 && x < BOARD_WIDTH) {
                    grid[i][x].setSign('*');
                    grid[i][x].setColor(color);
                }
                if (i >= 0 && i < BOARD_HEIGHT && x + size >= 0 && x + size < BOARD_WIDTH) {
                    grid[i][x + size].setSign('*');
                    grid[i][x + size].setColor(color);
                }
            }
        }
    }


    void remove(std::vector<std::vector<Point>>& grid) const override {
        if (isFilled) {
            for (int i = y; i <= y + size; ++i) {
                for (int j = x; j < x + size; ++j) {
                    if (i >= 0 && i < BOARD_HEIGHT && j >= 0 && j < BOARD_WIDTH) {
                        grid[i][j].setSign(' ');
                        grid[i][j].setColor("white");
                    }
                }
            }
        } else {
            for (int j = x; j < x + size; ++j) {
                if (y >= 0 && y < BOARD_HEIGHT && j >= 0 && j < BOARD_WIDTH) {
                    grid[y][j].setSign(' ');
                }
                if (y + size >= 0 && y + size < BOARD_HEIGHT && j >= 0 && j < BOARD_WIDTH) {
                    grid[y + size][j].setSign(' ');
                }
            }

            for (int i = y; i <= y + size; ++i) {
                if (i >= 0 && i < BOARD_HEIGHT && x >= 0 && x < BOARD_WIDTH) {
                    grid[i][x].setSign(' ');
                }
                if (i >= 0 && i < BOARD_HEIGHT && x + size >= 0 && x + size < BOARD_WIDTH) {
                    grid[i][x + size].setSign(' ');
                }
            }
        }
    }


    bool isInside(int x, int y) const override {
        if (isFilled) {
            return (x >= this->x && x <= this->x + size && y >= this->y && y <= this->y + size);
        } else {
            return ((x == this->x || x == this->x + size) && (y >= this->y && y <= this->y + size)) ||
                ((y == this->y || y == this->y + size) && (x >= this->x && x <= this->x + size));
        }
    }

    void edit(const vector<int>& param) override  {
        if (param.size() == 1) {
            int newSide = param[0];
            if (newSide > 0 && newSide <= BOARD_HEIGHT) {
                size = newSide;
            }
            else {
                cout << "Invalid size value: too small or too big";
            }
        }
        else {
            cout << "Incorrect argument count , enter new  side of a square ";
        }

    }

    void move(int newX, int newY) override {
        x = newX;
        y = newY;
    }


    string getType() const override {
        return "square";
    }

    string getParameters() const override {
        return "x: " + to_string(x) + ", y: " + to_string(y) + ", size: " + to_string(size) + ", color: " + color + ", type: " + (isFilled ? "fill" : "frame");
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

    Circle(int x, int y, int r, string color, bool isFilled)
        : Figure(color, isFilled), x_center(x), y_center(y), radius(r) {}
       

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

    void draw(std::vector<std::vector<Point>>& grid) const override {
       /* cout << "\nwe have BEFORE color in Circle.draw : " << color << "\n";*/
        setColor(color); 
       /* cout << "\nwe have AFTER color in Circle.draw : " << color << "\n";*/

        int r_squared = radius * radius;
        if (isFilled) {
            for (int i = 0; i < BOARD_HEIGHT; ++i) {
                for (int j = 0; j < BOARD_WIDTH; ++j) {
                    int dx = j - x_center;
                    int dy = i - y_center;
                    int dist_squared = dx * dx + dy * dy;
                    if (dist_squared <= r_squared) {
                        grid[i][j] = '*'; 
                        grid[i][j].setColor(color);
                    }
                }
            }
        } else {
            for (int i = 0; i < BOARD_HEIGHT; ++i) {
                for (int j = 0; j < BOARD_WIDTH; ++j) {
                    int dx = j - x_center;
                    int dy = i - y_center;
                    int dist_squared = dx * dx + dy * dy;
                    if (abs(dist_squared - r_squared) <= 1) {
                        grid[i][j] = '*'; 
                        grid[i][j].setColor(color);
                    }
                }
            }
        }

       resetColor(); 
    }


    

    void remove(std::vector<std::vector<Point>>& grid) const override {
        int r_squared = radius * radius;
        for (int i = 0; i < BOARD_HEIGHT; ++i) {
            for (int j = 0; j < BOARD_WIDTH; ++j) {
                int dx = j - x_center;
                int dy = i - y_center;
                int dist_squared = dx * dx + dy * dy;
                if (abs(dist_squared - r_squared) <= 1 || (isFilled && dist_squared <= r_squared)) {
                    grid[i][j] = ' ';  
                    grid[i][j].setColor("white");
                }
            }
        }
    }


    bool isInside(int x, int y) const override {
        int dx = x - x_center;
        int dy = y - y_center;
        int dist_squared = dx * dx + dy * dy;
        if (isFilled) {
            return dist_squared <= radius * radius; // Inside the filled circle
        } else {
            return abs(dist_squared - radius * radius) <= 1; // On the frame of the circle
        }
    }

    void edit(const vector<int>& param) override {
        if (param.size() == 1) {
            int newRadius = param[0];
            if (newRadius > 0 && newRadius <= BOARD_HEIGHT) {
                radius = newRadius;
            }
            else {
                cout << "Invalid radius value: too small or too big";
            }
        }
        else {
            cout << "Incorrect argument count , enter new radius";
        }
            
    }

    void move(int newX, int newY) override {
        x_center = newX;
        y_center = newY;
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
        return "x_center: " + to_string(x_center) + ", y_center: " + to_string(y_center) + ", radius: " + to_string(radius) + ", color: " + color + ", type: " + (isFilled ? "fill" : "frame");
    }
};



class Board {
private:
    vector<std::vector<Point>> grid;
    vector<Figure*> figures;

public:
    Board() : grid(BOARD_HEIGHT, vector<Point>(BOARD_WIDTH, Point(' ', "white"))) {}

    vector<std::vector<Point>>& getGrid() {
        return grid;
    }

    void print() {
        cout << '-' << string(BOARD_WIDTH, '-') << '-' << "\n";
        for (auto& row : grid) {
            cout << '|';
            for (auto& point : row) {
                setColor(point.getColor());
                cout << point.getSign();
            }
            resetColor();
            cout << "|\n";
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
        cout << "Example for triangle, type: fill, color: red, x: 10, y: 12, height: 5\n";
        cout << "Example for circle, type: frame, color: blue, x_center: 40, y_center: 12, radius: 5\n";
        cout << "Example for rectangle, type: fill, color: green, x: 15, y: 5, width: 10, height: 5\n";
        cout << "Example for square, type: frame, color: yellow, x: 20, y: 10, size: 5\n";
    }


    Figure* select(int id) const {
        for (const auto& figure : figures) {
            if (figure->getID() == id) {
                cout << "Figure found by ID:" << "\n";
                return figure;
            }
        }
        cout << "No figure found with the given ID.\n";
        return nullptr;
    }


    Figure* select(int x, int y) const {
        bool found = false;  

        for (const auto& figure : figures) {
            if (figure->isInside(x, y)) {
                cout << "Figure found at coordinates (" << x << ", " << y << "): \n";
                return figure;
            }
        }
        return nullptr;

      
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
                file << "circle " << circle->getX() << " " << circle->getY() << " " << circle->getRadius() 
                    << " " << circle->getColor() << " ";

                if (circle->isFigureFilled()) {
                    file << "fill\n";
                } else {
                    file << "frame\n";
                }
            } 


            else if (figure->getType() == "rectangle") {
                Rectangle* rect = dynamic_cast<Rectangle*>(figure);
                file << "rectangle " << rect->getX() << " " << rect->getY() << " " << rect->getWidth() << " " << rect->getHeight() 
                    << " " << rect->getColor() << " ";

                if (rect->isFigureFilled()) {
                    file << "fill\n";
                } else {
                    file << "frame\n";
                }
            }


            else if (figure->getType() == "triangle") {
                Triangle* triangle = dynamic_cast<Triangle*>(figure);
                file << "triangle " << triangle->getX() << " " << triangle->getY() << " " << triangle->getHeight() 
                    << " " << triangle->getColor() << " ";

                if (triangle->isFigureFilled()) {
                    file << "fill\n";
                } else {
                    file << "frame\n";
                }
            } 

            else if (figure->getType() == "square") {
                Square* square = dynamic_cast<Square*>(figure);
                file << "square " << square->getX() << " " << square->getY() << " " << square->getSize() 
                    << " " << square->getColor() << " ";

                if (square->isFigureFilled()) {
                    file << "fill\n";
                } else {
                    file << "frame\n";
                }
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
                std::string colorName, fillType;
                file >> x >> y >> r >> colorName >> fillType;


                bool isFilled = (fillType == "fill");

                Circle* circle = new Circle(x, y, r, colorName, isFilled);
                if (!circle->isLessThanBoard() || !circle->isWithinBoard()) {
                    cout << "Circle is out of bounds or too big and cannot be loaded.\n";
                    delete circle;
                } else {
                    addFigure(circle);
                }


            } else if (type == "rectangle") {
                int x, y, width, height;
                std::string colorName, fillType;
                file >> x >> y >> width >> height >> colorName >> fillType;

                
                bool isFilled = (fillType == "fill");

                Rectangle* rect = new Rectangle(x, y, width, height, colorName, isFilled);
                if (!rect->isLessThanBoard() || !rect->isWithinBoard()) {
                    cout << "Rectangle is out of bounds or too big and cannot be loaded.\n";
                    delete rect;
                } else {
                    addFigure(rect);
                }


            } else if (type == "square") {
                int x, y, size;
                std::string colorName, fillType;
                file >> x >> y >> size >> colorName >> fillType;

                bool isFilled = (fillType == "fill");

                Square* square = new Square(x, y, size, colorName, isFilled);
                if (!square->isLessThanBoard() || !square->isWithinBoard()) {
                    cout << "Square is out of bounds or too big and cannot be loaded.\n";
                    delete square;
                } else {
                    addFigure(square);
                }


            } else if (type == "triangle") {
                int x, y, height;
                std::string colorName, fillType;
                file >> x >> y >> height >> colorName >> fillType;

                bool isFilled = (fillType == "fill");

                Triangle* triangle = new Triangle(x, y, height, colorName, isFilled);
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


    void addShape(const string& input) {
        stringstream ss(input);
        string fillType, colorName, shapeType;

        
        if (!(ss >> fillType >> colorName >> shapeType)) {
            cout << "Error: Invalid input format. Please enter fill/frame, color, and shape type.\n";
            return;
        }

        
        bool isFilled;
        if (fillType == "fill") {
            isFilled = true;
        } else if (fillType == "frame") {
            isFilled = false;
        } else {
            cout << "Error: Invalid fill type. Use 'fill' or 'frame'.\n";
            return;
        }

        if (colorName != "red" && colorName != "green" && colorName != "blue" && colorName != "yellow") {
            cout << "Error: Invalid color. Allowed colors are red, green, blue, or yellow.\n";
            return;
        }

        
        if (shapeType == "circle") {
            int x, y, radius;
            if (ss >> x >> y >> radius) {
                Circle* circle = new Circle(x, y, radius, colorName, isFilled);
                addFigure(circle);
                
            } else {
                cout << "Error: Invalid parameters for circle. Expected format: 'add fill red circle x y radius'.\n";
            }
        } else if (shapeType == "rectangle") {
            int x, y, width, height;
            if (ss >> x >> y >> width >> height) {
                Rectangle* rect = new Rectangle(x, y, width, height, colorName, isFilled);
                addFigure(rect);
                
            } else {
                cout << "Error: Invalid parameters for rectangle. Expected format: 'add fill red rectangle x y width height'.\n";
            }
        } else if (shapeType == "square") {
            int x, y, size;
            if (ss >> x >> y >> size) {
                Square* square = new Square(x, y, size, colorName, isFilled);
                addFigure(square);
               
            } else {
                cout << "Error: Invalid parameters for square. Expected format: 'add fill red square x y size'.\n";
            }
        } else if (shapeType == "triangle") {
            int x, y, height;
            if (ss >> x >> y >> height) {
                Triangle* triangle = new Triangle(x, y, height, colorName, isFilled);
                addFigure(triangle);
               
            } else {
                cout << "Error: Invalid parameters for triangle. Expected format: 'add fill red triangle x y height'.\n";
            }
        } else {
            cout << "Invalid shape type. Please enter circle, rectangle, square, or triangle.\n";
        }
    }


    void removeShape(int id) {
        bool found = false;

        
        for (auto figure = figures.begin(); figure != figures.end(); ++figure) {
            if ((*figure)->getID() == id) {
                
                (*figure)->remove(grid);
                
                cout << "< " << (*figure)->getID() << " " << (*figure)->getType() << " removed\n";
                
                delete *figure;
                figures.erase(figure);
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "Error: No figure found with ID " << id << "\n";
        }
    }


    void moveFigure(Figure* figure, int newX, int newY) {
        if (figure != nullptr) {
            
            figure->remove(grid);

            
            figure->move(newX, newY);

          
            figure->draw(grid);

           
            cout << "< " << figure->getID() << " " << figure->getType() << " moved to (" << newX << ", " << newY << ").\n";
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
    cout << "9. select <file>\n";
    cout << "10. paint\n";
    cout << "11. edit\n";
    cout << "13. exit\n";
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
    if (input == "select") return SELECT;
    if (input == "remove") return REMOVE;
    if (input == "exit") return EXIT;
    if (input == "paint") return PAINT;
    if (input == "edit") return EDIT;

    return INVALID;
}



void run(Board& board) {
    string input;
    Figure* selectedFigure = nullptr; 
    while (true) {
        cout << "> ";
        getline(cin, input);  

        stringstream ss(input);
        string command;
        ss >> command;  

        if (command == "draw") {
            board.print();
        } 
        else if (command == "list") {
            board.list();
        } 
        else if (command == "shapes") {
            board.shapes();
        } 
        else if (command == "add") {
            string params;
            getline(ss, params); 
            board.addShape(params);  
        } 
        else if (command == "undo") {
            board.undo();
        } 
        else if (command == "clear") {
            board.clear();
        } 
        else if (command == "save") {
            string filepath;
            ss >> filepath;
            if (!filepath.empty()) {
                board.save(filepath);
            } else {
                cout << "Error: No file path provided.\n";
            }
        } 
        else if (command == "load") {
            string filepath;
            ss >> filepath;
            if (!filepath.empty()) {
                board.load(filepath);
            } else {
                cout << "Error: No file path provided.\n";
            }
        } 
        else if (command == "select") {
            int first, second;
            if (ss >> first) {
                if (ss >> second) {
                    selectedFigure = board.select(first, second);  
                    if (selectedFigure != nullptr) {
                        cout << "< " << selectedFigure->getType() << " [" << selectedFigure->getParameters() << "]\n";
                    } else {
                        cout << "Error: No figure found at coordinates (" << first << ", " << second << ")\n";
                    }
                } else {
                   
                    selectedFigure = board.select(first);  
                    if (selectedFigure != nullptr) {
                        cout << "< " << selectedFigure->getType() << " [" << selectedFigure->getParameters() << "]\n";
                    } else {
                        cout << "Error: No figure found with ID " << first << "\n";
                    }
                }
            } else {
                cout << "Error: Invalid input for select command.\n";
            }
        } 
        else if (command == "exit") {
            return; 
        } 

        else if (command == "paint") {
            string newColor;
            if (selectedFigure != nullptr) {
                ss >> newColor;
                selectedFigure->remove(board.getGrid());
                
                selectedFigure->paint(newColor);
                selectedFigure->draw(board.getGrid());

                cout << "< " << selectedFigure->getID() << " " << selectedFigure->getType() 
                    << " [" << selectedFigure->getParameters() << "] color changed to " << newColor << ".\n";
            } else {
                cout << "Error: No figure selected. Use 'select' first.\n";
            }
        }

        else if (command == "edit") {
            if (selectedFigure != nullptr) {
                vector<int> params;
                int param;
                
                while (ss >> param) {
                    params.push_back(param);
                }
                selectedFigure->remove(board.getGrid());
                selectedFigure->edit(params);
                selectedFigure->draw(board.getGrid());
                cout << "Figure was updated ";
            }

            else {
                cout << "Error: No figure selected. Use 'select' first.\n";
            }
        }


        else if (command == "remove") {
            if (selectedFigure != nullptr) {
                board.removeShape(selectedFigure->getID());  
                selectedFigure = nullptr; 
            } else {
                cout << "Error: No figure selected. Use 'select' first.\n";
            }
        }

        else if (command == "move") {
            if (selectedFigure != nullptr) {
                int newX, newY;
                if (ss >> newX >> newY) {
                    
                    selectedFigure->remove(board.getGrid());
                    board.moveFigure(selectedFigure, newX, newY);
                    selectedFigure->draw(board.getGrid());
                } else {
                    cout << "Error: Invalid coordinates.\n";
                }
            } else {
                cout << "Error: No figure selected. Use 'select' first.\n";
            }
            }
        else {
            cout << "Invalid command. Please try again.\n";
        }

    }
}





int main() {


    cout << "This should be blue text\n";
    resetColor();
    Board board;
    string input;
    showCommand();
    run(board);
   

    return 0;
}


