#ifndef SEARCHENGINE_H // Include guard start
#define SEARCHENGINE_H

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "readInput.h"

bool parseArguments(int argc, char **argv, std::string &dataFilePath, int &k);

std::vector<std::string> loadDocumentLines(const std::string &filePath, int maxLength);

std::map<std::string, std::set<int>> buildInvertedIndex(const std::vector<std::string> &documentLines);

void performSearch(const std::map<std::string, std::set<int>> &invertedIndex,
                   const std::vector<std::string> &documentLines,
                   int k);

#endif // SEARCHENGINE_H // Include guard end