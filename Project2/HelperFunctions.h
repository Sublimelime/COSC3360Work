//HELPERFUNCTIONS_H
#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <string>
#include <vector>

//small struct to hold info about a resource
struct Resource {
 public:
  std::string type;
  std::vector<std::string> entries = std::vector<std::string>();
};

// Returns true if a string starts with a number.
bool lineStartsWithNum(std::string line) {
  switch(line[0]) {
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    return true;
  }
  return false;
}

#endif //HELPERFUNCTIONS_H
