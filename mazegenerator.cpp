#include "mazegenerator.hpp"
#include <iostream>
#include <algorithm>
#include <stack>

MazeGenerator::MazeGenerator()
    : mMaze(CELLS_IN_WIDTH * CELLS_IN_HEIGHT, 0x00),
      mWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE) {
  auto screenWidth = sf::VideoMode::getDesktopMode().width;
  auto screenHeight = sf::VideoMode::getDesktopMode().height;
  auto dx = (screenWidth - mWindow.getSize().x) / 2;
  auto dy = (screenHeight - mWindow.getSize().y) / 2;
  mWindow.setPosition(sf::Vector2i(dx, dy));
  mStack.push(0);
  mMaze[0] = CELL_VISITED;
  mVisitedCount = 1;
}

void MazeGenerator::run() {
  while (mWindow.isOpen()) {
    inputPhase();
    updatePhase();
    renderingPhase();
  }
}

void MazeGenerator::inputPhase() {
  sf::Event event;
  while (mWindow.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      mWindow.close();
    }
    if (event.type == sf::Event::KeyReleased) {
      switch (event.key.code) {
        case sf::Keyboard::N:
          newMaze();
          break;
        case sf::Keyboard::Escape:
        case sf::Keyboard::Q:
          mWindow.close();
          break;
        default:
          break;
      }
    }
    if (event.type == sf::Event::MouseButtonReleased &&
        event.mouseButton.button == sf::Mouse::Button::Left) {
    }
  }
}

void MazeGenerator::updatePhase() {
  if (mVisitedCount >= CELLS_IN_WIDTH * CELLS_IN_HEIGHT) {
      std::cout << "FINISHED" << std::endl;
      return;
  }

  int currIndex = mStack.top();
  std::cout << "currIndex =  " << currIndex << std::endl;
  std::cout << "row =  " << (currIndex / CELLS_IN_WIDTH) << std::endl;
  std::cout << "col =  " << (currIndex % CELLS_IN_WIDTH) << std::endl;
  //mStack.pop();
  auto neighbours = getNeighbours(currIndex);
  /*std::vector<int> unvisitedNeighbours;
  std::copy_if(neighbours.begin(), neighbours.end(),
                 std::back_inserter(unvisitedNeighbours), [](int index){
      return !(mMaze[index] & CELL_VISITED);
  });*/

  neighbours.erase(std::remove_if(neighbours.begin(), neighbours.end(),[&](auto item){
      return mMaze[item.first] & CELL_VISITED;
  }), neighbours.end());

  if(!neighbours.empty()){
      int randNeighbourIndex = random.nextInt(neighbours.size());
      int randNeighbourIndexInMaze = neighbours[randNeighbourIndex].first;
      Direction neighbourDir = neighbours[randNeighbourIndex].second;
      mStack.push(randNeighbourIndexInMaze);
      if(neighbourDir == Direction::NORTH){
          mMaze[currIndex] |= CELL_PATH_NORTH;
          mMaze[randNeighbourIndexInMaze] |= CELL_PATH_SOUTH;
      } else if(neighbourDir == Direction::SOUTH){
          mMaze[currIndex] |= CELL_PATH_SOUTH;
          mMaze[randNeighbourIndexInMaze] |= CELL_PATH_NORTH;
      } else if(neighbourDir == Direction::EAST){
          mMaze[currIndex] |= CELL_PATH_EAST;
          mMaze[randNeighbourIndexInMaze] |= CELL_PATH_WEST;
      } else if(neighbourDir == Direction::WEST){
          mMaze[currIndex] |= CELL_PATH_WEST;
          mMaze[randNeighbourIndexInMaze] |= CELL_PATH_EAST;
      }
      mMaze[currIndex] |= CELL_VISITED;
      ++mVisitedCount;
  } else {
      mStack.pop();
  }

}

void MazeGenerator::renderingPhase() {
  mWindow.clear(sf::Color::Black);
  int counter = 0;
  for (size_t i{0u}; i < mMaze.size(); ++i) {
    auto spot = getCoordsByIndex(i);
    auto position = sf::Vector2i(spot.x * CELL_SIZE, spot.y * CELL_SIZE);
    sf::RectangleShape cell;
    cell.setPosition(position.x, position.y);
    cell.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    cell.setFillColor(sf::Color::Red);
    mWindow.draw(cell);

    cell.move(WALL_WIDTH, WALL_WIDTH);
    cell.setSize(sf::Vector2f(CELL_SIZE - WALL_WIDTH, CELL_SIZE - WALL_WIDTH));
    if (mMaze[i] & CELL_VISITED) {
      cell.setFillColor(sf::Color::Cyan);
    } else {
      cell.setFillColor(sf::Color::Blue);
    }
    mWindow.draw(cell);
    cell.setSize(sf::Vector2f(-CELL_SIZE, -CELL_SIZE));

    if (mMaze[i] & CELL_PATH_NORTH) {
      sf::RectangleShape northernPath;
      northernPath.setPosition(position.x + WALL_WIDTH, position.y);
      northernPath.setSize(
          sf::Vector2f(CELL_SIZE - 2 * WALL_WIDTH, WALL_WIDTH));
      northernPath.setFillColor(sf::Color::Cyan);
      mWindow.draw(northernPath);
    }

    if (mMaze[i] & CELL_PATH_SOUTH) {
      sf::RectangleShape southernPath;
      southernPath.setPosition(position.x + WALL_WIDTH,
                               position.y + CELL_SIZE - WALL_WIDTH);
      southernPath.setSize(
          sf::Vector2f(CELL_SIZE - 2 * WALL_WIDTH, WALL_WIDTH));
      southernPath.setFillColor(sf::Color::Cyan);
      mWindow.draw(southernPath);
    }

    if (mMaze[i] & CELL_PATH_EAST) {
      sf::RectangleShape easternPath;
      easternPath.setPosition(position.x + CELL_SIZE - WALL_WIDTH,
                              position.y + WALL_WIDTH);
      easternPath.setSize(sf::Vector2f(WALL_WIDTH, CELL_SIZE - 2 * WALL_WIDTH));
      easternPath.setFillColor(sf::Color::Cyan);
      mWindow.draw(easternPath);
    }

    if (mMaze[i] & CELL_PATH_WEST) {
      sf::RectangleShape westernPath;
      westernPath.setPosition(position.x, position.y + WALL_WIDTH);
      westernPath.setSize(sf::Vector2f(WALL_WIDTH, CELL_SIZE - 2 * WALL_WIDTH));
      westernPath.setFillColor(sf::Color::Cyan);
      mWindow.draw(westernPath);
    }
    ++counter;
  }
  mWindow.display();
}

void MazeGenerator::newMaze() {}

int MazeGenerator::getIndexByCoords(int x, int y) {
  return y * CELLS_IN_WIDTH + x;
}

sf::Vector2i MazeGenerator::getCoordsByIndex(int index) {
  int y = index / CELLS_IN_WIDTH;
  int x = index % CELLS_IN_WIDTH;
  return sf::Vector2i(x, y);
}

bool MazeGenerator::isIndexValid(int index) {
  return index >= 0 && index < mMaze.size();
}

std::vector<std::pair<int, MazeGenerator::Direction> > MazeGenerator::getNeighbours(int index) {
  std::vector<std::pair<int, Direction>> neighbours;
  std::vector<sf::Vector2i> offsets{{0, -1}, {0, 1}, {1, 0}, {-1, 0}};
  auto currCellPosition = getCoordsByIndex(index);
  for (int i = 0; i < offsets.size(); ++i) {
    Direction dir = static_cast<Direction>(i);
    auto offset = offsets[i];
    int nx = currCellPosition.x + offset.x;
    int ny = currCellPosition.y + offset.y;
    int neighbourIndex = getIndexByCoords(nx, ny);
    if (isIndexValid(neighbourIndex)) {
      neighbours.push_back(std::make_pair(neighbourIndex, dir));
    }
  }
  return neighbours;
}
