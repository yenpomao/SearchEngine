#include "readInput.h"

using namespace std;

bool read_size(int *linecounter, int *maxLength, const string &docfile)
{
    std::ifstream file(docfile);

    // Check if the file was successfully opened
    if (!file.is_open())
    {
        std::cout << "Failed to open file: " << docfile << std::endl;
        return false;
    }

    *linecounter = 0;
    *maxLength = 0;
    string line;

    while (getline(file, line))
    {
        (*linecounter)++;

        *maxLength = max(*maxLength, static_cast<int>(line.length()));
    }

    file.close();
    return true;
}
