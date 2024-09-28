#include <iostream>
#include <vector>
#include <string>


using namespace std;
const int BOARD_WIDTH = 80;
const int BOARD_HEIGHT = 25;


class Figure {
public:
	virtual void draw(std::vector<std::vector<char>>& grid) const = 0;
	virtual string getID()const = 0;
	virtual string getParameters() const = 0;
	virtual ~Figure() = default; 
};



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

			// draw the sides of the triangle
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
	string getID() const override{
		return "triagle";
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
	Rectangle(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {
	}

	void draw(std::vector<std::vector<char>>& grid) const override {

		for (int j = x; j < x + width; ++j) {
			if (y >= 0 && y < BOARD_HEIGHT && j >= 0 && j < BOARD_WIDTH){
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
			if (i >= 0 && i < BOARD_HEIGHT && x >= 0 && x < BOARD_WIDTH) {
				grid[i][x + width] = '*';
			}
		}

		

		
	}
	
	string getID() const override{
		return "triagle";
	}

	string getParameters() const override {
		return "x: " + to_string(x) + ", y: " + to_string(y) + ", height: " + to_string(height);
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

	string getID() const override {
		return "circle";
	}

	string getParameters() const override {
		return "x: " + to_string(x_center) + ", y: " + to_string(y_center) + ", radius: " + to_string(radius);
	}
};





class Board {
private:
	std::vector<std::vector<char>> grid;
public:
	Board() : grid(BOARD_HEIGHT, std::vector<char>(BOARD_WIDTH, ' ')) {}

	void print() {
		for (auto& row : grid) {//each row of the grid
			for (char c : row) {//each string character
				std::cout << c;
			}
			std::cout << "\n";
		}
	}

	void addFigure(const Figure& figure) {

		figure.draw(grid);
	}

	void clear() {
		for (auto& row : grid) {
			std::fill(row.begin(), row.end(), ' ');
		}
	}
	

};




int main() {
	Board board;
	//Triangle triangle(7, 24, 9);
	//Rectangle rectangle(6, 6, 3, 3);
	//Triangle triangle2(5, 0, 5);
	Circle circle(40, 12, 5);
	//board.addFigure(triangle);
	board.addFigure(circle);
	//board.addFigure(triangle2);
	//board.addFigure(rectangle);

	board.print();
	/*board.clear();
	board.print();*/

	return 0;
}