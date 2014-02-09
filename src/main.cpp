
#include <string>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <typeinfo>
#include <vector>
#include <malloc.h>
#include <memory.h>
#include <array>
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>

using namespace std;
using std::vector;
using std::array;


// simple typedef for a struct
// sizeof == 4
typedef 
struct  {
	int b;
} c_struct;


// c++ struct, no typedef,
// sizeof == 4
// with default constructor, for std::vector
// with one parameter contructor (type cast)
struct cpp_a{
	int b;
	
	cpp_a():b(0){}
	cpp_a(int i):b(i){}
};


const size_t size= 10;

// C ways for an array
// list of pointers to objects
// very complicated, but widely used, why? not knwon
// contructor didn't run
void c_demo_double_new(  ) {

   c_struct** ptr_c_struct = NULL;

	// Generate an array of relative cross positions
   if( NULL == ptr_c_struct )
	{
      ptr_c_struct = new c_struct*[ size ];
      for( size_t i = 0; i<size; i++ )
		{
         ptr_c_struct[i] = new c_struct();
		}
	}
	// use the array
   (*ptr_c_struct[2]).b = 4;
   ptr_c_struct[4]->b = 7;



   if( NULL!=ptr_c_struct )
	{
      for( size_t i = 0; i < size; i++ )
		{
         if( NULL!=ptr_c_struct[i] )
            delete ptr_c_struct[i];  //-> ?? problem with applying the 'delete' operator to SVPoint
		}
      delete[] ptr_c_struct;
      ptr_c_struct = NULL;
	}

   return ;
}

// pointer to array of objects
// contructor didn't run
void c_demo_malloc( void ) {

   c_struct* ptr_c_struct  = NULL;

	// Generate an array of relative cross positions
   if( NULL == ptr_c_struct )
	{

      ptr_c_struct = (c_struct*)malloc( size * sizeof(c_struct) );
      memset( ptr_c_struct, 0, size * sizeof(c_struct) );
	}
	// use the array
   c_struct* ptr_use = ptr_c_struct;
	ptr_use += 2;
	ptr_use->b = 4;

	ptr_use += 2;
	ptr_use->b = 7;


   if( NULL!=ptr_c_struct )
	{
      free(ptr_c_struct);
      ptr_c_struct = NULL;
	}

   return ;
}


// C++ ways for an array

// replacement of 'malloc' with 'operator new'
// code ist similar to C code
// constructor runs for each object in 'new'
// destructor runs for each object in delete[]
void cpp_demo_new(  ) {

   cpp_a* ptr_cpp_a  = nullptr;

   // Generate an array of relative cross positions
   if( nullptr == ptr_cpp_a )
   {

      ptr_cpp_a = new cpp_a[ size ];

   }
   // use the array
   ptr_cpp_a[2] = 4;
   *(ptr_cpp_a + 4) = 7;


   if( nullptr!=ptr_cpp_a )
   {
      delete[] ptr_cpp_a;
      ptr_cpp_a = nullptr;
   }

  return ;
}


void cpp_demo_smart_pointer( ) {
   boost::scoped_array<cpp_a> ptr_cpp_a(new cpp_a[size]);


   // use the array
   ptr_cpp_a[2] = 4;
   ptr_cpp_a[4] = 6;

   std::cout << ptr_cpp_a[2].b << endl;

   // not allowed
   //*(ptr_cpp_a + 4) = 7;


  // implicit delete[]


  return ;
}

// dynamically allocated STL vector
// very simple code
// no memory leaks possible
// constructor runs for each object in the 'vector' constructor of 'vector'
// destructor runs for each object in destructor of vector at the end of scope
void cpp_demo_STL( void ) {
   
	vector<cpp_a> vc_a(size);

	// use the array
	vc_a[2] = 4;
	vc_a[4] = 7;

   return ;
}


// dynamically allocated C++11 array
// very simple code
// no memory leaks possible
// fast
// constructor runs for each object in the 'array' constructor of 'array'
// destructor runs for each object in destructor of vector at the end of scope
void cpp_demo_array( void ) {

   array<cpp_a,size> values;

   // use the array
   values[2] = 4;
   values[4] = 7;
   return ;
}



int main( void ) {
   int i = sizeof(c_struct);
   int b = sizeof( cpp_a);
   c_demo_double_new(); 
   c_demo_malloc();   
   cpp_demo_new();
   cpp_demo_smart_pointer();
   cpp_demo_STL();    
   return 0;
}

