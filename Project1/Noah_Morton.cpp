#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

int main(int argc, char *args[]) {
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

  // get size of 2d array to make
  int lines = 0;
  string unused;
  while (inFile1) {
    getline(inFile1, unused);
    lines++;
  }
  lines--; // Remove extra line
  inFile1.close();

  auto matrix = vector<vector<int>>(lines);
  for (int i = 0; i < lines; i++) {
    matrix.at(i) = vector<int>(lines, 0);
  }

  // read in values from file
  inFile1.open(args[1], ios::in);
  string lineRead;
  int readingY = 0, readingX = 0;
  while (inFile1) {
    getline(inFile1, lineRead);
    for (int i = 0; i < lineRead.length(); i += 2) {
      // prevent it being converted to the ascii code
      matrix.at(readingY).at(readingX) = lineRead[i] - '0';
      readingX++;
    }
    readingY++;
    readingX = 0;
  }
  inFile1.close();

  // setup necessary amount of pipes, counting number of 1s in matrix
  int numOfPipes = 0;
  for (int i = 0; i < matrix.size(); i++) {
    for (int j = 0; j < matrix[i].size(); j++) {
      if (matrix[i][j] == 1)
        numOfPipes++;
    }
  }

  auto pipes = vector<int[2]>(numOfPipes);
  for (int i = 0; i < pipes.size(); i++) {
    if (pipe(pipes[i]) < 0) {
      printf("Cannot make pipes, exiting.");
      return 1;
    }
  }

  return 0;
}
