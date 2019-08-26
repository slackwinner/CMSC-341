// File: Sally.cpp
// Project: CMSC 341 Fall 2018 Project 2
// Author: Dane Magbuhos
// Date: 10/07/18
// Description: Implementation of member functions of Sally Forth interpreter
//

#include <iostream>
#include <string>
#include <list>
#include <stack>
#include <stdexcept>
#include <cstdlib>

using namespace std ;

#include "Sally.h"


// Basic Token constructor. Just assigns values.
//
Token::Token(TokenKind kind, int val, string txt) {
  m_kind = kind ;
  m_value = val ;
  m_text = txt ;
}


// Basic SymTabEntry constructor. Just assigns values.
//
SymTabEntry::SymTabEntry(TokenKind kind, int val, operation_t fptr) {
  m_kind   = kind ;
  m_value  = val ;
  m_dothis = fptr ;
}

// Constructor for Sally Forth interpreter.
// Adds built-in functions to the symbol table.
//
Sally::Sally(istream& input_stream) :
  istrm(input_stream)  // use member initializer to bind reference
{

  symtab["DUMP"]    =  SymTabEntry(KEYWORD,0,&doDUMP) ;

  // Basic Operations
  symtab["+"]    =  SymTabEntry(KEYWORD,0,&doPlus) ;
  symtab["-"]    =  SymTabEntry(KEYWORD,0,&doMinus) ;
  symtab["*"]    =  SymTabEntry(KEYWORD,0,&doTimes) ;
  symtab["/"]    =  SymTabEntry(KEYWORD,0,&doDivide) ;
  symtab["%"]    =  SymTabEntry(KEYWORD,0,&doMod) ;
  symtab["NEG"]  =  SymTabEntry(KEYWORD,0,&doNEG) ;

  // Helper Operations
  symtab["."]    =  SymTabEntry(KEYWORD,0,&doDot) ;
  symtab["SP"]   =  SymTabEntry(KEYWORD,0,&doSP) ;
  symtab["CR"]   =  SymTabEntry(KEYWORD,0,&doCR) ;

  // Stack Operations
  symtab["DUP"]   =  SymTabEntry(KEYWORD,0,&doDup) ;
  symtab["DROP"]  =  SymTabEntry(KEYWORD,0,&doDrop) ;
  symtab["SWAP"]  =  SymTabEntry(KEYWORD,0,&doSwap) ;
  symtab["ROT"]   =  SymTabEntry(KEYWORD,0,&doRot) ;

  // Variable Operations
  symtab["SET"]   =  SymTabEntry(KEYWORD,0,&doSet) ;
  symtab["@"]     =  SymTabEntry(KEYWORD,0,&getValue) ;
  symtab["!"]     =  SymTabEntry(KEYWORD,0,&storeValue) ;

  // Comparison Operations
  symtab["<"]    =  SymTabEntry(KEYWORD,0,&doLessThan) ;
  symtab["<="]   =  SymTabEntry(KEYWORD,0,&doLessThanOrEqual) ;
  symtab["=="]   =  SymTabEntry(KEYWORD,0,&doEqual) ;
  symtab["!="]   =  SymTabEntry(KEYWORD,0,&doNotEqual) ;
  symtab[">="]   =  SymTabEntry(KEYWORD,0,&doGreaterThanOrEqual) ;
  symtab[">"]    =  SymTabEntry(KEYWORD,0,&doGreaterThan) ;

  // Logic Operations
  symtab["AND"]   =  SymTabEntry(KEYWORD,0,&doAnd) ;
  symtab["OR"]    =  SymTabEntry(KEYWORD,0,&doOr) ;
  symtab["NOT"]   =  SymTabEntry(KEYWORD,0,&doNot) ;

  // If Statements
  symtab["IFTHEN"] =  SymTabEntry(KEYWORD,0,&doIfThen) ;
  symtab["ELSE"]   =  SymTabEntry(KEYWORD,0,&doElse) ;
  symtab["ENDIF"]  =  SymTabEntry(KEYWORD,0,&doEndIf) ;

  // Loop Construct
  symtab["DO"]   =  SymTabEntry(KEYWORD,0,&doLoop) ;
  symtab["UNTIL"]   =  SymTabEntry(KEYWORD,0,&doUntil) ;

}

// This function should be called when tkBuffer is empty.
// It adds tokens to tkBuffer.
//
// This function returns when an empty line was entered
// or if the end-of-file has been reached.
//
// This function returns false when the end-of-file was encountered.
//
// Processing done by fillBuffer()
//   - detects and ignores comments.
//   - detects string literals and combines as 1 token
//   - detects base 10 numbers
//
//
bool Sally::fillBuffer() {
  string line ;     // single line of input
  int pos ;         // current position in the line
  int len ;         // # of char in current token
  long int n ;      // int value of token
  char *endPtr ;    // used with strtol()


  while(true) {    // keep reading until empty line read or eof

    // get one line from standard in
    //
    getline(istrm, line) ;

    // if "normal" empty line encountered, return to mainLoop
    //
    if ( line.empty() && !istrm.eof() ) {
      return true ;
    }

    // if eof encountered, return to mainLoop, but say no more
    // input available
    //
    if ( istrm.eof() )  {
      return false ;
    }


    // Process line read

    pos = 0 ;                      // start from the beginning

    // skip over initial spaces & tabs
    //
    while( line[pos] != '\0' && (line[pos] == ' ' || line[pos] == '\t') ) {
      pos++ ;
    }

    // Keep going until end of line
    //
    while (line[pos] != '\0') {

      // is it a comment?? skip rest of line.
      //
      if (line[pos] == '/' && line[pos+1] == '/') break ;

      // is it a string literal?
      //
      if (line[pos] == '.' && line[pos+1] == '"') {

	pos += 2 ;  // skip over the ."
	len = 0 ;   // track length of literal

	// look for matching quote or end of line
	//
	while(line[pos+len] != '\0' && line[pos+len] != '"') {
	  len++ ;
	}

	// make new string with characters from
	// line[pos] to line[pos+len-1]
	string literal(line,pos,len) ;  // copy from pos for len chars

	// Add to token list
	//
	tkBuffer.push_back( Token(STRING,0,literal) ) ;

	// Different update if end reached or " found
	//
	if (line[pos+len] == '\0') {
	  pos = pos + len ;
	} else {
	  pos = pos + len + 1 ;
	}

      } else {  // otherwise "normal" token

	len = 0 ;  // track length of token

	// line[pos] should be an non-white space character
	// look for end of line or space or tab
	//
	while(line[pos+len] != '\0' && line[pos+len] != ' ' && line[pos+len] != '\t') {
	  len++ ;
	}

	string literal(line,pos,len) ;   // copy form pos for len chars
	pos = pos + len ;

	// Try to convert to a number
	//
	n = strtol(literal.c_str(), &endPtr, 10) ;

	if (*endPtr == '\0') {
	  tkBuffer.push_back( Token(INTEGER,n,literal) ) ;
	} else {
	  tkBuffer.push_back( Token(UNKNOWN,0,literal) ) ;
	}
      }

      // skip over trailing spaces & tabs
      //
      while( line[pos] != '\0' && (line[pos] == ' ' || line[pos] == '\t') ) {
	pos++ ;
      }

    }
  }
}

// Return next token from tkBuffer.
// Call fillBuffer() if needed.
// Checks for end-of-file and throws exception
//
Token Sally::nextToken() {
  Token tk ;
  bool more = true ;

  while(more && tkBuffer.empty() ) {
    more = fillBuffer() ;
  }

  if ( !more && tkBuffer.empty() ) {
    throw EOProgram("End of Program") ;
  }

  tk = tkBuffer.front() ;
  tkBuffer.pop_front() ;
  return tk ;
}


// The main interpreter loop of the Sally Forth interpreter.
// It gets a token and either push the token onto the parameter
// stack or looks for it in the symbol table.
//
//
void Sally::mainLoop() {

  Token tk ;
  map<string,SymTabEntry>::iterator it ;

  try {
    while( 1 ) {
      tk = nextToken() ;

      if (tk.m_kind == INTEGER || tk.m_kind == STRING) {

	// if INTEGER or STRING just push onto stack
	params.push(tk) ;

      } else {
	it = symtab.find(tk.m_text) ;

	if ( it == symtab.end() )  {   // not in symtab

	  params.push(tk) ;

	} else if (it->second.m_kind == KEYWORD)  {

	  // invoke the function for this operation
	  //
	  it->second.m_dothis(this) ;

	} else if (it->second.m_kind == VARIABLE) {

	  // variables are pushed as tokens
	  //
	  tk.m_kind = VARIABLE ;
	  params.push(tk) ;

	} else {

	  // default action
	  //
	  params.push(tk) ;

	}
      }
    }

  } catch (EOProgram& e) {

    cerr << "End of Program \n" ;
    if ( params.size() == 0 ) {
      cerr << "Parameter stack empty.\n" ;
    } else {
      cerr << "Parameter stack has " << params.size() << " token(s).\n" ;
    }

  } catch (out_of_range& e) {

    cerr << "Parameter stack underflow??\n" ;
    cerr << "Problem: " << e.what() << endl ;

  } catch (...) {

    cerr << "Unexpected exception caught\n" ;

  }
}

// -------------------------------------------------------

// Performs the addition operation
void Sally::doPlus(Sally *Sptr) {
  Token p1, p2 ;

  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for +.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  int answer = p2.m_value + p1.m_value ;
  Sptr->params.push( Token(INTEGER, answer, "") ) ;
}

// Performs the subtraction operation
void Sally::doMinus(Sally *Sptr) {
  Token p1, p2 ;

  if (Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for -.") ;

  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  int answer = p2.m_value - p1.m_value ;
  Sptr->params.push( Token(INTEGER, answer, "") ) ;
}

// Performs the multiplication operation
void Sally::doTimes(Sally *Sptr) {
  Token p1, p2 ;

  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for *.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  int answer = p2.m_value * p1.m_value ;
  Sptr->params.push( Token(INTEGER, answer, "") ) ;
}

// Performs the division operation 
void Sally::doDivide(Sally *Sptr) {
  Token p1, p2 ;

  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for /.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  int answer = p2.m_value / p1.m_value ;
  Sptr->params.push( Token(INTEGER, answer, "") ) ;
}

// Performs modulo division operation
void Sally::doMod(Sally *Sptr) {

  Token p1, p2 ;

  if ( Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for %.") ;
  }

  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  int answer = p2.m_value % p1.m_value ;
  Sptr->params.push( Token(INTEGER, answer, "") ) ;
}

// Performs the negative operation
void Sally::doNEG(Sally *Sptr) {

  if (Sptr->params.size() < 1 ) {
    throw out_of_range("Need one parameter for NEG.") ;
  }
  Token p ;

  p = Sptr->params.top() ;
  Sptr->params.pop() ;
  Sptr->params.push( Token(INTEGER, -p.m_value, "") ) ;
}

// Handles the dot operation
void Sally::doDot(Sally *Sptr) {

  Token p ;
  if ( Sptr->params.size() < 1 ) {
    throw out_of_range("Need one parameter for .") ;
  }

  p = Sptr->params.top() ;
  Sptr->params.pop() ;

  if (p.m_kind == INTEGER) {
    cout << p.m_value ;
  } else {
    cout << p.m_text ;
  }
}

// Adds space for output
void Sally::doSP(Sally *Sptr) {
  cout << " " ;
}

// Adds ending marking point of output
void Sally::doCR(Sally *Sptr) {
  cout << endl ;
}


// Duplicates top item in stack
void Sally::doDup(Sally *Sptr) {

  Token p1;

  if (Sptr->params.size() < 1 ) {
    throw out_of_range("Need one parameter for DUP.") ;
  }

  p1 = Sptr->params.top() ;
  Sptr->params.push(p1) ;
}

// Removes top item in stack
void Sally::doDrop(Sally *Sptr) {

  if (Sptr->params.size() == 0 ) {
    throw out_of_range("Need one parameter for DROP.") ;
  }

  Sptr->params.pop() ;

}

// Swaps top two item's positions
void Sally::doSwap(Sally *Sptr) {

  Token p1 , p2 ;

  if (Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for SWAP.") ;
  }

  // Grabs and removes first element
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;

  // Grabs and removes second element
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;

  // Switches positions of elements
  Sptr->params.push(p1) ;
  Sptr->params.push(p2) ;

}

// Swaps top three items' positions
void Sally::doRot(Sally *Sptr) {

  Token p1 , p2 , p3 ;

  if (Sptr->params.size() < 3 ) {
    throw out_of_range("Need three parameters for ROT.") ;
  }

  // Grabs and removes first element
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;

  // Grabs and removes second element
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;

  // Grabs and removes third element
  p3 = Sptr->params.top();
  Sptr->params.pop() ;

  // Switches positions of elements
  Sptr->params.push(p2) ;
  Sptr->params.push(p1) ;
  Sptr->params.push(p3) ;

}

// Stores variable in symbol table (Sets only int variables)
void Sally::doSet(Sally *Sptr) {

  Token variable , p2 ;
  bool duplicate = false ; // an indicator for finding a duplicate

  if(Sptr->params.size() < 2){
    throw out_of_range("Need two parameters for SET.") ;
  }

  // Grabs and removes variable element
  variable = Sptr->params.top() ;
  Sptr->params.pop() ;

  // Grabs and removes numeric element
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;

  // Searches through symbol table to find duplicate variable name
  map<string,SymTabEntry>:: iterator value ;

  for(value = Sptr->symtab.begin(); value != Sptr->symtab.end(); value++){

    // Outputs error message if duplicate is found
    if(value->first == variable.m_text){
      cout << "Variable name is currently being used." << endl ;
      duplicate = true ;
    }

  }

  // Sets variable name with specified value
  if(value == Sptr->symtab.end() && (duplicate == false)){

    Sptr->symtab[variable.m_text] = SymTabEntry(VARIABLE,p2.m_value) ;

  }
}

// Grabs value for variable
void Sally::getValue(Sally *Sptr) {

  Token search ;
  bool execution = false ; // an indicator for getting value
  int num ;               // stores extracted number

  if(Sptr->params.size() < 1){
    throw out_of_range("Need one parameter for @.");
  }

  // Grabs and removes variable name that's being looked up
  search = Sptr->params.top() ;
  Sptr->params.pop() ;

  // Searches through symbol table to find variable
  map<string,SymTabEntry>:: iterator value ;

  for(value = Sptr->symtab.begin(); value != Sptr->symtab.end(); value++){

    // Extracts numeric value for specified value and adds value to stack
    if(value->first == search.m_text){

      search.m_value = value->second.m_value ;

      num = search.m_value ;

      Sptr->params.push(Token(INTEGER, num)) ;

      execution = true;
    }
  }

  // Outputs error message if variable was not found
  if((value == Sptr->symtab.end()) && (execution == false)){
    cout << "Variable not found." << endl ;

  }


}

// Stores value in variable that exists already
void Sally::storeValue(Sally *Sptr) {

  Token variable , p2;
  bool execution = false; // an indicator for a redefined variable

  if(Sptr->params.size() < 2){
    throw out_of_range("Need two parameters for !.");
  }

  // Grabs and removes variable name
  variable = Sptr->params.top() ;
  Sptr->params.pop() ;

  // Grabs and removes numeric value
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;

  // Searches through symbol table to find variable
  map<string,SymTabEntry>:: iterator value ;

  for(value = Sptr->symtab.begin(); value != Sptr->symtab.end(); value++){

    // Sets existing variable to new value
    if(value->first == variable.m_text){

      Sptr->symtab[variable.m_text] = SymTabEntry(VARIABLE,p2.m_value) ;

      execution = true ;
    }

  }

  // Outputs error message if existing variable was not found
  if((value == Sptr->symtab.end()) && (execution == false)){
    cout << "Variable does not exists." << endl ;
  }

}

// Handles less than operation
void Sally::doLessThan(Sally *Sptr) {

  Token p1, p2 ;
  int result = 0 ; // Default boolean value

  if (Sptr->params.size() < 2 ) {
    throw out_of_range("Need one parameter for <.") ;
  }

  // Grabs and removes first element
  p1 = Sptr->params.top() ;
  Sptr->params.pop();

  // Grabs and removes second element
  p2 = Sptr->params.top();
  Sptr->params.pop();

  // Performs comparison operation
  if(p2.m_value < p1.m_value){
    result = 1;
  }

  // Pushes result to stack
  Sptr->params.push(Token(INTEGER,result));

}

// Handles less than or equal to operation
void Sally::doLessThanOrEqual(Sally *Sptr) {

  Token p1, p2 ;
  int result = 0 ;   // Stores boolean result

  if (Sptr->params.size() < 2 ) {
    throw out_of_range("Need one parameter for <=.") ;
  }

  // Grabs and removes first element
  p1 = Sptr->params.top() ;
  Sptr->params.pop();

  // Grabs and removes second element
  p2 = Sptr->params.top();
  Sptr->params.pop();

  // Performs comparison operation
  if(p2.m_value <= p1.m_value){
    result = 1;
  }


  // Pushes result to stack
  Sptr->params.push(Token(INTEGER,result));

}

// Handles equal to operation
void Sally::doEqual(Sally *Sptr) {

  Token p1, p2 ;
  int result = 0 ;   // Default boolean value

  if (Sptr->params.size() < 2 ) {
    throw out_of_range("Need one parameter for ==.") ;
  }

  // Grabs and removes first element
  p1 = Sptr->params.top() ;
  Sptr->params.pop();

  // Grabs and removes second element
  p2 = Sptr->params.top();
  Sptr->params.pop();

  // Performs comparison operation
  if(p2.m_value == p1.m_value){
    result = 1;
  }

  // Pushes result to stack
  Sptr->params.push(Token(INTEGER,result));

}

// Handles not equal to operation
void Sally::doNotEqual(Sally *Sptr) {

  Token p1, p2;
  int result = 0 ; // Default boolean value

  if (Sptr->params.size() < 2 ) {
    throw out_of_range("Need one parameter for !=.") ;
  }

  // Grabs and removes first element
  p1 = Sptr->params.top() ;
  Sptr->params.pop();

  // Grabs and removes second element
  p2 = Sptr->params.top();
  Sptr->params.pop();

  // Performs comparison operation
  if(p2.m_value != p1.m_value){
    result = 1;
  }

  // Pushes result to stack
  Sptr->params.push(Token(INTEGER,result));

}

// Handles greater than or equal to operation
void Sally::doGreaterThanOrEqual(Sally *Sptr) {

  Token p1, p2;
  int result = 0;   // Default boolean value

  if (Sptr->params.size() < 2 ) {
    throw out_of_range("Need one parameter for >=.") ;
  }

  // Grabs and removes first element
  p1 = Sptr->params.top() ;
  Sptr->params.pop();

  // Grabs and removes second element
  p2 = Sptr->params.top();
  Sptr->params.pop();

  // Performs comparison operation
  if(p2.m_value >= p1.m_value){
    result = 1;
  }

  // Pushes result to stack
  Sptr->params.push(Token(INTEGER,result));

}

// Handles greater than operation
void Sally::doGreaterThan(Sally *Sptr) {

  Token p1, p2;
  int result = 0;   // Default boolean value

  if (Sptr->params.size() < 2 ) {
    throw out_of_range("Need one parameter for >.") ;
  }

  // Grabs and removes first element
  p1 = Sptr->params.top() ;
  Sptr->params.pop();

  // Grabs and removes second element
  p2 = Sptr->params.top();
  Sptr->params.pop();

  // Performs comparison operation
  if(p2.m_value > p1.m_value){
    result = 1;
  }

  // Pushes result to stack
  Sptr->params.push(Token(INTEGER,result));

}


// Handles the logic and operation
void Sally::doAnd(Sally *Sptr) {

  Token p1, p2 ;
  int trueResult  = 1 ; // Denotes true status
  int falseResult = 0 ; // Denotes false status

  if (Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for AND.") ;
  }

  // Grabs and removes first element
  p1 = Sptr->params.top() ;
  Sptr->params.pop();

  // Grabs and removes second element
  p2 = Sptr->params.top();
  Sptr->params.pop();

  // Performs AND operation and pushes result to stack
  if((p1.m_value == trueResult) && (p2.m_value == trueResult))
    Sptr->params.push(Token(INTEGER, trueResult)) ;
  else
    Sptr->params.push(Token(INTEGER, falseResult)) ;

}

// Handles the logic or operation
void Sally::doOr(Sally *Sptr) {

  Token p1, p2 ;
  int trueResult  = 1 ; // Denotes true status
  int falseResult = 0 ; // Denotes false status

  if (Sptr->params.size() < 2 ) {
    throw out_of_range("Need two parameters for OR.") ;
  }

  // Grabs and removes first element
  p1 = Sptr->params.top() ;
  Sptr->params.pop();

  // Grabs and removes second element
  p2 = Sptr->params.top();
  Sptr->params.pop();

  // Performs OR operation and pushes result to stack
  if((p1.m_value == trueResult) || (p2.m_value == trueResult))
    Sptr->params.push(Token(INTEGER, trueResult)) ;
  else
    Sptr->params.push(Token(INTEGER, falseResult)) ;

}

// Handles the logic not operation
void Sally::doNot(Sally *Sptr) {

  Token p1 ;
  int trueResult  = 1 ; // Denotes true status
  int falseResult = 0 ; // Denotes false status

  if (Sptr->params.size() < 1 ) {
    throw out_of_range("Need one parameter for NOT.") ;
  }

  // Grabs and removes first element
  p1 = Sptr->params.top() ;
  Sptr->params.pop();

  // Performs NOT operation and pushes result to stack
  if(p1.m_value == trueResult){

    Sptr->params.push(Token(INTEGER, falseResult)) ;
  }

  else if(p1.m_value == falseResult){

    Sptr->params.push(Token(INTEGER, trueResult)) ;
  }

}

// Handles the IF Then operation
void Sally::doIfThen(Sally *Sptr) {

  Token p1 ;
  string symbol ;       // Stores token's symbol
  int trueResult  = 1 ; // Denotes a true result
  int falseResult = 0 ; // Denotes a false result
  int counter     = 1 ; // Counts amount of IFTHEN symbols

  if(Sptr->params.size() < 1) {
    throw out_of_range("Need one parameter for IFTHEN");
  }

  // Grabs and removes first element
  p1 = Sptr->params.top();
  Sptr->params.pop();

  // Exits out of function
  if (p1.m_value == trueResult){
    return ;
  }

  // Skips over nested IF THEN conditions
  else if (p1.m_value == falseResult){

    while (counter > 0){

      symbol = Sptr->nextToken().m_text;

      if(symbol == "IFTHEN"){

	counter++ ;
      }

      if(symbol == "ELSE"){

	counter-- ;
      }

    }
  }
}

// Handles the Else operation
void Sally::doElse(Sally *Sptr) {

  Token p1 ;
  string symbol ;       // Stores token's symbol
  int trueResult  = 1 ; // Denotes a true result
  int falseResult = 0 ; // Denotes a false result
  int counter     = 1 ; // Counts amount of IFTHEN symbols

  // Exits out of function
  if (p1.m_value == trueResult){
    return ;
  }
  // Skips over conditions nested in Else if condition is false
  if (p1.m_value == falseResult){

    while (counter > 0){
      symbol = Sptr->nextToken().m_text;

      if(symbol == "ELSE"){

	counter++ ;
      }

      if(symbol == "ENDIF"){

	counter-- ;
      }

    }
  }

}

// Handles the End If operation
void Sally::doEndIf(Sally *Sptr) {

  return ;
}

// Handles the do while operation
void Sally::doLoop(Sally *Sptr) {

  Token p1 = Sptr -> nextToken() ;

  // Clears out list in case previous loop was used
  Sptr -> storageA.clear();

  // Executes loop until SYMBOL "UNTIL" is reached
  while(p1.m_text != "UNTIL"){

    // Stores token into token list
    Sptr -> storageA.push_back(p1) ;

    // Grabs token
    p1 = Sptr -> nextToken() ;

  }
  // Pushes "UNTIL" token in order to invoke doUntil function
  Sptr -> storageA.push_back(p1) ;

  // Sets the storage list as tkBuffer
  Sptr-> tkBuffer = Sptr -> storageA;
}

// Handles the do while operation
void Sally::doUntil(Sally *Sptr) {

  Token entry = Sptr -> params.top() ;
  Sptr -> params.pop() ;

  list<Token>:: iterator it ;

  // Executes the loop's operation until entry is false
  if(entry.m_value == 0){

    // Iterates through saved tokens in list to perform execution
    for(it = Sptr->storageA.begin(); it != Sptr->storageA.end(); it++){

      entry = *it ;
      Sptr -> tkBuffer.push_back(entry) ;

    }
  }
}

// Debugger
void Sally::doDUMP(Sally *Sptr) {

  map<string,SymTabEntry>:: iterator it;

  for(it = Sptr->symtab.begin(); it != Sptr->symtab.end(); ++it){
    cout << it->first << " = " << it->second.m_value << endl;

  }

}
