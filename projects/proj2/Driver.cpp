// File: Driver.cpp
// Project: CMSC 341 Fall 2018 Project 2
// Author: Dane Magbuhos
// E-mail: mag4@umbc.edu

// Description: Simple driver program to call the Sally Forth interpreter
// This version accepts user input for filename of Sally Forth
// source code.
//

#include <iostream>
#include <fstream>
using namespace std ;

#include "Sally.h"

int main() {
  string fname ;

  cout << "Enter file name: " ;
  cin >> fname ;
  ifstream ifile(fname.c_str()) ;

  Sally S(ifile) ;

  S.mainLoop() ;

  ifile.close() ;
  return 0 ;
}
