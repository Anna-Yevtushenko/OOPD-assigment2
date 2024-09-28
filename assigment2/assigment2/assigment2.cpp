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
	Triangle triangle(7, 24, 9);
	Triangle triangle2(5, 0, 5);
	board.addFigure(triangle);
	board.addFigure(triangle2);
	board.print();

	return 0;
}