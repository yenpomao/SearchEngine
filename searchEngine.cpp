#include "searchEngine.h"

using namespace std;

const int EXPECT_ARG = 5;
const char *DATA_FLAG = "-d";
const char *K_LINES_ARG = "-k";
const int INITIAL_LINES_TO_SHOW = 3;
const int INDEX_ENTRIES_TO_SHOW = 5;

bool parseArguments(int argc, char **argv, string &dataFilePath, int &k)
{
    if (argc != EXPECT_ARG || strcmp(DATA_FLAG, argv[1]) != 0 || strcmp(K_LINES_ARG, argv[3]) != 0)
    {
        cout << "Usage: " << argv[0] << " -d <data_file_path> -k <num_results>" << endl;
        return false;
    }
    dataFilePath = argv[2];
    k = atoi(argv[4]);
    if (k <= 0)
    {
        cout << "Error number the results (k) must be positive integer " << endl;
        return false;
    }

    return true;
}

vector<string> loadDocumentLines(const string &filePath)
{
    vector<string> lines;
    ifstream file(filePath);
    if (!file.is_open())
    {
        cout << "Error file open " << filePath << endl;
        return lines;
    }

    string line;
    while (getline(file, line))
    {
        lines.push_back(line);
    }

    file.close();
    return lines;
}

map<string, set<int>> buildInvertedIndex(const vector<string> &documentLines)
{
    map<string, set<int>> invertedIndex;
    for (int i = 0; i < documentLines.size(); ++i)
    {
        istringstream iss(documentLines[i]);
        string word;
        while (iss >> word)
        {
            if (!word.empty())
            {
                transform(word.begin(), word.end(), word.begin(), [](unsigned char c)
                          { return tolower(c); });
                invertedIndex[word].insert(i);
            }
        }
    }

    return invertedIndex;
}

void performSearch(const map<string, set<int>> &invertedIndex, const vector<string> &documentLines, int k)
{
    while (true)
    {
        cout << "Enter the word to search or (exit) " << endl;
        string query;
        cin >> query;
        transform(query.begin(), query.end(), query.begin(), [](unsigned char c)
                  { return tolower(c); });

        if (query == "exit")
        {
            break;
        }

        auto it = invertedIndex.find(query);
        if (it == invertedIndex.end())
        {
            cout << "Find nothing" << endl;
        }
        else
        {
            int shown = 0;
            for (int lineNum : it->second)
            {
                if (shown > k)
                {
                    break;
                }
                cout << "[" << lineNum << "] " << documentLines[lineNum] << endl;
                shown++;
            }
        }
    }
}

int main(int argc, char **argv)
{
    string dataFilePath;
    int k = 0;

    if (!parseArguments(argc, argv, dataFilePath, k))
    {
        return -1;
    }

    cout << "Please wait" << endl;

    int linecounter = 0;
    int maxLength = -1;
    if (!read_size(&linecounter, &maxLength, dataFilePath.c_str()))
    {
        cout << "Error reading file size" << endl;
        return -1;
    }

    cout << "Lines in file: " << linecounter << endl;
    cout << "Max line length: " << maxLength << endl;

    vector<string> documentLines = loadDocumentLines(dataFilePath);
    if (documentLines.empty())
    {
        return -1;
    }
    cout << "Loaded " << documentLines.size() << " lines." << endl;

    for (int i = 0; i < min(INITIAL_LINES_TO_SHOW, (int)documentLines.size()); ++i)
    {
        cout << "[" << i << "]: " << documentLines[i] << endl;
    }

    map<string, set<int>> invertedIndex = buildInvertedIndex(documentLines);

    cout << " Words in index: ";
    int count = 0;
    for (const auto &entry : invertedIndex)
    {
        cout << entry.first << ": appears in " << entry.second.size() << " lines" << endl;
        if (++count >= INDEX_ENTRIES_TO_SHOW)
        {
            break;
        }
    }

    performSearch(invertedIndex, documentLines, k);
}