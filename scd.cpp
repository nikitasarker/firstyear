#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <vector>
// do not alter the header inclusion
// do not include any additional headers
// do not use using namespace std;

// do not alter the function declarations
// do not add any other functions to the program
// see below for more information about these functions

bool valid_sudoku(const std::vector<int>& g);
bool check_sequence(const std::vector<int>& v);
int mfind(int n, const std::vector<int>& v);

void get_row(int r, const std::vector<int>& in, std::vector<int>& out);
void get_col(int c, const std::vector<int>& in, std::vector<int>& out);
void get_subsq(int subs, const std::vector<int>& in, std::vector<int>& out);

int twod_to_oned(int row, int col, int rowlen);

// do not alter the main

int main(){

    std::vector<int> s;

    // we are going to represent the sudoku as a one dimensional vector
    // linearised by row, for instance the 4x4 sudoku:
    // 1 2 3 4
    // 4 1 2 76
    // 4 4 4 1
    // 9 4 1 -7
    // will be represented as:
    // 1 2 3 4 4 1 2 76 4 4 4 1 9 4 1 -7

    // beginning of block of code reading the sudoku from the text file
    std::string filename;

    std::cout << "please enter name of file containing the sudoku" << std::endl;
    std::cin >> filename;

    std::ifstream infile;
    infile.open(filename.c_str());

    if(!infile.is_open()){
        std::cout << "error, can't open input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    int tmp;

    while(infile >> tmp){
        s.push_back(tmp);
    }
    // end of block of code reading the sudoku from the text file
    // we now have the content of the sudoku in vector s

    int side = std::sqrt(s.size());
    // assuming it is a valid square grid in terms of size
    // (if it isn't, the program will not work properly, which is expected)

    // printing the content of the sudoku (as a square);
    // the sudoku is stored as a one-dimensional vector
    // but the vector can be indexed using two indices
    // by converting the indices of a two dimensional representation
    // into the corresponding one-dimensional index (using our function twod_to_oned)

    for(int i = 0; i < side; i++){
        for(int j = 0; j < side; j++){
            std::cout << s[twod_to_oned(i,j,side)] << " ";
        }
        std::cout << std::endl;
    }

    // calling the function checking if the sudoku is a valid one:

    bool valid = valid_sudoku(s);

    if(valid){
        std::cout << "valid" << std::endl;
    }
    else{
        std::cout << "not valid" << std::endl;
    }

    return 0;
}

// function twod_to_oned maps two-dimensional indices to a one-dimensional one
// it takes in input:
// - the two indices row and col of a two dimensional data structure
// - the length of the row, rowlen
// it returns:
// - the corresponding one-dimensional index

int twod_to_oned(int row, int col, int rowlen){
    return row*rowlen+col;
}

// function valid_sudoku
// takes in input:
// - a one dimensional vector representing a sudoku
// returns:
// - a boolean which is true if the sudoku is valid and false if it isn't valid

// write your code where indicated
// i.e. within the two remaining incomplete `for' loops
// do not alter anything else

bool valid_sudoku(const std::vector<int>& g){
    int side = std::sqrt(g.size());

    // for each row...
    for(int i = 0; i < side; i++){
        std::vector<int> row;
        get_row(i, g, row);

        if(!check_sequence(row)){
          return false;
        }
    }

    // for each column...
    for(int i = 0; i < side; i++){
        std::vector<int> column;
        get_col(i, g, column);

        if(!check_sequence(column)){
          return false;
        }
    }

    // for each subsquare...
   for(int i = 0; i < side; i++){
        std::vector<int> subsquare;
        get_subsq(i, g, subsquare);

        if(!check_sequence(subsquare)){
          return false;
        }
    }

    // if the program execution gets to here it means that
    // it hasn't executed a "return false" above
    // so the sudoku is valid:

    return true;
}


// function mfind
// takes in input:
// - a vector of integers v
// - an integer n
// returns:
// the index of the element n in v, if n is in v
// -1 if n is not in v
// for instance:
// v: 3 2 5; n: 2 | return: 1
// v: 3 1 2; n: -6 | return: -1
// v: 1 1 -12 1 5; n: 5 | return: 4

int mfind(int n, const std::vector<int>& v){
    for(int i=0;i<v.size();i++) {
      if(v[i]==n) {
        return i;
      }
    }
    return -1;
}

// function check_sequence
// takes in input:
// - a vector of integers v
// returns:
// - true if v contains all the numbers from 1 to v.size() and false otherwise
// for instance:
// v: 3 2 5 | return: false
// v: 3 1 2 | return: true
// v: 1 1 -12 1 5 | return: false
// note that check_sequence must (meaningfully) use function mfind
bool check_sequence(const std::vector<int>& v){
    for(int a=1;a<=v.size();a++) {
      if(mfind(a,v)==-1) {
        return false;
      }
    }
    return true;
}

// note that all the following functions must (meaningfully) use function twod_to_oned

// function get_row
// takes in input:
// - a row index r
// - a one dimensional vector `in' representing a sudoku
// provides in output:
// - a vector out containing the r-th row of in
// for example:
// r: 0; in: 1 2 3 4 4 1 2 76 4 4 4 1 9 4 1 -7 | out: 1 2 3 4
// r: 3; in: 1 2 3 4 4 1 2 76 4 4 4 1 9 4 1 -7 | out: 9 4 1 -7
void get_row(int r, const std::vector<int>& in, std::vector<int>& out){
    int row_length = std::sqrt(in.size());
    for(int column_index = 0; column_index < row_length; column_index++) {
      out.push_back(in[twod_to_oned(r, column_index, row_length)]);
    }
}

// function get_col
// like get_row but for columns
// for example:
// c: 0; in: 1 2 3 4 4 1 2 76 4 4 4 1 9 4 1 -7 | out: 1 4 4 9
void get_col(int c, const std::vector<int>& in, std::vector<int>& out){
    int col_length = std::sqrt(in.size());
    for(int row_index = 0; row_index < col_length; row_index++) {
      out.push_back(in[twod_to_oned(row_index, c, col_length)]);
    }
}


// function get_subsq
// like the two functions above but for subsquares
// we consider subsquares to be indexed from left to right
// and then from top to bottom
// for example:
// subs: 0; in: 1 2 3 4 4 1 2 76 4 4 4 1 9 4 1 -7 | out: 1 2 4 1
// subs: 1; in: 1 2 3 4 4 1 2 76 4 4 4 1 9 4 1 -7 | out: 3 4 2 76
// subs: 2; in: 1 2 3 4 4 1 2 76 4 4 4 1 9 4 1 -7 | out: 4 4 9 4
// subs: 3; in: 1 2 3 4 4 1 2 76 4 4 4 1 9 4 1 -7 | out: 4 1 1 -7
void get_subsq(int subs, const std::vector<int>& in, std::vector<int>& out) {
    int sq_length = std::sqrt(in.size()); //eg if 4x4, sq_length is 4
    int subsq_length = std::sqrt(sq_length); //eg show each subsq is 2x2

    int subs_row = (subs/subsq_length)*subsq_length;
    int subs_col = (subs%subsq_length)*subsq_length;

    for(int n=0;n<subsq_length;n++) {
      for(int i=0;i<subsq_length;i++) {
        out.push_back(in[twod_to_oned((subs_row+n), (subs_col+i), sq_length)]);
      }
    }
}
