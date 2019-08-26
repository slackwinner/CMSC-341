// File: obscure.cpp
//
// UMBC CMSC 341 Fall 2017 Project 4
//
// Test obscure logic bug
//
//


#include <iostream>
using namespace std ;

#include "MedianHeap.h"

#include "City.h"

// global array of cities

int numItems = 10 ;

City cities[10] = 
{  {"Town1", "AK", 60.0, -150.0, 1000, 50},
   {"Town2", "AK", 60.0, -150.0, 1000, 50},
   {"Town3", "AK", 60.0, -150.0, 1000, 50},
   {"Town4", "AK", 60.0, -150.0, 1000, 50},
   {"Town5", "AK", 60.0, -150.0, 1000, 50},
   {"Town6", "AK", 60.0, -150.0, 1000, 50},
   {"Town7", "AK", 60.0, -150.0, 1000, 50},
   {"Town8", "AK", 60.0, -150.0, 1000, 50},
   {"Town9", "AK", 60.0, -150.0, 1000, 50},
   {"Town10", "AK", 60.0, -150.0, 1000, 50}
} ;

int main() {
   string cityName, state ;
   

   // Declare MedianHeap of City pointers
   //
   MedianHeap<City> ElevH (City::smallerElevation, City::biggerElevation, 30) ;


   // Add items to MedianHeaps
   //
   for (int i=0 ; i < numItems ; i++) {
      ElevH.insert(cities[i]) ;
      cout << "=============================================\n" ;
      ElevH.dump() ;
      cout << "=============================================\n" ;
   }


   cout << "\n\nDump of Elevation MedianHeap\n" ;
   ElevH.dump() ;


   // Test deleteItem() with Population MedianHeap
   //

   cout << "\n\nDelete some items from Elevation MedianHeap\n" ;
   bool found ;
   City item1 = {"Town9", "AK"} ;
   City item2 = {"Town10", "AK"} ;
   City item3 = {"Town1", "AK"} ;

   found = ElevH.deleteItem(item1, City::sameCity) ;
   if (found) {
      cout << "Found and deleted:\n" << item1 << endl ;
   } else {
      cout << "Did not delete:\n" << item1 << endl ;
   }

   found = ElevH.deleteItem(item2, City::sameCity) ;
   if (found) {
      cout << "Found and deleted:\n" << item2 << endl ;
   } else {
      cout << "Did not delete:\n" << item2 << endl ;
   }

   cout << "\n\nDump of Elevation MedianHeap after deleting Town9 and Town10:\n" ;
   ElevH.dump() ;


   found = ElevH.deleteItem(item3, City::sameCity) ;
   if (found) {
      cout << "Found and deleted:\n" << item3 << endl ;
   } else {
      cout << "Did not delete:\n" << item3 << endl ;
   }

   cout << "\n\nDump of Elevation MedianHeap after deleting Town1:\n" ;

   ElevH.dump() ;

   return 0 ;
}
