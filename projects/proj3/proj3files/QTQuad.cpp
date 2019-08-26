//  File: QTQuad.cpp
//  Author: Dane Magbuhos
//  Date: 10/23/18
//  Title: CMSC 341 Project 3: Quad Tree For Life
//

#include "QTQuad.h"
#include <iostream>

using namespace std;

QTQuad::QTQuad(){
    
  for(int i = 0; i < QTQ_NUM_QUADS; i++){
    m_quads[i] = NULL;
  }
    
  m_cells = NULL;
        
}

QTQuad::~QTQuad(){
   
  // Clears out all empty and populated quadrants
  clearAll();
}

int QTQuad::wantedQuad(const Point &pt, const BBox &bounds){
    
  int quad = (bounds.m_dim / 2);
  int boundsX = bounds.m_bL.m_x + quad;
  int boundsY = bounds.m_bL.m_y + quad;
  int x_coor = pt.m_x;
  int y_coor = pt.m_y;
  int quadOne = 0;
  int quadTwo = 1;
  int quadThree = 2;
  int quadFour = 3;
  int correctQuad = 0;
    
  // Finds the quadrant within coordinate point range
  if((x_coor < boundsX) && (y_coor < boundsY)){
    correctQuad = quadOne;
  }
  else if((x_coor >= boundsX) && (y_coor < boundsY)){
    correctQuad = quadTwo;
  }
    
  else if((x_coor < boundsX) && (y_coor >= boundsY)){
    correctQuad = quadThree;
  }
    
  else if((x_coor >= boundsX) && (y_coor >= boundsY)){
    correctQuad = quadFour;
  }
    
  return correctQuad;
}

void QTQuad::pruneTree(int quadrant){

  // Traverses through each quadrants' sub nodes and deletes them if empty
  for(int i = 0; i < QTQ_NUM_QUADS; i++){
        
    if((m_quads[quadrant]->m_quads[i] == NULL) && (m_quads[quadrant]->m_cells == NULL)){
           
      delete m_quads[quadrant]->m_quads[i];
      m_quads[quadrant]->m_quads[i] = NULL;
           
    }
  }

}

int QTQuad::get(const Point &pt, const BBox &bounds){
    
  int quad = bounds.m_dim / 2;
  int x_coor = 0;
  int y_coor = 0;
  int value = 0;
  int correctQuad = wantedQuad(pt, bounds); // Examines the quadrant the point is in
    
  if(bounds.m_dim == QTQ_NUM_QUADS){
        
    // Extracts data at specified coordinate points
    x_coor = pt.m_x - bounds.m_bL.m_x;
    y_coor = pt.m_y - bounds.m_bL.m_y;
        
    value = m_cells[x_coor][y_coor];      
  }
  // Checks to see if current quad is a internal or leaf node
  else if(m_quads[correctQuad] != NULL){
            
      // Calculates new bounds to zoom in on target point

      // Represents Quadrant One
      if(correctQuad == 0){
                
	BBox bound (bounds.m_bL, quad);
	value = m_quads[0]->get(pt, bound);
      }
            
      // Represents Quadrant Two
      else if(correctQuad == 1){
                
	Point newBL(bounds.m_bL.m_x + quad, bounds.m_bL.m_y);
	BBox bound (newBL, bounds.m_dim / 2);
	value = m_quads[1]->get(pt, bound);
      }
            
      // Represents Quadrant Three
      else if(correctQuad == 2){
                
	Point newBL (bounds.m_bL.m_x, bounds.m_bL.m_y + quad);
	BBox bound (newBL, bounds.m_dim / 2);
	value = m_quads[2]->get(pt, bound);
      }
            
      // Represents Quadrant Four
      else if(correctQuad == 3){
                
	Point newBL (bounds.m_bL.m_x + quad, bounds.m_bL.m_y + quad);
	BBox bound (newBL, bounds.m_dim / 2);
	value = m_quads[3]->get(pt, bound);
      }
        
  }
    
  return value;
}

// Set cell value
void QTQuad::set(const Point &pt, int data, const BBox &bounds){
    
  int quad = bounds.m_dim / 2;
  int x_coor = 0, y_coor = 0;
  int z = 0, j = 0, inUse = 0;
  int quadOne = 0, quadTwo = 1, quadThree = 2, quadFour = 3;
  int correctQuad = wantedQuad(pt, bounds); // Examines the quadrant the point is in
    
  // Reached a quadrant that is 4X4
  if(bounds.m_dim == QTQ_GRID_DIM){
        
    if(m_cells == NULL){
            
      m_cells = new int[QTQ_GRID_DIM][QTQ_GRID_DIM];
            
      // Populates new cells with 0's
      for(int i = 0; i < QTQ_GRID_DIM;i++){
	for(int j = 0; j < QTQ_GRID_DIM; j++){
                    
	  m_cells[i][j] = 0;
	}
      }
    }
        
    // Extracts data at specified coordinate points
    x_coor = pt.m_x - bounds.m_bL.m_x;
    y_coor = pt.m_y - bounds.m_bL.m_y;
        
    m_cells[x_coor][y_coor] = data;


    // Checks to see if grid has empty values
    while((z < QTQ_NUM_QUADS) && (inUse == 0)){
      while((j < QTQ_NUM_QUADS) && (inUse == 0)){
                
	// Indicates that quadrant is not empty
	if(m_cells[z][j] != 0){
	  inUse++;
	}
	j++;
      }
      j = 0;
      z++;
    }
        
    // Deletes empty cell
    if(inUse == 0){
      delete [] m_cells;
      m_cells = NULL;
            
    }
  }
    
  else{
    // Checks to see if current quad is a internal or leaf node
    if(m_quads[correctQuad] == NULL){
      m_quads[correctQuad] = new QTQuad();
        
    }
        
    // Calculates new bounds to zoom in on target point
    if(correctQuad == quadOne){
        
      BBox bound (bounds.m_bL, bounds.m_dim / 2);
      m_quads[quadOne]->set(pt, data, bound);

      /// Trims off empty quadrants
      pruneTree(quadOne);
    }
        
    else if(correctQuad == quadTwo){
        
      Point newBL (bounds.m_bL.m_x + quad, bounds.m_bL.m_y);
      BBox bound (newBL, bounds.m_dim / 2);
      m_quads[quadTwo]->set(pt,data, bound);
      
      pruneTree(quadTwo);
            
    }
        
    else if(correctQuad == quadThree){
      Point newBL (bounds.m_bL.m_x, bounds.m_bL.m_y + quad);
      BBox bound (newBL, bounds.m_dim / 2);
      m_quads[quadThree]->set(pt, data, bound);

      pruneTree(quadThree);
    }
        
    else if(correctQuad == quadFour){
        
      Point newBL (bounds.m_bL.m_x + quad, bounds.m_bL.m_y + quad);
      BBox bound (newBL, bounds.m_dim / 2);
      m_quads[quadFour]->set(pt, data, bound);

      pruneTree(quadFour);
    }
  }
        
}

// Modify cell value by a signed delta amount; faster then get() + set()
int QTQuad::increment(const Point &pt, int delta, const BBox &bounds){
    
  int x_coor = 0;
  int y_coor = 0;
  int data = 0;
  int quad = bounds.m_dim / 2;
  int z = 0, j = 0, inUse = 0;
  int quadOne = 0, quadTwo = 1, quadThree = 2, quadFour = 3;
  int correctQuad = wantedQuad(pt, bounds); // Examines the quadrant the point is in
    
  if(bounds.m_dim == QTQ_GRID_DIM){
        
    if(m_cells == NULL){
            
      m_cells = new int[QTQ_GRID_DIM][QTQ_GRID_DIM];
            
      // Populates new cells with 0's
      for(int i = 0; i < QTQ_GRID_DIM;i++){
	for(int j = 0; j < QTQ_GRID_DIM; j++){
                    
	  m_cells[i][j] = 0;
	}
      }
    }
        
    // Extracts data at specified coordinate points
    x_coor = pt.m_x - bounds.m_bL.m_x;
    y_coor = pt.m_y - bounds.m_bL.m_y;
        
    data = m_cells[x_coor][y_coor];
    data = data + delta;
    m_cells[x_coor][y_coor] = data;

    // Trims off empty quadrants
    cout << "Prune Tree: INCREMENT" << endl;
        
    // Checks to see if grid has empty values
    while((z < QTQ_NUM_QUADS) && (inUse == 0)){
      while((j < QTQ_NUM_QUADS) && (inUse == 0)){
                
	// Indicates that quadrant is not empty
	if(m_cells[z][j] != 0){
	  inUse++;
	}
	j++;
      }
      j = 0;
      z++;
    }
        
    // Deletes empty cell
    if(inUse == 0){
      delete [] m_cells;
      m_cells = NULL;
            
    }
  }
    
  else{
    // Checks to see if current quad is a internal or leaf node
    if(m_quads[correctQuad] == NULL){
      m_quads[correctQuad] = new QTQuad();
            
    }
        
    // Calculates new bounds to zoom in on target point
    if(correctQuad == quadOne){
            
      BBox bound (bounds.m_bL, bounds.m_dim / 2);
      data = m_quads[quadOne]->increment(pt, delta, bound);
      
      // Trims off empty quadrants
      pruneTree(quadOne);
       
    }
        
    else if(correctQuad == quadTwo){
            
      Point newBL (bounds.m_bL.m_x + quad, bounds.m_bL.m_y);
      BBox bound (newBL, bounds.m_dim / 2);
      data = m_quads[quadTwo]->increment(pt,delta, bound);

      pruneTree(quadTwo);
            
    }
        
    else if(correctQuad == quadThree){
      Point newBL (bounds.m_bL.m_x, bounds.m_bL.m_y + quad);
      BBox bound (newBL, bounds.m_dim / 2);
      data = m_quads[2]->increment(pt, delta, bound);

      pruneTree(quadThree);
    }
        
    else if(correctQuad == quadFour){
            
      Point newBL (bounds.m_bL.m_x + quad, bounds.m_bL.m_y + quad);
      BBox bound (newBL, bounds.m_dim / 2);
      data = m_quads[quadFour]->increment(pt, delta, bound);

      pruneTree(quadFour);
    }
  }
    
  return data;
}

// Clear all cells in subtree
void QTQuad::clearAll(){
    
  for(int i = 0; i < QTQ_GRID_DIM; i++){
        
    // Traverses down quadrant to find leaf node
    if(m_quads[i] != NULL){
      m_quads[i]->clearAll();
            
      // Deletes examined parent quadrant
      delete m_quads[i];
      m_quads[i] = NULL;
    }
        
    // Leaf node is found, clear entire 2D array
    else if(m_quads[i] == NULL){
            
      delete [] m_cells;
      m_cells = NULL;
            
    }
  }



}
