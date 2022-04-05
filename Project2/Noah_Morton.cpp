#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include "HelperFunctions.h"
#include "ProcessInfo.h"

int main(int argc, char **args) {
  using namespace std;

  if (argc != 3) {
    printf("Started with too many/few arguments, exiting.\n");
    return 1;
  }

  fstream inFile1, inFile2;
  inFile1.open(args[1], ios::in);
  if (!inFile1.is_open()) {
    printf("Unable to open many.txt input file, exiting.\n");
    return 1;
  }

  // holds lines from `many` file, used to setup program
  auto commandFileLines = vector<string>();
  string inFile1Line;
  // read in all lines from file
  while (inFile1) {
    getline(inFile1, inFile1Line);
    if (inFile1Line != "") // ignore empty lines
      commandFileLines.push_back(inFile1Line);
  }
  inFile1.close();

  // Keeps track of max resources available for each resource
  auto resourcesAvailable = vector<int>(stoi(commandFileLines.at(0)), 0);
  // holds ProcessInfos of each process
  auto processes = vector<ProcessInfo>(stoi(commandFileLines.at(1)));

  // read available and max usage of resources per process, store this info
  int processInstructionBeginPoint;
  for (int i = 2; i < commandFileLines.size(); i++) {
    if (commandFileLines.at(i).substr(0, 9) == "available") {
      // going to just assume no more than 9
      int resource = commandFileLines.at(i)[10] - '0';
      int maximum = commandFileLines.at(i)[13] - '0';
      resourcesAvailable.at(resource) = maximum;
    } else if (commandFileLines.at(i).substr(0, 3) == "max") {
      // going to just assume no more than 9, one digit
      int process = commandFileLines.at(i)[4] - '0';
      int resource = commandFileLines.at(i)[6] - '0';
      int resourceMax = commandFileLines.at(i)[9] - '0';
      processes.at(process).resourceMaximums.at(resource) = resourceMax;
    } else { // found where the process instructions begin
      processInstructionBeginPoint = i;
      break;
    }
  }

  // read each process's commands, deadline and processing time into its
  // ProcessInfo
  int processBeingRead = 0;
  for (int i = processInstructionBeginPoint; i < commandFileLines.size(); i++) {
    string line = commandFileLines.at(i);
    if (line.substr(0, 7) == "process") {
      processBeingRead = line[8] - '0';
      processBeingRead--; // starts 1 indexed, so reduce to 0
      cout << "Switching to new process " << processBeingRead << endl;
    } else if (lineStartsWithNum(line)) {
      // got deadline + comp time, read next line too
      auto &currentProcess = processes.at(processBeingRead);
      currentProcess.deadline = stoi(line);
      line = commandFileLines.at(i + 1); // next line
      currentProcess.computationTime = stoi(line);
      i++; // skip over

      // sanity check
      if (currentProcess.deadline < currentProcess.computationTime) {
        printf("Process %d's computation time exceeds deadline. Impossible "
               "input.\n",
               processBeingRead);
        return 1;
      }

    } else { // got a command, add it to list
      processes.at(processBeingRead).commands.push_back(line);
    }
  }

  // second input file reading ------------------

  inFile2.open(args[2], ios::in);
  if (!inFile2.is_open()) {
    printf("Unable to open many_words.txt input file, exiting.\n");
    return 1;
  }

  // holds lines from `many_words` file
  auto wordsFileLines = vector<string>();
  string inFile2Line;
  // read in all lines from file
  while (inFile2) {
    getline(inFile2, inFile2Line);
    if (inFile2Line != "") // ignore empty lines
      wordsFileLines.push_back(inFile2Line);
  }
  inFile2.close();

  // parse lines into Resources
  auto resources = vector<Resource>(wordsFileLines.size());
  for (int i = 0; i < wordsFileLines.size(); i++) {
    string currentLine = wordsFileLines.at(i);
    // get type ------------
    string type = "";
    int entriesBeginAt;
    for (int x = 4; x < currentLine.length(); x++) {
      if (currentLine[x] == ':') {
        entriesBeginAt = x + 2;
        break;
      }
      // adding character by character until we hit `:`
      type += currentLine[x];
    }
    resources.at(i).type = type;

    // get entries --------------
    string entry = "";
    for (int x = entriesBeginAt; x < currentLine.length(); x++) {
      if (currentLine[x] == ',') {
        // when we hit a comma we finalize the entry and shift to the next
        resources.at(i).entries.push_back(entry);
        x += 2;
      } else {
        entry += currentLine[x];
      }
    }
  }

  return 0;

  int pnum = -1;
  int pid;
  for (int k = 0; k < processes.size(); k++) {
    pid = fork();
    if (pid == 0) {
      pnum = k;
      break;
    }
  }

  // clean up stuff TODO
}
