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
    CELL_SIZE = 32,
    WALL_WIDTH = 6,
    CELLS_IN_WIDTH = 20,
    CELLS_IN_HEIGHT = 20,
    WINDOW_WIDTH = CELLS_IN_WIDTH * CELL_SIZE,
    WINDOW_HEIGHT = CELLS_IN_HEIGHT * CELL_SIZE
  };
  enum class Direction { NORTH, EAST, SOUTH, WEST };
  enum {
    CELL_PATH_NORTH = 0x01,
    CELL_PATH_EAST = 0x02,
    CELL_PATH_SOUTH = 0x04,
    CELL_PATH_WEST = 0x08,
    CELL_VISITED = 0x10
  };
  std::vector<int> mMaze;
  std::stack<int> mStack;
  size_t mCurrIndex;
  size_t mVisitedCount{0};
  bool mFinished {false};
  Random random;
  const std::string WINDOW_TITLE{"MazeGenerator"};
  sf::RenderWindow mWindow;
  sf::Color mWallColor {sf::Color::White};
  sf::Color mVisitedColor {sf::Color(127,0,110)};
  sf::Color mUnvisitedColor {sf::Color(64,64,64)};
  void inputPhase();
  void updatePhase();
  void renderingPhase();
  void newMaze();
  int getIndexByCoords(int x, int y);
  sf::Vector2i getCoordsByIndex(int index);
  bool areCoordinatesValid(int x, int y);
  std::vector<std::pair<int, Direction> > getNeighbours(int index);
};

#endif  // MAZEGENERATOR_H
