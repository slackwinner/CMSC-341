// File: HashTable.cpp
// Project: CMSC 341: Incremental Hashing
// Author: Dane Magbuhos
// Date: 12/12/18
//
// Description: This program deals utilizes a
//              hashing algorithm to store
//              a set of strings
//

#include <iostream>
#include <stdlib.h>
#include <stdexcept>
#include <string.h>

using namespace std;

#include "HashTable.h"
#include "primes.cpp"

HashTableObj::HashTableObj(int n, int prime, bool hashing){
    
  m_maxSize = 199999;
  m_minSize = 101;
  m_currentSize = 0;
    
  // Checks to see if size n surpasses max size
  if(n > m_maxSize)
    throw::out_of_range("Requested size is too large");
    
  // Checks to see if size is below min size
  else if(n < m_minSize)
    n = m_minSize;
    
  m_requestedSize = n;
    
  // Prime number has not been chosen
  if(hashing == false){
    m_TSize = roundUpPrime(m_requestedSize);
  }
    
  // Prime number has been chosen prior to execution
  else if(hashing == true){
    m_TSize = prime;
  }
  // Dynamically allocates hash table char array
  m_hashT = (char**)malloc(m_TSize * sizeof(char**));
    
  // Dynamically allocates hash table int array
  m_idxHolder = new int[m_TSize];
    
  // Pre-populates arrays with default values
  for(int i = 0; i < m_TSize; i++){
        
    m_hashT[i] = NULL;
    m_idxHolder[i] = 0;
  }
}

HashTableObj::~HashTableObj(){
    
  //Destructor: Uses the hash table's deconstructor
    
}


// Represents deleted item
char * const HashTable::DELETED = (char *) &DELETED;

// Constructor
HashTable::HashTable(int n){
    
  m_hashArrayIdx = 0;
  m_hashArraySize = 3;
  m_tableCount = 0;
  m_hashStat = 0;
    
  // Creates a new hashTable
  m_T[0] = new HashTableObj(n);
  m_T[1] = NULL;
  m_T[2] = NULL;
    
  // Indicates that one table is being used
  m_tableCount++;
}

// Destructor
HashTable::~HashTable(){
    
  // Deletes dynamically allocated char array
  for(int j = 0; j < m_hashArraySize; j++){
        
    // Checks to see if array index is not empty
    if(m_T[j] != NULL){
            
      // Deallocates items in table array
      for(int i = 0; i < m_T[j]->m_TSize; i++){
                
	if(m_T[j]->m_hashT[i] != DELETED){
	  free(m_T[j]->m_hashT[i]);
	  m_T[j]->m_hashT[i] = NULL;
	}
      }
            
      // Deallocates the hash array table
      free(m_T[j]->m_hashT);
      m_T[j]->m_hashT = NULL;
            
      // Deallocates the idx holder array from object
      delete [] m_T[j]->m_idxHolder;
      m_T[j]->m_idxHolder = NULL;
            
      // Deallocates hash table object
      delete m_T[j];
      m_T[j] = NULL;
            
    }
  }
}

// Copy Constructor
HashTable::HashTable(HashTable& other){
 
  m_hashStat = other.m_hashStat;
    
  /* // Combines the tables into one table
    if(m_hashStat > 0){
        mergeTable();
	}*/
    
  for(int j = 0; j < m_hashArraySize; j++){
        
    // Checks to see if array index is not empty
    if(m_T[j] != NULL){
            
      // Copies over all hashTable components
      for(int i = 0; i < m_T[j]->m_TSize; i++){
            
	m_T[j]->m_requestedSize = other.m_T[j]->m_requestedSize;
	m_T[j]->m_TSize = other.m_T[j]->m_TSize;
            
	// Creates array
	m_T[j]->m_hashT = (char**)malloc(m_T[j]->m_TSize * sizeof(char**));
            
	// Copies over hash table contents
	for(int i = 0; i < m_T[j]->m_TSize; i++){
	  m_T[j]->m_hashT[i] = strdup(other.m_T[j]->m_hashT[i]);
                
	}
      }
    }
  }
}

// Overloaded Assignment Operator
const HashTable& HashTable::operator= (HashTable &rhs){
    
  /*  m_hashStat = rhs.m_hashStat;
    
    // Combines the tables into one table
    if(m_hashStat > 0){
        mergeTable();
	}*/
    
  for(int j = 0; j < m_hashArraySize; j++){
        
    // Checks to see if array index is not empty
    if(m_T[j] != NULL){
            
      // Copies over all hashTable components
      for(int i = 0; i < m_T[j]->m_TSize; i++){
                
	// Resets all member variables
	free(m_T[j]->m_hashT);
	m_T[j]->m_requestedSize = NULL;
	m_T[j]->m_TSize = NULL;
                
	// Copies member variables
	m_T[j]->m_requestedSize = rhs.m_T[j]->m_requestedSize;
	m_T[j]->m_TSize = rhs.m_T[j]->m_TSize;
                
	// Creates array
	m_T[j]->m_hashT = (char**)malloc(m_T[j]->m_TSize * sizeof(char**));
                
	// Copies content from array
	for(int i = 0; i < m_T[j]->m_TSize; i++){
	  m_T[j]->m_hashT[i] = strdup(rhs.m_T[j]->m_hashT[i]);
                    
	}
      }
    }
  }
    
  return *this;
}

// Hashing Algorithm
unsigned int HashTable::hashCode(const char *str){
    
  unsigned int val = 0 ;
  const unsigned int thirtyThree = 33 ;  // magic number from textbook
    
  int i = 0 ;
  while (str[i] != '\0'){
    val = val * thirtyThree + str[i] ;
    i++ ;
  }
  return val ;
}

// Insert Function
void HashTable::insert(const char *str){
   
  int index = 0;
  int initialIdx = index;
  int pivotIdx = 0;
  int probeCount = 1;
  int maxProbe = 10;
  int halfBatch = 0.5;

  bool duplicate = false;
  bool insertedValue = false;
    
  // Indicates that we are traversing through first table
  if(m_hashStat == 0){
        
    index = hashCode(str) % m_T[m_hashArrayIdx]->m_TSize;
    initialIdx = index;
        
    // Traverses through hashTable
    while((insertedValue == false) && (duplicate == false)){
            
      probeCount++;
             
      // Only compares string in hash table if position is filled
      if(m_T[m_hashArrayIdx]->m_hashT[index] != NULL){
                
	// Finds duplicate within cluster
	if(strcmp(m_T[m_hashArrayIdx]->m_hashT[index], str) == 0){
	  duplicate = true;
	}
      }
            
      if(duplicate == false){
                
	// Determines if slot is avaliable for insertion
	if((m_T[m_hashArrayIdx]->m_hashT[index] == NULL) ||
	   (strcmp(m_T[m_hashArrayIdx]->m_hashT[index], DELETED) == 0)){
                    
	  m_T[m_hashArrayIdx]->m_hashT[index] = strdup(str);
	  m_T[m_hashArrayIdx]->m_idxHolder[index] = initialIdx;
	  m_T[m_hashArrayIdx]->m_currentSize++;
	  insertedValue = true;
	}
      }
             
      index = (index + 1) % m_T[m_hashArrayIdx]->m_TSize;
    }
        
    // Handles the probing factor case
    if(probeCount >= maxProbe){
            
      // Invokes incremental hashing
      m_hashStat++;
      pivotIdx = clusterID(initialIdx, m_hashArrayIdx);
      incrementalHash(pivotIdx);
    }
        
    // Handles the 0.5 factor case
    if((m_T[m_hashArrayIdx]->m_currentSize / m_T[m_hashArrayIdx]->m_TSize) > halfBatch){
            
      // Invokes incremental hashing
      m_hashStat++;
      pivotIdx = clusterID(initialIdx, m_hashArrayIdx);
      incrementalHash(pivotIdx);
    }
  }

  // Indicates that we are traversing through second table
  else if(m_hashStat == 1){
        
    index = hashCode(str) % m_T[m_hashArrayIdx]->m_TSize;
    initialIdx = index;
        
    // Traverses through hashTable any looking for str duplicate
    while((m_T[m_hashArrayIdx]->m_hashT[index] != NULL) && (duplicate == false)){
            
      probeCount++;
            
      // Only compares string in hash table if position is filled
      if(m_T[m_hashArrayIdx]->m_hashT[index] != NULL){
                
	// Finds duplicate within cluster
	if(strcmp(m_T[m_hashArrayIdx]->m_hashT[index], str) == 0){
	  duplicate = true;
	}
      }
            
      index = (index + 1) % m_T[m_hashArrayIdx]->m_TSize;
            
    }
        
    // Handles the probing factor case
    if(probeCount > 1){
            
      // Invokes incremental hashing
      pivotIdx = clusterID(initialIdx, m_hashArrayIdx);
      incrementalHash(pivotIdx);
    }
        
    // Handles the 0.5 factor case
    if((m_T[m_hashArrayIdx]->m_currentSize / m_T[m_hashArrayIdx]->m_TSize) > halfBatch){
            
      // Invokes incremental hashing
      pivotIdx = clusterID(initialIdx, m_hashArrayIdx);
      incrementalHash(pivotIdx);
    }
        
        
    // No duplicate string was found in first table
    // Checking second table
    if(duplicate == false){
            
      index = hashCode(str) % m_T[m_hashArrayIdx + 1]->m_TSize;
      initialIdx = index;
      probeCount = 1;
      insertedValue = false;
            
      // Traverses through hashTable
      while((insertedValue == false) && (duplicate == false)){
                
	probeCount++;
                
	// Only compares string in hash table if position is filled
	if(m_T[m_hashArrayIdx + 1]->m_hashT[index] != NULL){
                    
	  // Finds duplicate within cluster
	  if(strcmp(m_T[m_hashArrayIdx + 1]->m_hashT[index], str) == 0){
	    duplicate = true;
	  }
	}
                
	if(duplicate == false){
                    
	  // Determines if slot is avaliable for insertion
	  if((m_T[m_hashArrayIdx + 1]->m_hashT[index] == NULL) ||
	     (strcmp(m_T[m_hashArrayIdx + 1]->m_hashT[index], DELETED) == 0)){
                        
	    m_T[m_hashArrayIdx + 1]->m_hashT[index] = strdup(str);
	    m_T[m_hashArrayIdx + 1]->m_idxHolder[index] = initialIdx;
	    m_T[m_hashArrayIdx + 1]->m_currentSize++;
	    insertedValue = true;
	  }

	}
                
	index = (index + 1) % m_T[m_hashArrayIdx + 1]->m_TSize;
      }
    }
        
    // Handles the probing factor case
    if(probeCount >= maxProbe){
            
      // Gives up on incremental hashing; merges two tables
      mergeTable();
    }
        
    // Handles the 0.5 factor case
    else if((m_T[m_hashArrayIdx + 1]->m_currentSize / m_T[m_hashArrayIdx + 1]->m_TSize) > halfBatch){
            
      // Gives up on incremental hashing; merges two tables
      mergeTable();
    }
    
  }
}

// Find Function
bool HashTable::find(const char *str){
    
  bool result = false;
  bool exitLoop = false;
  bool secondT = false;
    
  int traverseIdx = 0;
  int initialIdx = 0;
  int pivotIdx = 0;
  int halfBatch = 2;
  int maxProbe = 10;
  int probeCount = 1;
    
  // Traversing through first table
  if(m_hashStat == 0){
        
    traverseIdx = hashCode(str) % m_T[m_hashArrayIdx]->m_TSize;
    initialIdx = traverseIdx;
        
    while(m_T[m_hashArrayIdx]->m_hashT[traverseIdx] != NULL){
            
      // Checks to see if index is not null
      if(m_T[m_hashArrayIdx]->m_hashT[traverseIdx] != NULL){
                
	// Checks to see if index value is an exact match with string
	if(strcmp(m_T[m_hashArrayIdx]->m_hashT[traverseIdx], str) == 0){
                    
	  result = true;
	  exitLoop = true;
	}
      }
            
      probeCount++;
      traverseIdx = (traverseIdx + 1) % m_T[m_hashArrayIdx]->m_TSize;

    }
        
    // Handles the probing factor case
    if(probeCount >= maxProbe){
            
      // Invokes incremental hashing
      m_hashStat++;
      pivotIdx = clusterID(initialIdx, m_hashArrayIdx);
      incrementalHash(pivotIdx);
    }
        
    // Handles the 0.5 factor case
    if((m_T[m_hashArrayIdx]->m_currentSize / m_T[m_hashArrayIdx]->m_TSize) > halfBatch){
            
      // Invokes incremental hashing
      m_hashStat++;
      pivotIdx = clusterID(initialIdx, m_hashArrayIdx);
      incrementalHash(pivotIdx);
    }
  }
    
  // Traversing through first table
  else if(m_hashStat == 1){
        
    traverseIdx = hashCode(str) % m_T[m_hashArrayIdx]->m_TSize;
    initialIdx = traverseIdx;
        
    // Traverses through first hash table
    while(m_T[m_hashArrayIdx]->m_hashT[traverseIdx] != NULL){
            
      // Checks to see if index is not null
      if(m_T[m_hashArrayIdx]->m_hashT[traverseIdx] != NULL){
                
	// Checks to see if index value is an exact match with string
	if(strcmp(m_T[m_hashArrayIdx]->m_hashT[traverseIdx], str) == 0){
                    
	  result = true;
	  exitLoop = true;
	}
      }
      probeCount++;
      traverseIdx = (traverseIdx + 1) % m_T[m_hashArrayIdx]->m_TSize;
    }
        
    // Handles the probing factor case
    if(probeCount > 1){
            
      // Invokes incremental hashing
      pivotIdx = clusterID(initialIdx, m_hashArrayIdx);
      incrementalHash(pivotIdx);
    }
        
    // Handles the 0.5 factor case
    if((m_T[m_hashArrayIdx]->m_currentSize / m_T[m_hashArrayIdx]->m_TSize) > halfBatch){
            
      // Invokes incremental hashing
      pivotIdx = clusterID(initialIdx, m_hashArrayIdx);
      incrementalHash(pivotIdx);
    }
        
    if(result == false)
      secondT = true;
        
    // Did not find item in first table; Looking into second table
    if(secondT == true){
            
      traverseIdx = hashCode(str) % m_T[m_hashArrayIdx + 1]->m_TSize;
      initialIdx = traverseIdx;
      probeCount = 1;
            
      // Traverses through second hash table
      while(m_T[m_hashArrayIdx + 1]->m_hashT[traverseIdx] != NULL){
                
	probeCount++;
                
	// Checks to see if index is not null
	if(m_T[m_hashArrayIdx + 1]->m_hashT[traverseIdx] != NULL){
                    
	  // Checks to see if index value is an exact match with string
	  if(strcmp(m_T[m_hashArrayIdx + 1]->m_hashT[traverseIdx], str) == 0){
                        
	    result = true;
	    exitLoop = true;
	  }
	}
            
	traverseIdx = (traverseIdx + 1) % m_T[m_hashArrayIdx + 1]->m_TSize;
      }
            
      // Handles the probing factor case
      if(probeCount >= maxProbe){
            
	// Gives up on incremental hashing; merges two tables
	mergeTable();
      }
        
      // Handles the 0.5 factor case
      else if((m_T[m_hashArrayIdx + 1]->m_currentSize / m_T[m_hashArrayIdx + 1]->m_TSize) > halfBatch){
            
	// Gives up on incremental hashing; merges two tables
	mergeTable();
      }
    }
  }
    
  return result;

}

// Remove Function
char *HashTable::remove(const char *str){
    
  bool secondT = false;
    
  int traverseIdx = 0;
  int initialIdx = 0;
  int pivotIdx = 0;
    
  int halfBatch = 2;
  int maxProbe = 10;
  int probeCount = 1;
    
  char *value = NULL;

  // Traversing through first table
  if(m_hashStat == 0){
        
    traverseIdx = hashCode(str) % m_T[m_hashArrayIdx]->m_TSize;
    initialIdx = traverseIdx;
        
    while(m_T[m_hashArrayIdx]->m_hashT[traverseIdx] != NULL){
            
        
      // Checks to see if index is not null
      if(m_T[m_hashArrayIdx]->m_hashT[traverseIdx] != NULL){
                
	// Checks to see if index value matches with string
	if(strcmp(m_T[m_hashArrayIdx]->m_hashT[traverseIdx], str) == 0){
                        
	  value = m_T[m_hashArrayIdx]->m_hashT[traverseIdx];
	  m_T[m_hashArrayIdx]->m_hashT[traverseIdx] = DELETED;
	  m_T[m_hashArrayIdx]->m_currentSize--;
	}
      }
      traverseIdx = (traverseIdx + 1) % m_T[m_hashArrayIdx]->m_TSize;
      probeCount++;
    }
        
    // Handles the probing factor case
    if(probeCount >= maxProbe){
            
      // Invokes incremental hashing
      m_hashStat++;
      pivotIdx = clusterID(initialIdx, m_hashArrayIdx);
      incrementalHash(pivotIdx);
    }
        
    // Handles the 0.5 factor case
    if((m_T[m_hashArrayIdx]->m_currentSize / m_T[m_hashArrayIdx]->m_TSize) > halfBatch){
            
      // Invokes incremental hashing
      m_hashStat++;
      pivotIdx = clusterID(initialIdx, m_hashArrayIdx);
      incrementalHash(pivotIdx);
    }
  }
  
  // Traversing through first table
  else if(m_hashStat == 1){
        
    traverseIdx = hashCode(str) % m_T[m_hashArrayIdx]->m_TSize;
    initialIdx = traverseIdx;
        
    // Traverses through first hash table
    while(m_T[m_hashArrayIdx]->m_hashT[traverseIdx] != NULL){
            
      // Checks to see if index is not null
      if(m_T[m_hashArrayIdx]->m_hashT[traverseIdx] != NULL){
                
	// Checks to see if index value matches with string
	if(strcmp(m_T[m_hashArrayIdx]->m_hashT[traverseIdx], str) == 0){
                    
	  value = m_T[m_hashArrayIdx]->m_hashT[traverseIdx];
	  m_T[m_hashArrayIdx]->m_hashT[traverseIdx] = DELETED;
	  m_T[m_hashArrayIdx]->m_currentSize--;
	}
      }
      traverseIdx = (traverseIdx + 1) % m_T[m_hashArrayIdx]->m_TSize;
      probeCount++;
            
    }
        
    // Handles the probing factor case
    if(probeCount > 1){
            
      // Invokes incremental hashing
      pivotIdx = clusterID(initialIdx, m_hashArrayIdx);
      incrementalHash(pivotIdx);
    }
        
    // Handles the 0.5 factor case
    if((m_T[m_hashArrayIdx]->m_currentSize / m_T[m_hashArrayIdx]->m_TSize) > halfBatch){
            
      // Invokes incremental hashing
      pivotIdx = clusterID(initialIdx, m_hashArrayIdx);
      incrementalHash(pivotIdx);
    }
        
    if(value == NULL)
      secondT = true;
        
    // Did not find item in first table; Looking into second table
    if(secondT == true){
            
      traverseIdx = hashCode(str) % m_T[m_hashArrayIdx + 1]->m_TSize;
      initialIdx = traverseIdx;
      probeCount = 1;
            
      // Traverses through second hash table
      while(m_T[m_hashArrayIdx + 1]->m_hashT[traverseIdx] != NULL){
                
	// Checks to see if index is not null
	if(m_T[m_hashArrayIdx + 1]->m_hashT[traverseIdx] != NULL){
                    
	  // Checks to see if index value matches with string
	  if(strcmp(m_T[m_hashArrayIdx + 1]->m_hashT[traverseIdx], str) == 0){
                        
	    value = m_T[m_hashArrayIdx + 1]->m_hashT[traverseIdx];
	    m_T[m_hashArrayIdx + 1]->m_hashT[traverseIdx] = DELETED;
	    m_T[m_hashArrayIdx + 1]->m_currentSize--;
	  }
	}
	traverseIdx = (traverseIdx + 1) % m_T[m_hashArrayIdx + 1]->m_TSize;
	probeCount++;
      }
            
      // Handles the probing factor case
      if(probeCount >= maxProbe){
                
	// Gives up on incremental hashing; merges two tables
	mergeTable();
      }
            
      // Handles the 0.5 factor case
      else if((m_T[m_hashArrayIdx + 1]->m_currentSize / m_T[m_hashArrayIdx + 1]->m_TSize) > halfBatch){
                
	// Gives up on incremental hashing; merges two tables
	mergeTable();
      }
    }
  }
  
  return value;
}

int HashTable::sizeOfHash(int n){
    
  int oldSize = m_T[m_hashArrayIdx]->m_TSize;
  int newSize = 0;
  int newTotal;
  bool exitLoop = false;
    
  newTotal = n;
  newSize = roundUpPrime(newTotal);
    
  // Stores previous table size
  while(exitLoop == false){
        
    // Handles the case where old and new table sizes are equal
    if(oldSize == newSize){
      newTotal++;
      newSize = roundUpPrime(newTotal);
    }
        
    // Handles the case where new prime number is greater than maxSize
    else if(newSize > m_T[m_hashArrayIdx]->m_maxSize){
      throw::out_of_range("Table size is too large");
    }
        
    if(oldSize != newSize){
      exitLoop = true;
    }
        
  }
    
  return newSize;
}

// Incremental Hashing
void HashTable::incrementalHash(int index){

  bool hashing = true;
  double smallBatch = 0.03;
  int currentT = 1;
  int previousT = 0;
    
  // Creates new hash table
  if(m_tableCount < m_hashArraySize - 1){
        
    int newTotal = m_T[m_hashArrayIdx]->m_currentSize * 4;
        
    // Calculates new size for hashing table
    int newSize = sizeOfHash(newTotal);
            
    // Creates new hashing table object
    m_T[currentT] = new HashTableObj(newTotal, newSize, hashing);
    m_tableCount++;
  }
    
  // Transfers contents from old table to new table
  if(m_T[(previousT) % m_hashArraySize] != NULL){
        
    // Traverses through cluster and moves them to new table
    while(m_T[previousT]->m_hashT[index] != NULL){
            
      // Moves cluster over to new table
      transferCluster(index);
            
      // Increments the indices for both tables for next iteration
      index = (index + 1) % m_T[m_hashArrayIdx]->m_TSize;
            
    }
        
    // Handles the 3% case
    double currentSize = m_T[previousT]->m_currentSize;
    double tableSize = m_T[previousT]->m_TSize;
    double batchSize = currentSize / tableSize;
        
    if(batchSize < smallBatch){
            
      // Moves rest of table content over to new table
      for(int i = 0; i < m_T[previousT]->m_TSize; i++){
                
	if(m_T[previousT]->m_hashT[i] != NULL){
                    
	  // Moves cluster over to new table
	  transferCluster(i);
	}
      }
    }
        
    // Deletes empty table
    if(m_T[previousT]->m_currentSize == 0){
            
      // Destroys table;
      destroyTable(previousT);
      m_tableCount--;
            
      // Places the new hash table in first slot
      m_T[previousT] = m_T[currentT];
      m_T[currentT] = NULL;
      m_hashStat = 0;
    }
  }
}

int HashTable::clusterID(int index, int table){
    
  int pivotIdx = index;
    
  // Traverses through cluster to identify the beginning of cluster
  while(m_T[table]->m_hashT[index] != NULL){
        
    index = (index - 1) % m_T[table]->m_TSize;
        
    if(m_T[table]->m_hashT[index] != NULL){
      pivotIdx = index;
    }
  }
    
  return pivotIdx;
    
}

void HashTable::transferCluster(int index){
    
  char *value = NULL;
  int previousT = 0;
  int currentT = 1;
  int currentIdx = 0;
  int initialIdx = 0;
    
  // Checks to see if index is populated and not equal to DELETED
  if((m_T[previousT]->m_hashT[index] != NULL) && (strcmp(m_T[previousT]->m_hashT[index], DELETED) != 0)){
        
    // Grabs value in old table
    value = m_T[previousT]->m_hashT[index];
    
    // Value is removed from old table
    m_T[previousT]->m_hashT[index] = NULL;
    
    // Decrements value count in old table
    m_T[previousT]->m_currentSize--;
    
    // Inserts only valid strings within new table
    if(value != NULL){
        
      // Calculates new index prior to new table insertion
      currentIdx = hashCode(value) % m_T[currentT]->m_TSize;
        
      initialIdx = currentIdx;
        
      // Ensures that placement is not occupied
      while(m_T[currentT]->m_hashT[currentIdx] != NULL){
	currentIdx++;
      }
      // Inserts value in new table
      m_T[currentT]->m_hashT[currentIdx] = value;
        
      // Stores initial index for string in new table
      m_T[currentT]->m_idxHolder[currentIdx] = initialIdx;
        
      // Increments value count in new table
      m_T[currentT]->m_currentSize++;
        
    }
  }
}
    
void HashTable::mergeTable(){
    
  char *value = NULL;
  bool hashing = true;
  int previousT = 0;
  int currentT = 1;
  int mergeT = 2;
  int currentIdx = 0;
  int initialIdx = 0;
    
  // Calculates third table's total size
  int currentSizeOne = m_T[previousT]->m_currentSize;
  int currentSizeTwo = m_T[currentT]->m_currentSize;
  int newTotal = (currentSizeOne + currentSizeTwo) * 4;
    
  // Calculates table's size
  int newSize = sizeOfHash(newTotal);

  // Creates new hashing table object
  m_T[mergeT] = new HashTableObj(newTotal, newSize, hashing);
    
  // Checks to see if current table exists
  if(m_T[currentT] != NULL){
        
    // Transfers contents over to third table
    for(int i = 0; i < m_T[currentT]->m_TSize; i++){
            
      // Checks to see if index is populated and not equal to DELETED
      if((m_T[currentT]->m_hashT[i] != NULL) && (strcmp(m_T[currentT]->m_hashT[i], DELETED) != 0)){
                
	// Grabs value in old table
	value = m_T[currentT]->m_hashT[i];
                
	// Value is removed from old table
	m_T[currentT]->m_hashT[i] = NULL;
                
	// Decrements value count in old table
	m_T[currentT]->m_currentSize--;
                
	// Only inserts valid strings into new table
	if(value != NULL){
	  // Calculates new index prior to new table insertion
	  currentIdx = hashCode(value) % m_T[mergeT]->m_TSize;
	  initialIdx = currentIdx;
                    
	  // Ensures that placement is not occupied
	  while(m_T[mergeT]->m_hashT[currentIdx] != NULL){
	    currentIdx = (currentIdx + 1) % m_T[mergeT]->m_TSize;
	  }
	  // Inserts value in new table
	  m_T[mergeT]->m_hashT[currentIdx] = value;
                    
	  // Stores initial index for string in new table
	  m_T[mergeT]->m_idxHolder[currentIdx] = initialIdx;
                    
	  // Increments value count in new table
	  m_T[mergeT]->m_currentSize++;
                    
	}
      }
    }
        
    // Destroys currentT
    destroyTable(currentT);
    m_tableCount--;
  }

  // Checks to see if previous table exists
  if(m_T[previousT] != NULL){
        
    // Transfers contents over to third table
    for(int i = 0; i < m_T[previousT]->m_TSize; i++){
            
      // Checks to see if index is populated and not equal to DELETED
      if((m_T[previousT]->m_hashT[i] != NULL) && (strcmp(m_T[previousT]->m_hashT[i], DELETED) != 0)){
                
	// Grabs value in old table
	value = m_T[previousT]->m_hashT[i];
                
	// Value is removed from old table
	m_T[previousT]->m_hashT[i] = NULL;
                
	// Decrements value count in old table
	m_T[previousT]->m_currentSize--;
                
	// Only inserts valid strings into new table
	if(value != NULL){
                    
	  // Calculates new index prior to new table insertion
	  currentIdx = hashCode(value) % m_T[mergeT]->m_TSize;
	  initialIdx = currentIdx;
                    
	  // Ensures that placement is not occupied
	  while(m_T[mergeT]->m_hashT[currentIdx] != NULL){
	    currentIdx = (currentIdx + 1) % m_T[mergeT]->m_TSize;
	  }
                    
	  // Inserts value in new table
	  m_T[mergeT]->m_hashT[currentIdx] = value;
                    
	  // Stores initial index for string in new table
	  m_T[mergeT]->m_idxHolder[currentIdx] = initialIdx;
                    
	  // Increments value count in new table
	  m_T[mergeT]->m_currentSize++;
                    
	}
      }
    }
        
    // Destroys previousT
    destroyTable(previousT);
    m_tableCount--;
  }
    
  // Reshifts the objects' positions
  m_T[previousT] = m_T[mergeT];
  m_T[currentT] = NULL;
  m_T[mergeT] = NULL;
    
  // Resets the hashing status
  m_hashStat = 0;
    
}

void HashTable::destroyTable(int table){
    
  // Deletes empty table
  if(m_T[table]->m_currentSize == 0){
        
    // Deallocates items in table array
    for(int i = 0; i < m_T[table]->m_TSize; i++){
            
      if(m_T[table]->m_hashT[i] != DELETED){
	free(m_T[table]->m_hashT[i]);
	m_T[table]->m_hashT[i] = NULL;
      }
    }
        
    // Deallocates the hash array table
    free(m_T[table]->m_hashT);
    m_T[table]->m_hashT = NULL;
        
    // Deallocates the idx holder array from object
    delete [] m_T[table]->m_idxHolder;
    m_T[table]->m_idxHolder = NULL;
        
    // Deallocates hash table object
    delete m_T[table];
    m_T[table] = NULL;
        
  }
}

// Grading Functions
bool HashTable::isRehashing(){
    
  bool result = false;
    
  // Indicates that it's in hashing mode
  if(m_hashStat > 0){
        
    result = true;
  }
    
  return result;
}
int HashTable::tableSize(int table){
    
  return m_T[table]->m_TSize;
}
int HashTable::size(int table){
    
  return m_T[table]->m_currentSize;
}
const char *HashTable::at(int index, int table){
    
  char * value = NULL;
    
  if(table == 0){
        
    if(index >= m_T[0]->m_TSize)
      throw out_of_range("Index is out of range");
    else
      value = m_T[table]->m_hashT[index];
      
  }
    
  else if(table == 1){
    if(index >= m_T[1]->m_TSize)
      throw out_of_range("Index is out of range");
    else
      value = m_T[table]->m_hashT[index];
  }
    
  else if(table == 2){
    if(index >= m_T[2]->m_TSize)
      throw out_of_range("Index is out of range");
    else
      value = m_T[table]->m_hashT[index];
  }
    
  return value;
}

// Dump Function
void HashTable::dump(){
    
  // Traverses through hash array storage
  for(int j = 0; j < m_hashArraySize; j++){
       
    if(m_T[j] != NULL){
           
      cout << "Hash Table: " << j + 1;
      cout << "     Size: " << m_T[j]->m_currentSize;
      cout << "     Table Size: " << m_T[j]->m_TSize << endl;
            
      // Traverses through hash table
      for(int i = 0; i < m_T[j]->m_TSize; i++){
                
	// Outputs null format
	if(m_T[j]->m_hashT[i] == NULL){
	  cout <<"H" << j + 1 <<": " << "[" << i << "] = " << endl;
	}
                
	// Outputs deleted format
	else if(strcmp(m_T[j]->m_hashT[i], DELETED) == 0){
	  cout <<"H" << j + 1 <<": " << "[" << i << "] = DELETED" << endl;
	}
                
	// Outputs populated index format
	else{
	  cout <<"H" << j + 1 <<": " << "[" << i << "] = ";
	  cout << m_T[j]->m_hashT[i] << "  (" << m_T[j]->m_idxHolder[i] << ")" << endl;
	}
      }
    }
  }
}

