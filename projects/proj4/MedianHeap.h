// File: MedianHeap.h
// Project: CMSC 341: Median Heap
// Author: Dane Magbuhos
// Date: 11/05/18
//
// Description: This file contains the function prototypes
//              and member variables of Median Heap project.
//


#ifndef MedianHeap_h
#define MedianHeap_h

#include <iostream>
#include <stdexcept>
#include <stdlib.h>

	//#include "MedianHeap.cpp";

template <typename T>
class MedianHeap;

template<typename T>
class Heap{
    
 public:
    
  // Constructor: Builds the heap array
  Heap(bool (*fpt) (const T&, const T&), int size);
    
  // Copy Constructor
  Heap(const Heap<T> &otherH);
    
  // Overloaded Assignment Operator
  const Heap<T>& operator=(const Heap<T> &rhs);
    
  // Destructor
  ~Heap();
    
  // Adds object to array
  void insert(T value);
    
  // Returns a pointer to heap array
  T* getArray();
    
  // Returns the size of heap array
  int getSize();
    
  // Returns min or max value from min or max heap
  T getFirst();

  // Gets rid of first object in index
  void deleteFirst(int nodeIndex);
    
  // Functions that grabs specific indices
  int leftIdx(int nodeIdx);
  int rightIdx(int nodeIdx);
  int parentIdx(int nodeIdx);
    
  // Shifts element upwards and adjusts the nodes accordingly
  void shiftUp(int nodeIdx);
    
  // Shifts elements downwards and adjusts the nodes accordingly
  void shiftDown(int nodeIdex);
    

 private:
    
  // Stores array
  T* m_values;
    
  int m_heapSize;
  int m_arraySize;
    
  // Stores function pointer
  bool (*comparison)(const T&, const T&);
    
  friend class MedianHeap<T>;
    
};


template<typename T>
class MedianHeap{
    
 public:
    
  // Constructor
  MedianHeap(bool (*lt) (const T&, const T&), bool (*gt) (const T&, const T&), int cap=100);
    
  // Copy Constructor
  MedianHeap(const MedianHeap<T>& otherH);
    
  // Destructor
  ~MedianHeap();
    
  // Overloaded assignment operator
  const MedianHeap<T>& operator=(const MedianHeap<T>& rhs);
    
  // Returns the maximum size that array can store
  int size();
    
  // Returns the maximum of items that can be stored
  int capacity();
    
  // Balances heap after deletion or insertion
  void balanceHeap();
    
  // Finds the new minimum value
  void newMin();
    
  // Finds the new maximum value
  void newMax();
    
  // Inserts object within median heap
  void insert(const T& item);
    
    
  // Grabs the median value
  T getMedian();
    
  // Grabs minimum value
  T getMax();
    
  // Grabs maximum value
  T getMin();
    
  // Deletes a certain item within median heap
  bool deleteItem(T& givenItem, bool (*equalTo) (const T&, const T&));
    
  // Prints out items in median heap
  void dump() ;
    
  // Grade Functions:
  // Grabs the max size in max heap
  int maxHeapSize();
    
  // Grabs the min size min heap
  int minHeapSize();
    
  // Returns a copy of object at specific index in max heap
  T locateInMaxHeap(int pos);
    
  // Returns a copy of object at specific index in min heap
  T locateInMinHeap(int pos);
    
 private:
    
  int m_size;
  int m_cap;
    
  T m_median;
  T m_minimum;
  T m_maximum;
    
  // Points to min heap object
  Heap<T> *m_minHeap;
    
  // Points to max heap object
  Heap<T> *m_maxHeap;
    
  // Stores greater than function pointer
  bool (*greaterThan)(const T&, const T&);
    
  // Stores less than function pointer
  bool (*lessThan)(const T&, const T&);
    
  bool (*equalTo)(const T&, const T&);
    
  friend class Heap<T>;
    
};

// Includes the templated implementations
#include "MedianHeap.cpp"

#endif
