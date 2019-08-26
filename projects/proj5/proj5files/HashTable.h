// File: HashTable.h
// Project: CMSC 341: Incremental Hashing
// Author: Dane Magbuhos
// Date: 12/10/18
//
// Description: This files contains the functions
//              and private variables for the hashing
//              program
//

#ifndef HashTable_h
#define HashTable_h

using namespace std;

class HashTableObj{
    
  friend class HashTable;
    
 public:
  // Constructor
  HashTableObj(int n = 101, int prime = 101, bool hashing = false);
    
  // Destructor
  ~HashTableObj();
    
 private:
    
  int m_maxSize;
  int m_minSize;
  int m_requestedSize;
  int m_TSize;
  int m_currentSize;
    
  // Hash Table Object
  char **m_hashT;
    
  // Index Holder
  int *m_idxHolder;
    
};

class HashTable{
    
  friend class HashTableObj;
    
 public:
    
  // Deleted Item
  static char * const DELETED;
    
  // Constructor
  HashTable(int n = 101);

  // Destructor
  ~HashTable();

  // Copy Constructor
  HashTable(HashTable& other);

  // Overloaded Assignment Operator
  const HashTable& operator= (HashTable &rhs);
    
  // Hashing Algorithm
  unsigned int hashCode(const char *str);
    
  // Insert Function
  void insert(const char *str);

  // Find Function
  bool find(const char *str);

  // Remove Function
  char * remove(const char *str);
    
  // Main source of hash
  void incrementalHash (int index);
    
  // Calculates size of new table
  int sizeOfHash (int n);
    
  // Identifies beginning of cluster
  int clusterID (int index, int table);
    
  // Handles the movement of clusters
  void transferCluster (int index);
    
  // Merges two tables into one
  void mergeTable ();
    
  // Destroys specific table
  void destroyTable (int table);
    
    
  // Grading Functions
  bool isRehashing();
  int tableSize(int table=0);
  int size(int table=0);
  const char * at(int index, int table=0);
    
  // Dump Function
  void dump();

 private:
    
  int m_count;
  int m_hashArrayIdx;
  int m_hashArraySize;
  int m_tableCount;
  int m_hashStat;
    
  HashTableObj *m_T[3];
    
  bool m_hash;
        
};

#endif /* HashTable_h */
