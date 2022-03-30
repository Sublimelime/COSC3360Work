#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

int main(int argc, char **args) {
  using namespace std;

  if (argc != 3) {
    printf("Started with too many/few arguments, exiting.\n");
    return 1;
  }

  fstream inFile1, inFile2;
  inFile1.open(args[1], ios::in);
  if (!inFile1.is_open()) {
    printf("Unable to open matrix input file, exiting.\n");
    return 1;
  }

  // holds lines from `many` file, used to setup program
  auto commandFileLines = vector<string>();
  string line;

  //read in all lines from file
  while (inFile1) {
    getline(inFile1, line);
    commandFileLines.push_back(line);
  }
  inFile1.close();


}
