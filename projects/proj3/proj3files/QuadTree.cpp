//  File: QuadTree.cpp
//  Author: Dane Magbuhos
//  Date: 10/23/18
//  Title: CMSC 341 Project 3: Quad Tree For Life
//

#include "QuadTree.h"
#include <iostream>

using namespace std;

QuadTree::iterator::iterator(){
    
    
    
}

QuadTree::iterator::~iterator(){
    
    
}
bool QuadTree::iterator::operator==(const QuadTree::iterator &other){
    
  bool result = false;
    
  return result;
}
bool QuadTree::iterator::operator!=(const QuadTree::iterator &other){
    
  bool result = false;
    
  return result;
}

QuadTree::iterator & QuadTree::iterator::operator++(){
    
  return *this;
}

QuadTree::iterator QuadTree::iterator::operator++(int dummy){
    
  return *this;
}

Point & QuadTree::iterator::operator*(){
    
  Point *p  = new Point(0,0); 
    
  return *p;
}

const QuadTree::iterator & QuadTree::iterator::operator=(const iterator &rhs){
    
  return *this;
    
 }

// Creates a 16x16 default board
QuadTree::QuadTree(){
    
  m_root = new QTQuad();
  m_bounds = BBox(Point(0,0), 16);
  m_qBounds = m_bounds;
    
}

QuadTree::QuadTree(const BBox &bounds){
    
  m_root = new QTQuad();
  m_bounds = bounds;
  m_qBounds = BBox(Point(bounds.m_bL.m_x,bounds.m_bL.m_x),BBox::roundUpPow2(bounds.m_dim));
}

QuadTree::~QuadTree(){
    
  clearAll();

  delete m_root;
  m_root = NULL;
}

// Retrieve cell value
int QuadTree::get(const Point &pt){
    
  bool getBound = m_bounds.inBounds(pt);
  int value = 0;
    
  // Executes get function if point is within quadrant bounds
  if(getBound == true){
    value = m_root->get(pt, m_qBounds);
  }
    
  return value;
}

// Set cell value
void QuadTree::set(const Point &pt, int data){
    
  bool getBound = m_bounds.inBounds(pt);
    
  // Executes set function if point is within quadrant bounds
  if(getBound == true){
    m_root->set(pt,data,m_qBounds);
  }
}

// Modify cell value by a signed delta amount; faster then get() + set()
int QuadTree::increment(const Point &pt, int delta){
    
  bool getBound = m_bounds.inBounds(pt);
  int value = 0;
    
  // Executes increment function if point is within quadrant bounds
  if(getBound == true){
    value = m_root->increment(pt, delta, m_qBounds);
  }
    
  return value;
}

// Clear all cells in tree
void QuadTree::clearAll(){
  
  m_root->clearAll();
    
}

// return iterator to first non-zero cell; == end() if qtree empty
QuadTree::iterator QuadTree::begin(){
    
  QuadTree::iterator it;

  return it;
    
}

// iterator pointing beyond last cell;
QuadTree::iterator QuadTree::end(){
    
  QuadTree::iterator it;

  return it;
}



