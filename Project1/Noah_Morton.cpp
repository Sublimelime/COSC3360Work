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

// Function to loop through all pipes created and close them, ensuring no loose
// ends. Will be called by every process.
void closeAllPipes(std::vector<int[2]> &pipeVector) {
  for (unsigned long int i = 0; i < pipeVector.size(); i++) {
    close(pipeVector[i][0]);
    close(pipeVector[i][1]);
  }
}

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
  auto pipes = vector<int[2]>(numOfPipes); // vector to hold pipe descriptors
  for (long unsigned int i = 0; i < pipes.size(); i++) {
    if (pipe(pipes[i]) < 0) {
      printf("Cannot make pipes, exiting.\n");
      return 1;
    }
  }

  // Read words from second file in
  auto words = vector<string>();
  inFile2.open(args[2], ios::in);
  if (!inFile2.is_open()) {
    printf("Unable to open file 2 for reading, exiting.\n");
    return 1;
  }
  string word;
  while (inFile2 >> word) {
    if (word[word.length() - 1] == ',') { // Really wish C++ had .EndsWith()
      word.erase(word.length() - 1, 1);   // chop off comma
    }
    words.push_back(word);
  }
  inFile2.close();

  // Forking - Store process info into vector of processes for later work
  auto processes = vector<VertexProcess>();
  for (int i = 0; i < numOfProcesses; i++) {
    auto proc = VertexProcess(i, isInput(matrix, i), isOutput(matrix, i));
    if (!proc.isInitialInput)
      proc.determineInputs(matrix);
    if (!proc.isFinalOutput)
      proc.determineOutputs(matrix);
    processes.push_back(proc);
  }

  // assign words to initial inputs
  for (int i = 0; i < processes.size(); i++) {
    if (processes.at(i).isInitialInput) {
      processes.at(i).myWord = words.front();
      // remove word from list of words once it's assigned
      words.erase(words.begin());
    }
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

  // Processes diverge here, depending on ID and information in `proccesses`

  if (processes.at(myID).isInitialInput) {
    close(pipes[myID][0]); // close read end
    auto &myOutputs = processes.at(myID).outputs;
    for (int i = 0; i < myOutputs.size(); i++) {
      write(pipes[myOutputs[i]][1], processes.at(myID).myWord.c_str(),
            processes.at(myID).myWord.length() + 1);
      close(pipes[myOutputs[i]][1]); // close pipe written to
    }
  } else if (processes.at(myID).isFinalOutput) { // final output printer
    char sentenceRead[100];
    auto &myInputs = processes.at(myID).inputs;
    for (int i = 0; i < myInputs.size(); i++) {
      // close write end
      close(pipes[myInputs[i]][1]);
      read(pipes[myInputs[i]][0], sentenceRead, 100);
    }

    // word processing stuff I don't have time to do

    printf("%s\n", sentenceRead);
  } else { // connecting process

  }

  closeAllPipes(pipes); // Pipe cleanup
  return 0;
}
