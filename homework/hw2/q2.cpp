#include <iostream>
using namespace std ;

void initialize(int * &ptr, int size){

  // Creates an array to hold the int vales
 ptr = new int [size];

 // Populates array with corresponding values
 for(int i = 0; i < size; i++){

   ptr[i] = i;
   
 }

}

int main() {
   int *ptr ;
   int n = 10 ;

   initialize(ptr,n) ;

   for (int i=0 ; i < n ; i++) {
      cout << "ptr[" << i << "] = " << ptr[i] << endl  ;
   }

   int sum = 0 ;
   for (int i=0 ; i < n ; i++) {
      sum += ptr[i] ;
   }
   cout << "Sum = " << sum << endl ;

   delete [] ptr ;

   int *ptr2 ;
   initialize(ptr2,15) ;

   sum = 0 ;
   for (int i=0 ; i < 15 ; i++) {
      sum += ptr2[i] ;
   }
   cout << "Sum = " << sum << endl ;

   delete [] ptr2 ;
}
