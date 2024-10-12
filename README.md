# Shapes blackboard - assignment2

## Task 🛠️

Develop a console-based application which will represent a shapes board with the ability to draw
different types of shapes on it. The main idea is that the user has an empty blackboard and can
interact with it in a few ways: show the blackboard, place or remove some shape within the
blackboard or clear it completely. Additionally, the user should be able to save and load the state
of the blackboard from the file

---


## Command-line Interface:

| Command  | Parameters      | Description                              | Result                                                    | Example              |
|----------|-----------------|------------------------------------------|-----------------------------------------------------------|----------------------|
| draw     | -               | Draw blackboard to the console            | ASCII-drawn blackboard with all shapes on it, or empty     | `> draw`             |
| list     | -               | Print all added shapes with their IDs and parameters | List of all added shapes. One shape per line with shape ID and all information related to the shape | `> list` <br> `> id circle radius coordinates` <br> `> id square width height coordinates` |
| shapes   | -               | Print a list of all available shapes and parameters for the `add` command | List of all available shapes. One shape per line with shape info as defined by the student. | `> shapes` <br> `> circle radius coordinates` <br> `> square width height coordinates` |
| add      | shape parameters| Add shape to the blackboard               | Shape is added to the blackboard                           | `> add circle 10 10 5` |
| undo     | -               | Remove the last added shape from the blackboard | The last added shape is removed                          | `> undo`             |
| clear    | -               | Remove all shapes from the blackboard     | All shapes are removed from the blackboard                | `> clear`            |
| save     | file-path       | Save the blackboard to the file           | Blackboard is saved to the file. The state of the blackboard remains unchanged. | `> save D:\test.bb`  |
| load     | file-path       | Load a blackboard from the file           | Blackboard is loaded from the file. Previous state of the blackboard is cleared. | `> load D:\test.bb`  |






## Constrains: 
There are some constraints and preferences related to that task.
1. Shapes are drawn as frames.
2. Shapes can overlap. In that case, shapes should be correctly drawn.
3. Shapes cannot be placed outside of the board.
4. Shapes bigger than the board cannot be placed on the board.
5. Shapes that are partially outside of the board can be placed, but they are cropped.
6. Figures with the same type and parameters cannot be placed on the same spot.
7. There is no limit to the shape count on the board.
8. Empty boards should also be correctly saved and loaded.
9. A file with an invalid board should not be loadable.
10. There must be shapes with different parameter count (radius, width\height, height\angle).


![image](https://github.com/user-attachments/assets/5c6b7507-d3a1-451f-83d2-a9fdfb7c89a7)


<details>
<summary>Code example🖥️ </summary>

---


   ```sh
  #include <iostream>
#include <vector>
// Define the size of the board
const int BOARD_WIDTH = 80;
const int BOARD_HEIGHT = 25;
// Struct to define the board
struct Board {
 std::vector<std::vector<char>> grid;
 Board() : grid(BOARD_HEIGHT, std::vector<char>(BOARD_WIDTH, ' ')) {}
 void print() {
 for (auto& row : grid) {
 for (char c : row) {
 std::cout << c;
 }
 std::cout << "\n";
 }
 }
 void drawTriangle(int x, int y, int height) {
 if (height <= 0) return; // Ensure the triangle height is positive and sensible
 for (int i = 0; i < height; ++i) {
 int leftMost = x - i; // Calculate the starting position
 int rightMost = x + i; // Calculate the ending position
 int posY = y + i; // Calculate the vertical position
 // Draw only the edges/border of the triangle
 if (posY < BOARD_HEIGHT) {
 if (leftMost >= 0 && leftMost < BOARD_WIDTH) // Check bounds for left
most position
 grid[posY][leftMost] = '*';
 if (rightMost >= 0 && rightMost < BOARD_WIDTH && leftMost != rightMost)
// Check bounds for right most position
 grid[posY][rightMost] = '*';
 }
 }
 // Draw the base of the triangle separately
 for (int j = 0; j < 2 * height - 1; ++j) {
 int baseX = x - height + 1 + j;
 int baseY = y + height - 1;
 if (baseX >= 0 && baseX < BOARD_WIDTH && baseY < BOARD_HEIGHT) // Check
bounds for each position on the base
 grid[baseY][baseX] = '*';
 }
 }
};
int main() {
 Board board;
 board.drawTriangle(10, 1, 5);
 board.print();
 return 0;
}
```

---

</details>



## Task milestones 
1. Design your  system on  the paper or by  using some online tools, please use squares and 
arrows to draw classes and their relations, no specific notation is needed in this task, think 
in advance how would your system look like. 
2. Implement the I/O system to parse input file, create a class for user input processing 
3. Implement logic to book the ticket, create appropriate classes and relations between them, 
consider effective data structures 
4. Implement logic to view booking results 
5. Test the solution, develop your own input data files 

## Control questions  

1. What is inheritance?
2. What is polymorphism? How can it be achieved?
3. What is operator overloading?
4. Which types of constructors do you know?
5. What does the virtual keyword mean?
6. What does the override keyword mean?
7. What does the final keyword mean? In which contexts it can be used?
8. What does the pure method mean?
9. When do we need virtual functions?
10. What is dynamic_cast and how is it implemented under the hood?
11. Explain the difference between unique_ptr and shared_ptr.
12. Explain the difference between shared_ptr and weak_ptr.
13. RAII idiom.



## Assessment 🌟 
  - Usage of inheritance and polymorphism principles  - 4
  - The shape interface is uniform and strictly defined  - 2
  - The draw interface is the same for figures of all shapes and parameter count  - 2
  - There are 4 or more shapes available to the user (e.g. triangle, rectangle,
circle, line) - 1
  - All constraints are satisfied) - 1
- Test in Moodle - 1

**Total 10 + 1**

---






# Shapes blackboard - assignment3

## Task 🛠️

In this assignment, you should extend the blackboard that was already developed from the previous
assignment. This extension will be presented in the form of new commands, and the appearance
of new stricter requirements to the already developed parts.


| CMD     | Parameters                      | Description                                                                 | Result                                                                                                                | Example                                     |
|---------|----------------------------------|-----------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------------|---------------------------------------------|
| draw    | -                                | Draw blackboard to the console                                               | ASCII-drawn blackboard with all shapes on it in regards to their colour and fill mode or empty.                        | > draw <br> < [board]                      |
| list    | -                                | Print all added shapes with their IDs and parameters                         | List of all added shapes. One shape per line with shape ID and all information related to the shape.                   | > list <br> < [id][shape][params]          |
| shapes  | -                                | Print a list of all available shapes and parameters for add call             | List of all available shapes. One shape per line with a shape ID and all related information.                          | > shapes <br> < circle [params] <br>< box [params] |
| add     | shape fill\|frame red\|green… params | Add shape with specified colour and fill mode to the blackboard              | A shape with a specified colour is added to the blackboard.                                                            | > add fill red box 10 5 <br> < [id] box red 10 5 |
| select  | ID or coordinates                | Select a shape with an ID or foreground shape by the selected coordinates    | The shape was selected. Print info about the selected shape to the output.                                             | > select 0001 <br> < [shape][parameters] <br> > select 0 5 <br> < shape was not found |
| remove  | -                                | Remove the selected shape from the blackboard                                | The selected shape was removed.                                                                                       | > select 0001 <br> < [id][shape] removed   |
| edit    | new params                       | Allows to modify the parameters of the selected figure                       | The selected shape is modified. You cannot change the shape type. You cannot make a valid shape invalid.               | > select 0001 <br> > edit 5 1 <br> < error: invalid argument count <br> > edit 10 <br> < size of box changed <br> > edit 1000 <br> < error: shape will go out of the board |
| paint   | colour                           | Change the colour of the selected figure                                     | The selected shape has a new colour.                                                                                  | > select 0001 <br> > paint red <br> < [id][shape] red |
| move    | coordinates                      | Move the selected shape to the coordinates                                   | The selected shape is moved and becomes a foreground.                                                                 | > select 0001 <br> > move to 10 10 <br> < [id][shape] moved |
| clear   | -                                | Remove all shapes from blackboard                                            | All shapes are removed from the blackboard.                                                                           | > clear <br> < board is clear              |
| save    | file-path                        | Save the blackboard to the file                                              | Blackboard is saved to the file. The state of the blackboard remains unchanged.                                        | > save D:\test.bb                         |
| load    | file-path                        | Load a blackboard from the file                                              | Blackboard is loaded from the file. The previous state of the blackboard is cleared.                                   | > load D:\test.bb                         |



<details>
<summary>Code example🖥️ </summary>

---
#include <iostream>
#include <typeinfo>
class Base {
public:
virtual void show() {
std::cout << "Base class show function" << std::endl;
}
virtual ~Base() = default; // Ensure Base class has a virtual destructor
};
class Derived : public Base {
public:
void show() override {
std::cout << "Derived class show function" << std::endl;
}
void derivedFunction() {
std::cout << "Derived class specific function" << std::endl;
}
};
int main() {
Base* basePtr = new Derived(); // Base class pointer pointing to a Derived object
// Use dynamic_cast to attempt downcasting
Derived* derivedPtr = dynamic_cast<Derived*>(basePtr);
if (derivedPtr) {
// If cast succeeds, we can use Derived class specific functions
derivedPtr->show();
derivedPtr->derivedFunction();
} else {
std::cout << "dynamic_cast failed" << std::endl;
}
delete basePtr; // Clean up memory
return 0;
}

```

---

</details>



## Constraints
In addition to previous constraints there are a new one:
1. Command must be read from the console as a whole. In other words, you should input the
command with all its arguments as a single string.
2. Now shapes can be drawn not only as a frame but also as a whole-filled figure.
3. Shapes on the board have an associated colour with them, which should be respected.
Shapes must be “drawn” with the first letter of their colour as a symbol.
4. A file with an invalid board should not be loadable.
5. All objects must be stored in a single array. This was not an explicit, but an implicit
requirement from the previous work. Here we just want to state it explicitly.
6. You can draw figures using real colours. In that case, there will be extra points for you.
You can use external libraries or your platform API for that one.




## Task milestones 

1. Carefully read the table and think about the corner case of the designed system.
2. Update the system design if needed.
3. Implement figures selecting logic, consider figure level (background, foreground,
middle).
4. Add figure removing functionality.
5. Implement figures filling with a colour (painting).
6. Add ability to edit figures parameters.
7. Implement moving logic of the figure.
8. Test your solution and prepare a report document.



## Assessment 🌟 
  - Usage of inheritance and polymorphism principles including dynamic_cast
usage  - 4
  - Extended functionality is fully implemented  - 5
  - All constraints are satisfied  - 1
  - Extra points:real colours in the terminal -1


**Total 10 + 1**


