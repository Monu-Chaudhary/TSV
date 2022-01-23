/**
 * A simple program to print tsv file based on filter and column number or column name.
 *
 * Copyright (C) 2021 chaudhm3@miamioh.edu
 */

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <utility>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>

using namespace std;
using namespace std::string_literals;

using DataList = std::vector<std::vector<std::string>>;

/**
 * Prints the result in the specified format.
 *
 * @param vector of string containing the column index to be printed
 * @param Datalist a vector of vector containing strings which stores
 * the rows and columns of the TSV file.
 * @param filter a value by which the first column is filtered.
 * @return void.
 */
void evaluate(std::vector<std::string> vec, DataList data,
              std::string filter) {
    for (size_t i = 0; i < data.size(); i++) {
        // Print the tsv file's column data based on the filter applied
        if ((filter != "" &&
        data[i][stoi(vec[0])].find(filter) != std::string::npos)
        || i == 0) {
            for (size_t j = 0; j < vec.size(); j++) {
                    cout << data[i][stoi(vec[j])];
                    if (j != vec.size() - 1) cout << "\t";
            }
            cout << "\n";
        } else if (filter == "") {
            for (size_t j = 0; j < vec.size(); j++) {
                cout << data[i][stoi(vec[j])];
                if (j != vec.size() - 1) cout << "\t";
            }
            cout << "\n";
        }
    }
}

/**
 * Splits a line and returns the results in the form of a vector
 * of a vector.
 *
 * @param istream a TSV file open to be read.
 * @return vector of a vector containing strings.
 */
DataList split(std::istream& is) {
    DataList data;
    std::vector<std::string> vec;
    // Read each line of the tsv file
    for (std::string line; std::getline(is, line);) {
        // Ignore if the line read is empty
        if (line != "") {
            std::istringstream ifs(line);
            for (std::string value; ifs >> std::quoted(value);) {
                // Insert each column of each row into a vector
                vec.push_back(value);
            }
            // Insert each row of tsv file as a vector into another vector
            data.push_back(vec);
            int i = vec.size();
            vec.erase(vec.begin(), vec.begin() + i);
        }
    }
    return data;
}

/**
 * Checks if a filter argument exists and returns the filter value.
 *
 * @param argv check if the third argument is a filter argument.
 * If true then return the filter value.
 * @return the filter value
 */
std::string filter(char *argv[]) {
    std::string filter_value = "";
    std::string filter = argv[2];
    if (filter == "--filter") {
        filter_value = argv[3];
    }
    return filter_value;
}

/**
 * Create a vector of column number passed as an argument by the user.
 *
 * @param argc the maximum size of the argument passed.
 * @param argv to determine the starting index of the column number in the argument.
 * It is either 3 or 5.
 * @return the vector of column numbers.
 */
std::vector<std::string> colnums_v(int argc, char *argv[]) {
    std::vector<std::string> col_nums;
    int num;
    std::string arg;
    // Check at which position the argument lies.
    if ((arg = argv[2]) == "--colnums") {
        num = 3;
    } else if ((arg = argv[4]) == "--colnums") {
        num = 5;
    }
    std::string col_num;
    // Loop through the index of column number in
    // the argument vector to the end.
    for (int i = num; i < argc; i++) {
        col_num = argv[i];
        // Insert the column number into the vector col_nums
        col_nums.push_back(col_num);
    }
    return col_nums;
}


/**
 * Create a vector of column number corresponding to the column name passed as an argument by the user.
 *
 * @param argc the maximum size of the argument passed.
 * @param argv to determine the starting index of the column name in argument.
 * It is either 3 or 5.
 * @return the vector of column numbers.
 */
std::vector<std::string> cols_v(int argc, char *argv[], DataList data) {
    std::vector<std::string> col_nums;
    int num;
    std::string arg;
    // Check at which position the argument lies.
    if ((arg = argv[2]) == "--cols") {
        num = 3;
    } else if ((arg = argv[4]) == "--cols") {
        num = 5;
    }
    std::string col;
    // Loop through the index of column name in
    // the argument vector to the end.
    for (int i = num; i < argc; i++) {
        col = argv[i];
        // Find the corresponding column number for the column name
        for (size_t i = 0; i < data.size(); i++) {
            if (col == data[0][i]) {
                // Insert the column number into the vector col_nums
                col_nums.push_back(std::to_string(i));
            }
        }
    }
    return col_nums;
}

int main(int argc, char *argv[]) {
    std::string path = argv[1];
    std::string arg = argv[2];
    // Check to ensure we have a data file specified.
    if (argc < 2 || (path.find(".tsv") == std::string::npos)) {
        std::cout << "Specify data file as command-line argument.\n";
        return 1;
    }
    // Open the data file and ensure it is readable.
    std::ifstream is(argv[1]);
    if (!is.good()) {
        std::cout << "Error reading data from " << argv[1] << std::endl;
        return 2;
    }
    // Call the split function which reads from the tsv file
    const auto result = split(is);

    // Get the filter value from the argument
    std::string filter_value;
    if (arg == "--filter") {
        filter_value = filter(argv);
        arg = argv[4];
    }
    vector<std::string> col_nums;
    // Check if the argument is columns or cols
    if (arg == "--colnums") col_nums = colnums_v(argc, argv);
    else
        col_nums = cols_v(argc, argv, result);

    // Output based on the colnums
    evaluate(col_nums, result, filter_value);
    return 0;
}

// End of source code
