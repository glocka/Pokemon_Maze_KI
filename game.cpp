//-----------------------------------------------------------------------------
// game.cpp
//
// Group: Group 13734, study assistant Christoph Maurer
//
// Authors: Gerald Birngruber (1530438)
// Paul Plessing (1530486)
// Richard Sadek (1531352)
//------------------------------------------------------------------------------
//

#include "game.h"
#include "helper.h"
#include "cmd_fastmove.h"
#include "field.h"
#include "direction.h"
#include "invalid_move_exception.h"
#include "error.h"
#include "dijkstra.h"
#include "backtrack.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <climits>

using std::string;
using std::getline;
using std::ifstream;
using std::ofstream;

Vector2 Game::Size = Vector2();

//------------------------------------------------------------------------------
Game::Game()
{
}

//------------------------------------------------------------------------------
Vector2 Game::findCurrentTeleporter()
{
  char current = getCurrentField();
  if (current >= 'A' && current <= 'Z')
  {
    int letter = current - 'A';
    if (std::get<0>(teleporters[letter]) == player_)
    {
      return std::get<1>(teleporters[letter]);
    }
    else if (std::get<1>(teleporters[letter]) == player_)
    {
      return std::get<0>(teleporters[letter]);
    }
  }

  // should never happen
  return Vector2();
}

//------------------------------------------------------------------------------
bool Game::isValidMaze()
{
  string valid_fields = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghij# ox+<>^vs123456789";
  int start_count = 0;
  int finish_count = 0;
  size_t maze_width = maze_[0].size();

  int teleporter[26];
  std::fill(teleporter, teleporter + 26, 0);
  for (size_t row = 0; row < maze_.size(); row++)
  {
    if (maze_[row].size() != maze_width)
    {
      return false;
    }

    if (maze_[row].find_first_not_of(valid_fields) != string::npos)
    {
      return false;
    }

    for (size_t column = 0; column < maze_[row].size(); column++)
    {
      char current = maze_[row][column];
      //check for valid walls
      if (row == 0 || row == maze_.size() - 1 || column == 0
          || column == maze_[row].length() - 1)
      {
        if (current != '#')
        {
          return false;
        }
      }

      // check for start and finish
      if (current == 'o')
      {
        player_.set(column, row);
        start_position_.set(column, row);
        start_count++;
      }
      if (current == 'x')
      {
        finish_count++;
        finish_position_.set(column, row);
      }

      if (current >= 'a' && current <= 'e')
      {
        Vector2 position(column, row);
        bonus_fields_.push_back(position.getY() * maze_width + position.getX());
      }

      // check for if there are 2 teleport fields #backtothefuture
      if (current >= 'A' && current <= 'Z')
      {
        int count = teleporter[current - 'A']++;
        if (count == 0)
        {
          std::get<0>(teleporters[current - 'A']) = Vector2(column, row);
        }
        else
        {
          std::get<1>(teleporters[current - 'A']) = Vector2(column, row);
        }
      }
    }
  }

  if (start_count != 1 || finish_count != 1)
  {
    return false;
  }

  for (int tp_count = 0; tp_count < 26; tp_count++)
  {
    if (teleporter[tp_count] != 2 && teleporter[tp_count] != 0)
    {
      return false;
    }
  }
  return true;
}

//------------------------------------------------------------------------------
bool Game::load(const string& load_filename)
{
  if (!Helper::isValidFilename(load_filename))
  {
    Error::show(Error::WRONG_PARAMETER);
    return false;
  }

  // read information from file
  string line;
  string steps;
  string steps_count;

  ifstream load_file;
  load_filename_ = load_filename;
  load_file.open(load_filename);
  if (!load_file)
  {
    Error::show(Error::CANNOT_OPEN_FILE);
    return false;
  }

  getline(load_file, steps);
  getline(load_file, steps_count);

  while (getline(load_file, line))
  {
    maze_.push_back(line);
  }

  if (maze_.size() == 0)
  {
    Error::show(Error::INVALID_FILE);
    return false;
  }

  if (maze_.back() == "")
  {
    maze_.pop_back();
  }

  // check path for correct characters
  if (steps.find_first_not_of("urdl") != string::npos)
  {
    Error::show(Error::INVALID_FILE);
    return false;
  }

  // check steps for correct charecters
  if (steps_count == ""
      || steps_count.find_first_not_of("0123456789") != string::npos)
  {
    Error::show(Error::INVALID_FILE);
    return false;
  }
  remaining_steps_ = std::stoi(steps_count);


  if (!isValidMaze())
  {
    Error::show(Error::INVALID_FILE);
    return false;
  }

  original_maze_ = maze_;
  original_remaining_steps_ = remaining_steps_;

  try
  {
    fastmove(steps);
  }
  catch (InvalidMoveException& e)
  {
    Error::show(Error::INVALID_PATH);
    return false;
  }

  // set static size of field
  Size = Vector2(maze_[0].size(), maze_.size());

  return true;
}

//------------------------------------------------------------------------------
void Game::save() const
{
  save(auto_save_filename_);
}

//------------------------------------------------------------------------------
void Game::save(const string& save_filename) const
{
  if (save_filename == "")
  {
    return;
  }
  if (!Helper::isValidFilename(save_filename))
  {
    Error::show(Error::WRONG_PARAMETER);
    return;
  }

  // put content into file
  try
  {
    ofstream save_file;
    save_file.open(save_filename);
    save_file.exceptions(ofstream::failbit | ofstream::badbit);

    save_file << steps_ << "" << std::endl;
    save_file << original_remaining_steps_ << "" << std::endl;
    for (size_t row = 0; row < original_maze_.size(); row++)
    {
      // content gets flushed in close of stream(no endl needed)
      save_file << original_maze_[row] << "\n";
    }
    save_file.close();
  }
  catch (ofstream::failure& e)
  {
    Error::show(Error::CANNOT_WRITE_FILE);
  }
}

//------------------------------------------------------------------------------
void Game::reset()
{
  maze_ = original_maze_;
  remaining_steps_ = original_remaining_steps_;
  steps_ = "";
  player_ = start_position_;
  save();
}

//------------------------------------------------------------------------------
void Game::show() const
{
  for (unsigned i = 0; i < getMaze().size(); i++)
  {
    string current = getMaze()[i];
    if (i == (unsigned)player_.getY())
    {
      current[player_.getX()] = '*';
    }
    std::cout << current << std::endl;
  }
}

//------------------------------------------------------------------------------
void Game::showMore() const
{
  std::cout << "Remaining Steps: " << getRemainingSteps()  << std::endl;
  std::cout << "Moved Steps: "<< getSteps()<< std::endl;

  show();
}


//------------------------------------------------------------------------------
void Game::showMoreNopath() const
{
  std::cout << "Remaining Steps: " << getRemainingSteps()  << std::endl;

  show();
}

//------------------------------------------------------------------------------
void Game::move(const Direction& direction)
{
  if (getCurrentField() == 'x')
  {
    // all steps invalid after reaching finish
    throw InvalidMoveException();
  }

  if(getRemainingSteps() == 0)
  {
    Error::show(Error::NO_STEPS_POSSIBLE);
    reset();
    return;
  }

  Game tmp_game = *this;
  step(tmp_game, direction);
  *this = tmp_game;
  save();
  show();

  if (getCurrentField() == 'x')
  {
    std::cout << "Congratulation! You solved the maze." << std::endl;
  }
}

//------------------------------------------------------------------------------
void Game::fastmove(const string& steps)
{
  if (getCurrentField() == 'x')
  {
    // all steps invalid after reaching finish
    throw InvalidMoveException();
  }

  if(getRemainingSteps() == 0)
  {
    Error::show(Error::NO_STEPS_POSSIBLE);
    reset();
    return;
  }

  Game tmp_game = *this;

  for (char direction : steps)
  {
    string dir;
    dir += direction;
    step(tmp_game, Direction::getDirection(dir));
  }

  *this = tmp_game;
  save();
  show();

  if (getCurrentField() == 'x')
  {
    std::cout << "Congratulation! You solved the maze." << std::endl;
  }
}

//------------------------------------------------------------------------------
void Game::step(Game& tmp_game, const Direction& direction)
{
  if(tmp_game.getRemainingSteps() == 0)
  {
    // will only occur when fastmoving, because
    // remaining steps is checked before calling this method
    throw InvalidMoveException();
  }

  // handle current field
  switch (tmp_game.getCurrentField())
  {
    case Field::ONEWAYUP:
      if (direction.getY() != -1)
        throw InvalidMoveException();
      break;
    case Field::ONEWAYDOWN:
      if (direction.getY() != 1)
        throw InvalidMoveException();
      break;
    case Field::ONEWAYLEFT:
      if (direction.getX() != -1)
        throw InvalidMoveException();
      break;
    case Field::ONEWAYRIGHT:
      if (direction.getX() != 1)
        throw InvalidMoveException();
      break;
  }

  tmp_game.player_.moveby(direction.getX(), direction.getY());

  // handle new field
  switch (tmp_game.getCurrentField())
  {
    case Field::WALL:
      throw InvalidMoveException();

    case Field::ICE:
      while (tmp_game.getCurrentField() == Field::ICE)
      {
        tmp_game.player_.moveby(direction.getX(), direction.getY());

        if (tmp_game.getCurrentField() == Field::WALL)
        {
          tmp_game.player_.moveby(-direction.getX(), -direction.getY());
          break;
        }
      }
    break;
  }

  // handle teleporting
  char current = tmp_game.getCurrentField();
  if (current >= 'A' && current <= 'Z')
  {
    tmp_game.player_ = tmp_game.findCurrentTeleporter();
  }

  // handle hole
  if (current == 's')
  {
    tmp_game.player_ = start_position_;
  }

  // handle counterfields
  if (current == '0')
  {
    throw InvalidMoveException();
  }
  else if (current >= '1' && current <= '9')
  {
    if (current == '1')
    {
      tmp_game.setCurrentField('#');
    }
    else
    {
      tmp_game.setCurrentField(current - 1);
    }
  }

  // handle bonus / malus
  int bonus = 0;
  if (current >= 'a' && current <= 'e')
  {
    bonus = current - 'a' + 1;
    tmp_game.setCurrentField(' ');
  }
  else if (current >= 'f' && current <= 'j')
  {
    bonus = -(current - 'f' + 1);
  }
  tmp_game.setRemainingSteps(tmp_game.getRemainingSteps() + bonus - 1);


  if (tmp_game.getRemainingSteps() < 0)
  {
    tmp_game.setRemainingSteps(0);
  }

  tmp_game.addSteps(direction.getShortHand());
}

//------------------------------------------------------------------------------
int Game::tryStep(const Direction& direction)
{

  // handle current field
  switch (getCurrentField())
  {
    case Field::WALL:
      return 0;
      break;
    case Field::ONEWAYUP:
      if (direction.getY() != -1)
        return 0;
      break;
    case Field::ONEWAYDOWN:
      if (direction.getY() != 1)
        return 0;
      break;
    case Field::ONEWAYLEFT:
      if (direction.getX() != -1)
        return 0;
      break;
    case Field::ONEWAYRIGHT:
      if (direction.getX() != 1)
        return 0;
      break;
  }
  player_.moveby(direction.getX(), direction.getY());

  // handle new field
  switch (getCurrentField())
  {
    case Field::WALL:
      return 0;

    case Field::ICE:
      while (getCurrentField() == Field::ICE)
      {
        player_.moveby(direction.getX(), direction.getY());

        if (getCurrentField() == Field::WALL)
        {
          player_.moveby(-direction.getX(), -direction.getY());
          break;
        }
      }
    break;
  }

  // handle teleporting
  char current = getCurrentField();
  if (current >= 'A' && current <= 'Z')
  {
    player_ = findCurrentTeleporter();
  }

  if (current == 's')
  {
    player_ = start_position_;
  }

  // handle bonus / malus
  if (current >= 'f' && current <= 'j')
  {
    return current - 'f' + 2;
  }

  return 1;
}

void Game::solve(bool silent)
{
  if (getCurrentField() == 'x')
  {
    Error::show(Error::ALREADY_SOLVED);
    return;
  }

  if (solveMaze())
  {
    save(load_filename_ + "Solved");

    std::cout << "The maze was solved in " << original_remaining_steps_ - remaining_steps_ << " steps." << std::endl;
    if (!silent)
    {
      std::cout << "Found path: " << steps_ << std::endl;
    }
  }
}

//------------------------------------------------------------------------------
bool Game::solveMaze()
{
  int field_count = Size.getX() * Size.getY();
  Vector2 original_player = player_;
  Vector2 first;

  bonus_fields_.push_back(player_.getIndex());
  bonus_fields_.push_back(finish_position_.getIndex());

  Backtrack bonus_backtrack(bonus_fields_.size());
  Dijkstra dijkstra(field_count);
  int total_path_length = 0;

  // iterate over all fields, but the fields on the edge
  for (int i = 1; i < Size.getY() - 1; i++)
  {
    for (int j = 1; j < Size.getX() - 1; j++)
    {
      for (Direction direction : Direction::Directions)
      {
        player_ = Vector2(j, i);
        first = player_;

        int distance = tryStep(direction);
        if (distance != 0)
        {
          int fieldIndex1 = first.getIndex();
          int fieldIndex2 = player_.getIndex();
          dijkstra.addEdge(fieldIndex1, fieldIndex2, distance);
        }
      }
    }
  }

  // calculate shortest paths between all bonus fields and start and finish
  for (unsigned i = 0; i < bonus_fields_.size(); i++)
  {
    dijkstra.setSource(bonus_fields_[i]);
    dijkstra.calculate();
    for (unsigned j = 0; j < bonus_fields_.size(); j++)
    {
      if (i != j)
      {
        dijkstra.setDestination(bonus_fields_[j]);
        Vector2 field(bonus_fields_[j]);
        int bonus = getBonus(field);
        int weight = dijkstra.getShortestPathLength() + bonus;

        bonus_backtrack.addEdge(i, j, weight);
      }
    }
  }

  bonus_backtrack.setSource(bonus_fields_.size() - 2);
  bonus_backtrack.setDestination(bonus_fields_.size() - 1);
  bonus_backtrack.calculate();

  vector<int> bonus_path = bonus_backtrack.getPath().getPath();
  vector<int> complete_path;

  if (bonus_path.size() == 0)
  {
    player_ = original_player;
    Error::show(Error::NO_PATH_FOUND);
    return false;
  }

  for (unsigned i = 0; i < bonus_path.size() - 1; i++)
  {
    dijkstra.setSource(bonus_fields_[bonus_path[i]]);
    dijkstra.setDestination(bonus_fields_[bonus_path[i + 1]]);
    dijkstra.calculate();

    vector<int> path = dijkstra.getPath().getPath();
    int length = dijkstra.getPath().getLength();
    Vector2 field(bonus_fields_[i]);
    total_path_length += length + getBonus(field);

    std::reverse(path.begin(), path.end());
    complete_path.insert(complete_path.end(), path.begin(), path.end() - 1);
  }
  complete_path.push_back(finish_position_.getIndex());

  total_path_length++; // for reaching finish
  if (total_path_length > remaining_steps_)
  {
    player_ = original_player;
    Error::show(Error::NO_PATH_FOUND);
    return false;
  }

  player_ = original_player;
  calculatePath(complete_path);
  return true;
}

void Game::calculatePath(vector<int>& nodes)
{  
  Vector2 original_player = player_;
  string path = "";

  for (unsigned i = 0; i < nodes.size() - 1; i++)
  {
    for (Direction direction : Direction::Directions)
    {
      player_ = Vector2(nodes[i]);
      tryStep(direction);
      if (player_ == nodes[i + 1])
      {
        path.push_back(direction.getShortHand());
      }
    }
  }
  player_ = original_player;
  fastmove(path);
}

//------------------------------------------------------------------------------
bool Game::isLoaded() const
{
  if (maze_.size() == 0)
  {
    return false;
  }
  else
  {
    return true;
  }
}

//------------------------------------------------------------------------------
string Game::getAutoSaveFilename() const
{
  return auto_save_filename_;
}

//------------------------------------------------------------------------------
const vector<string>& Game::getMaze() const
{
  return maze_;
}

//------------------------------------------------------------------------------
char Game::getCurrentField() const
{
  return maze_[player_.getY()][player_.getX()];
}

int Game::getBonus(Vector2 field) const
{
  char field_char = maze_[field.getY()][field.getX()];
  if (field_char >= 'a' && field_char <= 'e')
  {
    return -(field_char - 'a') - 1;
  }
  else
  {
    return 0;
  }
}

//------------------------------------------------------------------------------
int Game::getRemainingSteps() const
{
  return remaining_steps_;
}

//------------------------------------------------------------------------------
string Game::getSteps() const
{
  return steps_;
}

//------------------------------------------------------------------------------
void Game::setAutoSaveFilename(const string& save_filename)
{
  auto_save_filename_ = save_filename;
}

//------------------------------------------------------------------------------
void Game::setCurrentField(const char field)
{
  maze_[player_.getY()][player_.getX()] = field;
}

//------------------------------------------------------------------------------
void Game::setRemainingSteps(const int number)
{
  remaining_steps_ = number;
}

//------------------------------------------------------------------------------
void Game::addSteps(const char step)
{
  steps_.push_back(step);
}
