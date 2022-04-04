//PROCESSINFO_H
#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include <vector>
#include <string>

class ProcessInfo {
 public:
  int id;
  std::vector<std::string> commands = std::vector<std::string>();

};

#endif //PROCESSINFO_H
