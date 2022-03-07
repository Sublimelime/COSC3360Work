// VERTEXPROCESS_H
#ifndef VERTEXPROCESS_H
#define VERTEXPROCESS_H

#include <string>
#include <vector>

class VertexProcess {
public:
  // ID of the process, created during forking
  int id;

  // Determines whether the process is an initial input that will input words to
  // the system
  bool isInitialInput;

  // determines whether the process is an output that will be printing
  bool isFinalOutput;

  // holds a list of pipe ids that the process should input from and output onto
  // this is intended to be mapped 1:1 to `pipes` vector of int[2]'s in main
  // file, with inputs[1] corresponding to pipes[1], etc.
  std::vector<int> inputs = std::vector<int>();
  std::vector<int> outputs = std::vector<int>();

  // holds the word the process is supposed to initially write, if
  // isInitialInput is true.
  std::string myWord;

  VertexProcess(int id, bool isInput, bool isOutput)
      : id(id), isInitialInput(isInput), isFinalOutput(isOutput) {}

  // Configures `inputs` depending on provided matrix.
  void determineInputs(const std::vector<std::vector<int>> &matrix) {
    for (long unsigned int i = 0; i < matrix.size(); i++) {
      if (matrix[i][id] == 1)
        inputs.push_back(i);
    }
  }

  // Configures `outputs` depending on provided matrix.
  void determineOutputs(const std::vector<std::vector<int>> &matrix) {
    for (long unsigned int i = 0; i < matrix.size(); i++) {
      if (matrix[id][i] == 1)
        outputs.push_back(i);
    }
  }
};

#endif // VERTEXPROCESS_H
