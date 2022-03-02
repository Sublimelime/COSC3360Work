#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include "VertexProcess.h"

// Counts the number of 1s in the matrix to determine neccessary amount of pipes
int calcNumOfPipes(const std::vector<std::vector<int>> &matrix) {
  int numOfPipes = 0;
  for (long unsigned int i = 0; i < matrix.size(); i++) {
    for (long unsigned int j = 0; j < matrix[i].size(); j++) {
      if (matrix[i][j] == 1)
        numOfPipes++;
    }
  }
  return numOfPipes;
}

// Figures out if a process is an initial input, based on the matrix having all
// 0s across the vertical.
bool isInput(std::vector<std::vector<int>> &matrix, int id) {
  bool success = true;
  for (long unsigned int i = 0; i < matrix.size(); i++) {
    if (matrix[i][id] == 1)
      success = false;
  }

  return success;
}

// Figures out if the process will be a printing process, based on the matrix
// having all 0s across the horizontal.
bool isOutput(std::vector<std::vector<int>> &matrix, int id) {
  bool success = true;
  for (long unsigned int i = 0; i < matrix.size(); i++) {
    if (matrix[id][i] == 1)
      success = false;
  }

  return success;
}

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
  int numOfProcesses = 0;
  string unused;
  while (inFile1) {
    getline(inFile1, unused);
    numOfProcesses++;
  }
  numOfProcesses--; // Remove extra line
  inFile1.close();

  auto matrix = vector<vector<int>>(numOfProcesses);
  for (int i = 0; i < numOfProcesses; i++) {
    matrix.at(i) = vector<int>(numOfProcesses, 0);
  }

  // read in values from file
  inFile1.open(args[1], ios::in);
  string lineRead;
  int readingY = 0, readingX = 0;
  while (inFile1) {
    getline(inFile1, lineRead);
    for (long unsigned int i = 0; i < lineRead.length(); i += 2) {
      // prevent it being converted to the ascii code
      matrix.at(readingY).at(readingX) = lineRead[i] - '0';
      readingX++;
    }
    readingY++;
    readingX = 0;
  }
  inFile1.close();

  // setup necessary amount of pipes, counting number of 1s in matrix
  int numOfPipes = calcNumOfPipes(matrix);
  auto pipes = vector<int[2]>(numOfPipes); //vector to hold pipe descriptors
  for (long unsigned int i = 0; i < pipes.size(); i++) {
    if (pipe(pipes[i]) < 0) {
      printf("Cannot make pipes, exiting.\n");
      return 1;
    }
  }

  // Forking - Store process info into vector of processes for later work
  auto processes = vector<VertexProcess>();
  for (int i = 0; i < numOfProcesses; i++) {
    auto proc = VertexProcess(i, isInput(matrix, i), isOutput(matrix, i));
    if (!proc.isInitialInput)
      proc.determineInputs(matrix);
    if (!proc.isOutput)
      proc.determineOutputs(matrix);
    processes.push_back(proc);
  }

  int myID = -1;
  for (int i = 0; i < numOfProcesses; i++) {
    int x = fork();
    if (x == 0) {
      myID = i;
      break;
    } else if (x < 0) {
      printf("Unable to fork, exiting.\n");
      return 1;
    }
  }

  return 0;
}
