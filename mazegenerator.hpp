#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H

#include <SFML/Graphics.hpp>
#include <stack>
#include <string>
#include <vector>
#include "random.hpp"

class MazeGenerator {
 public:
  explicit MazeGenerator();
  void run();

 private:
  enum {
    CELL_SIZE = 128,
    WALL_WIDTH = 4,
    CELLS_IN_WIDTH = 3,
    CELLS_IN_HEIGHT = 3,
    WINDOW_WIDTH = CELLS_IN_WIDTH * CELL_SIZE,
    WINDOW_HEIGHT = CELLS_IN_HEIGHT * CELL_SIZE
  };
  enum class Direction { NORTH, SOUTH, EAST, WEST };
  enum {
    CELL_PATH_NORTH = 0x01,
    CELL_PATH_SOUTH = 0x02,
    CELL_PATH_EAST = 0x04,
    CELL_PATH_WEST = 0x08,
    CELL_VISITED = 0x10
  };
  std::vector<int> mMaze;
  std::stack<int> mStack;
  int mVisitedCount{0};
  Random random;
  const std::string WINDOW_TITLE{"MazeGenerator"};
  sf::RenderWindow mWindow;
  void inputPhase();
  void updatePhase();
  void renderingPhase();
  void newMaze();
  int getIndexByCoords(int x, int y);
  sf::Vector2i getCoordsByIndex(int index);
  bool isIndexValid(int index);
  std::vector<std::pair<int, Direction> > getNeighbours(int index);
};

#endif  // MAZEGENERATOR_H