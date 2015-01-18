
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
// no init, no  destroy
// sizeof == 4
typedef
struct  {
   int b;
} c_struct_b;

// better

struct c_struct_b0 {
   int b;
} ;


// c++ struct, no typedef,
// with init
// wirh destroy
// sizeof == 4
// with default constructor, std::vector needs this
// with one parameter contructor (type cast)
struct cpp_b {
   int b;

   // default constructur, needed for STL container
   cpp_b(): b( 0 ) {
      cout << "default constructor" << endl;
   }

   // cast constructor, casts autmatically an int to an object
   cpp_b( int i ): b( i ) {
      cout << "cast constructor" << endl;
   }
   ~cpp_b() {
      cout << "destruktor" << endl;
   }

   int getb()const {
      return b;
   }
   void setb( int b_ ) {
      b = b_;
   }
};


const size_t size = 10;

// C ways for an array
// list of pointers to objects
//   very complicated, but widely used, why? -  not known,
void c_demo_double_new( ) {

   // pointer to array of pointers
   c_struct_b** ptr_c_struct = nullptr;

   if( nullptr == ptr_c_struct ) {
      ptr_c_struct = new c_struct_b*[ size ];

      for( size_t i = 0; i < size; i++ ) {
         // pointer to new object
         ptr_c_struct[i] = new c_struct_b();
      }
   }

   // use the array
   ( *ptr_c_struct[2] ).b = 4;
   ptr_c_struct[4]->b = 7;

   // simulate early return
   if( ptr_c_struct[4]->b == 7 ) {


      // no delete[], --> memory leak
      return;
   }

   // delete at end, also complicated
   if( nullptr != ptr_c_struct ) {
      for( size_t i = 0; i < size; i++ ) {
         if( nullptr != ptr_c_struct[i] ) {
            delete ptr_c_struct[i];   //-> ?? problem with applying the 'delete' operator, which, [] or other
         }
      }

      delete[] ptr_c_struct;
      ptr_c_struct = nullptr;
   }

   return ;
}

// pointer to array of objects
// contructor didn't run, must be called by program
void c_demo_malloc( void ) {

   c_struct_b* ptr_c_struct  = nullptr;

   if( nullptr == ptr_c_struct ) {

      ptr_c_struct = static_cast< c_struct_b*>(malloc( size * sizeof( c_struct_b ) ) );
      // Konstruktor auf 'C' Art, alle Werte werden auf 0 gesetzt.
      memset( ptr_c_struct, 0, size * sizeof( c_struct_b ) );
   }

   // use the array
   c_struct_b* ptr_use = ptr_c_struct;
   ptr_use += 2;
   ptr_use->b = 4;
   ( *ptr_use ).b = 4; // ok
   //(*ptr_use) = 4;  // error, C can't cats an int to an object, C++ can do this

   ptr_use += 2;
   ptr_use->b = 7;

   // simulate early return
   if( ptr_use->b == 7 ) {

      // no delete[], --> memory leak
      return;
   }



   if( nullptr != ptr_c_struct ) {
      // no call of destructors
      // memory leak, if class contains pointers
      free( ptr_c_struct );
      ptr_c_struct = nullptr;
   }

   return ;
}


// C++ ways for an array, in old C style, dangerous

// replacement of 'malloc' with 'operator new'
// code ist similar to C code
// constructor runs for each object in 'new'
// destructor runs for each object in delete[]
void cpp_demo_new( ) {

   cpp_b* ptr_cpp_b  = nullptr;

   // Generate an array of relative cross positions
   if( nullptr == ptr_cpp_b ) {

      // constructor ist called
      ptr_cpp_b = new cpp_b[ size ];

   }

   // use the array
   ptr_cpp_b[2].b = 4;
   ( *( ptr_cpp_b + 4 ) ).b = 7; // ok
   ( *( ptr_cpp_b + 4 ) ) = 7; // also ok, cast constructor is applied, se above

   // simulate early return
   if( ptr_cpp_b[4].b == 7 ) {
      // no delete[], --> memory leak
      return;
   }

   if( nullptr != ptr_cpp_b ) {
      // Destruktor Aufruf
      delete[] ptr_cpp_b;
      ptr_cpp_b = nullptr;
   }

   return ;
}


// replacement of pointer with smart pointer
// code looks very different to C code
void cpp_demo_smart_pointer( ) {
   boost::scoped_array<cpp_b> smart_ptr_cpp_b( new cpp_b[size] );


   // use the array
   smart_ptr_cpp_b[2] = 4;  // über cast Konstruktor von cpp_b
   smart_ptr_cpp_b[4].b = 6;

   // simulate early return
   if( smart_ptr_cpp_b[4].b == 6 ) {

      // implicit delete[] and call of all destructors
      return;
   }


   std::cout << smart_ptr_cpp_b[2].b << endl;


   // implicit delete[] and call of all destructors


   return ;
}

// dynamically allocated STL vector
// very simple code
// no memory leaks possible
// constructor runs for each object in the 'vector' constructor of 'vector'
// destructor runs for each object in destructor of vector at the end of scope
void cpp_demo_STL( void ) {

   // create a vector with size elements, all are initialized
   vector<cpp_b> vector_b( size );

   // use the array
   vector_b[2] = 4;
   vector_b[4].b = 7; // cast from int to class, with cast constructor

   // simulate early return
   if( vector_b[4].b == 7 ) {

      // implicit delete[] and call of all destructors
      return;
   }

   // implicit delete[] and call of all destructors
   return ;
}


// dynamically allocated C++11 array
// very simple code
// no memory leaks possible
// fast
// constructor runs for each object in the 'array' constructor of 'array'
// destructor runs for each object in destructor of vector at the end of scope
void cpp_demo_array( void ) {

   // erzeuge size Elemente, alle sind initalisiert
   array<cpp_b, size> values;

   // use the array
   values[2].b = 4;
   values[4] = 7;
   // implicit delete[] and call of all destructors
   return ;
}



int main( void ) {
   int i = sizeof( c_struct_b );
   int b = sizeof( cpp_b );
   c_demo_double_new();
   c_demo_malloc();
   cpp_demo_new();
   cpp_demo_smart_pointer();
   cpp_demo_STL();
   return 0;
}

