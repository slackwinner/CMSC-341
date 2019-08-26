// File: MedianHeap.h
// Project: CMSC 341: Median Heap
// Author: Dane Magbuhos
// Date: 11/05/18
//
// Description: This program stores values in a
//              min and max heap and keeps track of the
//              following statistics: median, minimum,
//              and maximum
//

#ifndef Median_Heap_cpp
#define Median_Heap_cpp

#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "MedianHeap.h"

using namespace std;

// Median Implementations
// Constructor
template <typename T>
Heap<T>::Heap(bool(*hfpt) (const T&, const T&), int size){
    
  // Indexing starts at [1] instead of [0]
  // Initailizes the heap components
  size = size + 2;

  m_values = new T[size];
  m_heapSize = 0;
  m_arraySize = size;
  comparison = hfpt;
}

// Copy Constructor
template <typename T>
Heap<T>::Heap(const Heap<T> &otherH){
    
  m_heapSize = otherH.m_heapSize;
  m_arraySize = otherH.m_arraySize;
  comparison = otherH.comparison;
    
  // Creates array
  m_values = new T[m_arraySize];
    
  // Copies values from heap
  for(int j = 1; j <= m_heapSize; j++){
    m_values[j] = T(otherH.m_values[j]);
        
  }
}

// Destructor
template <typename T>
Heap<T>::~Heap(){
    
  delete [] m_values;
    
}

// Overloaded Assignment Operator
template <typename T>
const Heap<T>& Heap<T>::operator=(const Heap<T> &rhs){
    
  delete [] m_values;
  m_heapSize = NULL;
  m_arraySize = NULL;
    
  // Copies items if lhs and rhs don't match
  if(this != rhs){
        
    m_heapSize = rhs.m_heapSize;
    m_arraySize = rhs.m_arraySize;
        
    m_values = new T[m_arraySize];
        
    for(int i = 1; i<= m_heapSize; i++){
      m_values[i] = T(rhs.data[i]);
    }
        
  }
    
  return *this;
    
}

// Insert Function
template <typename T>
void Heap<T>::insert(T value){
    
  // Checks to see if heap is full
  if((m_heapSize + 1) == m_arraySize){
    throw std::out_of_range("Heap is full");
  }
    
  else{
        
    // Inserts value at specific index
    m_heapSize++;
    m_values[m_heapSize] = value;
        
  }
}

// getArray Function
template <typename T>
T* Heap<T>::getArray(){
    
  return m_values;
}

// getSize Function
template <typename T>
int Heap<T>::getSize(){
    
  return m_heapSize;
    
}

// getFirst Function
template <typename T>
T Heap<T>::getFirst(){
    
  int empty = 0;
  int idx = 1;
    
  if(m_heapSize == empty){
    throw std::out_of_range("Heap is empty");
  }
  else
    return m_values[idx];
}

// deleteFirst Function
template <typename T>
void Heap<T>::deleteFirst(int nodeIdx){
    
  int empty = 0;
    
  // Checks to see if heap array is empty
  if(m_heapSize == empty){
        
    throw std::out_of_range("Heap is empty");
  }
    
  else{
        
    // Makes last element the new root
    m_values[nodeIdx] = m_values[m_heapSize];
    m_heapSize--;
        
    // Only shifts downward if the root wasn't deleted
    if(m_heapSize > empty){
      shiftDown(nodeIdx);
    }
        
  }
}

// Left Child Index Function
template <typename T>
int Heap<T>::leftIdx(int nodeIdx){
    
  // Calculates left child of current index
  nodeIdx = 2 * nodeIdx;

  return nodeIdx;
    
}

// Right Child Index Function
template <typename T>
int Heap<T>::rightIdx(int nodeIdx){
    
  // Calculates right child of current index
  nodeIdx = (2 * nodeIdx) + 1;
    
  return nodeIdx;
    
}

// Parent Index Function
template <typename T>
int Heap<T>::parentIdx(int nodeIdx){
    
  // Calculates parent of current index
  nodeIdx = nodeIdx / 2;
    
  return nodeIdx;
    
}

// Shifting up Function
template <typename T>
void Heap<T>::shiftUp(int nodeIdx){
    
  int parentIndex;
  T temp;
    
  // Checks to see if inserted value was not the root
  if(nodeIdx != 1){
    parentIndex = parentIdx(nodeIdx);
        
    // Compares current and parent nodes
    if(comparison(m_values[nodeIdx], m_values[parentIndex])){
            
      // Swaps the positions of current node and parent node
      temp = m_values[parentIndex];
      m_values[parentIndex] = m_values[nodeIdx];
      m_values[nodeIdx] = temp;
      shiftUp(parentIndex);
            
    }
  }
}

// Shifting Down Function
template <typename T>
void Heap<T>::shiftDown(int nodeIdx){
    
  int leftIndex = leftIdx(nodeIdx);
  int rightIndex = rightIdx(nodeIdx);
  int comparisonIndex = leftIndex;
  T temp;

  // Checks to see if left and right indices are out of array range
  if((leftIndex > m_heapSize) && (rightIndex > m_heapSize))
    return;
    
  else{
        
    // Compares the left and right nodes
    if(comparison(m_values[rightIndex], m_values[leftIndex])){
     
      // Checks that rightIndex is within size range
       if(rightIndex <= m_heapSize){
	  comparisonIndex = rightIndex;
        } 

        // Checks that leftIndex is within size range
	else if(leftIndex <= m_heapSize){
	     comparisonIndex = leftIndex;
	}
    }
  }
    
  // Swaps the positions of of current node and right/left node
  if(comparison(m_values[comparisonIndex], m_values[nodeIdx])){
    temp = m_values[comparisonIndex];
    m_values[comparisonIndex] = m_values[nodeIdx];
    m_values[nodeIdx] = temp;
    shiftDown(comparisonIndex);
        
  }
}

// Median Heap Implementations

  // Constructor
template <typename T>
MedianHeap<T>::MedianHeap( bool (*lt) (const T&, const T&), bool (*gt) (const T&, const T&), int cap){
        
  // Represents the first half of median range
  m_minHeap = new Heap<T> (lt, cap/2);
    
  // Represents the second half of median range
  m_maxHeap = new Heap<T> (gt, cap/2);
    
  // Creates other components of heaps
  m_size = 0;
  m_cap = cap;
  greaterThan = gt;
  lessThan = lt;
  
}

// Copy Constructor
template <typename T>
MedianHeap<T>::MedianHeap(const MedianHeap<T>& otherH){
    
  // Makes a new copy of the heaps
  greaterThan = otherH.greaterThan;
  lessThan = otherH.lessThan;
  m_cap = otherH.m_cap;
    
  m_minHeap = new Heap<T> (*(otherH.m_minHeap));
  m_maxHeap = new Heap<T> (*(otherH.m_maxHeap));
    
}

// Destructor
template <typename T>
MedianHeap<T>::~MedianHeap(){
    
  delete m_minHeap;
  delete m_maxHeap;
}

// Overloaded Assignment Operator
template <typename T>
const MedianHeap<T>& MedianHeap<T>::operator=(const MedianHeap<T>& rhs){
    
  // Resets all Median Heap parts
  delete m_minHeap;
  delete m_maxHeap;
    
  greaterThan = NULL;
  lessThan = NULL;
  m_cap = NULL;
            
  // Assigns the corrsponding Median Heap parts
  greaterThan = rhs.greaterThan;
  lessThan = rhs.lessThan;
  m_cap = rhs.m_cap;
        
  m_minHeap = new Heap<T>(*(rhs.m_minHeap));
  m_maxHeap = new Heap<T>(*(rhs.m_maxHeap));
 
    
  return *this;
    
}


// Size function
template <typename T>
int MedianHeap<T>::size(){
    
  // Grabs size of min heap
  int minSize = m_minHeap->getSize();
    
  // Grabs size of max heap
  int maxSize = m_maxHeap->getSize();
    
  m_size = minSize + maxSize;
    
  return m_size;
    
}

// Capacity Function
template <typename T>
int MedianHeap<T>::capacity(){
    
  return m_cap;
    
}


// Balancing Heap Function
template<typename T>
void MedianHeap<T>::balanceHeap(){
    
  int currentMaxSize = m_maxHeap->getSize();
  int currentMinSize = m_minHeap->getSize();
  int element = 2;
  int idx = 1;
        
  // Rebalances heap if max heap has twice as more items than min heap
  if(currentMaxSize - currentMinSize == element){
        
    // Stores max value into temp
    T temp = m_maxHeap->getFirst();
        
    // Removes max value from max heap
    m_maxHeap->deleteFirst(idx);
        
    // Inserts temp into min heap
    m_minHeap->insert(temp);
        
    // Reshifts heap array
    m_minHeap->shiftUp(m_minHeap->m_heapSize);
        
  }
    
  // Rebalances heap if min heap has twice as more items than max heap
  else if(currentMinSize - currentMaxSize == element){
            
    // Stores min value into temp
    T temp = m_minHeap->getFirst();
            
    // Removes min value from min heap
    m_minHeap->deleteFirst(idx);
            
    // Inserts temp into max heap
    m_maxHeap->insert(temp);
            
    // Reshifts heap array
    m_maxHeap->shiftUp(m_maxHeap->m_heapSize);
            
  }
}

// Finds new minimum value
template<typename T>
void MedianHeap<T>::newMin(){
    
  int index;
  int initialIdx = 1;
  int maxSize = m_maxHeap->getSize();
    
  // Sets initial minimum value from max heap
  m_minimum = m_maxHeap->m_values[initialIdx];
    
  // Traverses through entire heap looking for minimum value
    for(index = 2; index < maxSize; index++){
      if(lessThan(m_maxHeap->m_values[index], m_minimum)){
	m_minimum = m_maxHeap->m_values[index];
            
      }
    }
  }

// Finds new maximum value
template<typename T>
void MedianHeap<T>::newMax(){
    
  int index;
  int initialIdx = 1;
  int minSize = m_minHeap->getSize();
    
  // Sets initial maximum value from min heap
  m_maximum = m_minHeap->m_values[initialIdx];
    
  // Traverses through entire heap looking for minimum value
  for(index = 2; index < minSize; index++){
    if(greaterThan(m_minHeap->m_values[index], m_maximum)){
      m_maximum = m_minHeap->m_values[index];
            
    }
        
  }
}

// Insert Function
template <typename T>
void MedianHeap<T>::insert(const T& item){
    
  int empty = 0;

  m_size = size();
    
  // Checks to see if heap is at max capacity
  if(m_size == m_cap){
    throw std::out_of_range("Median Heap is currently full");
  }
    
  // No populated heaps places item in maxHeap by default
  // (i.e. since there are no other items in the array heap)
  if(m_size == empty){
        
    m_maxHeap->insert(item);    
    m_minimum = item;
    m_maximum = item;
        
  }
    
  else{
        
    // Compare item to median
    // Inserts item in max heap if item is less than to median
    if(lessThan(item, getMedian())){
      m_maxHeap->insert(item);
      m_maxHeap->shiftUp(m_maxHeap->m_heapSize);
            
    }
        
    // Inserts item in min heap if item is greater than or equal to median
    else{
      m_minHeap->insert(item);
      m_minHeap->shiftUp(m_minHeap->m_heapSize);
    }
        
    // Check to see it inserted value is the new min or max
    if(lessThan(item, m_minimum)){
      m_minimum = item;
    }
        
    else if(greaterThan(item, m_maximum)){
      m_maximum = item;
            
    }
        
    // Checks to make sure heap is balanced
    balanceHeap();
        
  }
}

// Median Function
template <typename T>
T MedianHeap<T>::getMedian(){
    
  int empty = 0;
  int minSize = m_minHeap->getSize();
  int maxSize = m_maxHeap->getSize();
  T median;

  // Computes the total array size
  m_size = minSize + maxSize;

   
  // Checks to see if heap is empty
  if(m_size == empty){
    throw std::out_of_range("Heap is empty");
  }
    
  // Checks to see which heap size is the greatest
  if(minSize > maxSize)
        
    // Sets the median equal to the first value in min heap
    median = m_minHeap->getFirst();
    
  else if(maxSize >= minSize){
        
    // Sets the median equal to the first value in max heap
    median = m_maxHeap->getFirst();
  }
    
  return median;
    
}

// Minimum Function
template <typename T>
T MedianHeap<T>::getMin(){
    
  int empty = 0;
    
  // Grabs current size of heap
  m_size = size();
    
  // Checks to see if heap is empty
  if(m_size == empty){
    throw std::out_of_range("Median Heap is empty; No Min Value Located");
  }
    
  return m_minimum;
    

}

// Maximum Function
template <typename T>
T MedianHeap<T>::getMax(){
    
  int empty = 0;
    
  // Grabs current size of heap
  m_size = size();
    
  // Checks to see if heap is empty
  if(m_size == empty){
    throw std::out_of_range("Median Heap is empty; No Max Value Located");
  }
    
  return m_maximum;
    
}

// Deletion Function
template <typename T>
bool MedianHeap<T>::deleteItem(T& givenItem, bool (*equalTo) (const T&, const T&)){
    
  int empty = 0;
  int maxSize = m_maxHeap->getSize();
  int minSize = m_minHeap->getSize();
  bool deletedItem = false;

  // Computes the total array size;
  m_size = maxSize + minSize;
  
  if(m_size == empty){
    throw std::out_of_range("Median Heap is empty");
  }
    
  // Traverses through max heap and delete specific value
  for(int j = 1; j <= maxSize; j++){
        
    if(equalTo(givenItem, m_maxHeap->m_values[j])){
            
      // Stores selected item for equalTo comparison
      givenItem = m_maxHeap->m_values[j];
            
      // Deletes item at specific index
      m_maxHeap->deleteFirst(j);
            
      // Adjusts the heap after deletion
      m_maxHeap->shiftDown(j);
      m_maxHeap->shiftUp(j);
            
      // Ensures that entire max heap is balanced
      balanceHeap();
            
      // Checks to see if minimum value was deleted
      if(equalTo(givenItem, m_minimum)){
                
	// Finds the new minimum value
	newMin();
      }
            
      deletedItem = true;
    }
  }
    
  // Traverses through min heap and delete specific value
  for(int i = 1; i <= minSize; i++){
        
    if(equalTo(givenItem, m_minHeap->m_values[i])){
            
      // // Stores selected item for equalTo comparison
      givenItem = m_minHeap->m_values[i];
            
      // Deletes item at specific index
      m_minHeap->deleteFirst(i);
            
      // Adjusts the heap based on deletion
      m_minHeap->shiftDown(i);
      m_minHeap->shiftUp(i);
            
      // Ensures that entire min heap is balanced
      balanceHeap();
            
      // Checks to see if maximum value was deleted
      if(equalTo(givenItem, m_maximum)){
                
	// Finds the new maximum value
	newMax();
      }
            
      deletedItem = true;
    }
  }
    
  return deletedItem;
}

// Heap Print Function
template <typename T>
void MedianHeap<T>::dump(){
 
  cout << "MedianHeap::dump()"<<endl;
  cout << "---------Max Heap---------\n";
    
  cout <<"SIZE: " << m_maxHeap->m_heapSize << "    Capacity: " << m_maxHeap->m_arraySize << endl;
    
  for(int j = 1; j <= m_maxHeap->m_heapSize; j++){
    cout<<"Index[" << j << "] = " << m_maxHeap->m_values[j] << endl;
  }
   
  cout << " " << endl;
  cout << "---------Min Heap---------\n";
    
  cout <<"SIZE: " << m_minHeap->m_heapSize << "    Capacity: " << m_minHeap->m_arraySize << endl;
    
  for(int j = 1; j <= m_minHeap->m_heapSize; j++){
    cout<<"Index[" << j << "] = " << m_minHeap->m_values[j] << endl;
  }
    
  cout << "----------------------------\n\n";

  cout << "Min: " << setw(5) << getMin() << endl;
  cout << "Median: " << setw(1) << getMedian() << endl;
  cout << "Max: " << setw(5) << getMax() << endl;
  cout << endl;

}

// Grade Functions

template <typename T>
int MedianHeap<T>::maxHeapSize(){
    
  return m_maxHeap->m_heapSize;
    
}

template <typename T>
int MedianHeap<T>::minHeapSize(){
    
  return m_minHeap->m_heapSize;
    
}

template <typename T>
T MedianHeap<T>::locateInMaxHeap(int pos){
    
  // Checks to see if position is out of range
  if ((pos < 1 ) || (pos > m_maxHeap->m_heapSize))
    throw out_of_range("Position is out of max heap's range");
    
  return m_maxHeap->m_values[pos];
}

template <typename T>
T MedianHeap<T>::locateInMinHeap(int pos){
    
  // Checks to see if position is out of range
  if ((pos < 1 ) || (pos > m_minHeap->m_heapSize))
    throw out_of_range("Position is out of min heap's range");
    
  return m_minHeap->m_values[pos];
    
}

#endif
