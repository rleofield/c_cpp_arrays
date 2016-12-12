#include <string>
#include <iostream>
#include <vector>
#include <memory.h>
#include <array>
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>

using std::string;
using std::vector;
using std::array;


// simple typedef for a struct
// no init, no  destroy
// sizeof == 4
typedef struct  {
    int b;
} c_struct_b;

// better, puiblic class
struct c_struct_b0 {
    int b;
};


// c++ struct, no typedef,
// with init
// wirh destroy
// sizeof == 4
// with default constructor, std::vector needs this
// with 'one parameter contructor' as type cast int to struct

struct cpp_b {
    int b;

    // default constructur, needed for STL container
    cpp_b(): b( 0 ) {
        std::cout << "default constructor" << std::endl;
    }

    // cast constructor, casts autmatically an int to an object
    cpp_b( int i ): b( i ) {
        std::cout << "cast constructor" << std::endl;
    }
    ~cpp_b() {
        std::cout << "destruktor" << std::endl;
    }

    int getb()const {
        return b;
    }
    void setb( int b_ ) {
        b = b_;
    }
};

class t_scope_log {
   string _f;
public:
   t_scope_log( string const& f ) : _f( f ) {
      std::cout << "-- start func: '" + _f + "'" << std::endl;
   }
   ~t_scope_log() {
      std::cout << "-- end func:   '" + _f + "'\n" << std::endl;
   }
};

#define __scope_log__ t_scope_log scope_log(__FUNCTION__);


namespace local{
    const size_t size = 10;
    bool debug_stop = false; // line for breakpoint in debugger
}

void demo_sizeof_structs( ) {
    __scope_log__
    // sizeof of the used structs
    size_t c = sizeof( c_struct_b ); // == 4, contains 1 member
    size_t cpp = sizeof( cpp_b );      // == 4, contains 1 member and many functions
    local::debug_stop = true;
}





// old C way for an array
// list of pointers to objects
// very complicated, but widely used, why?
void cpp_demo_c_style( ) {
    __scope_log__
    size_t size = local::size;

    // pointer to array of pointers
    c_struct_b** ptr_ptr_c_struct = nullptr;

    if( nullptr == ptr_ptr_c_struct ) {
        ptr_ptr_c_struct = new c_struct_b*[ size ];
        for( size_t i = 0; i < size; i++ ) {
            // pointer to new objects
            ptr_ptr_c_struct[i] = new c_struct_b();
        }
    }

    // use the array
    ( *ptr_ptr_c_struct[2] ).b = 444;
    ptr_ptr_c_struct[4]->b = 6; //7;
    int control = ptr_ptr_c_struct[2]->b;

    // simulate early return
    if( ptr_ptr_c_struct[4]->b == 7 ) {
        // no delete[], --> memory leak
        std::cout << "memory leak" << std::endl;
        return;
    }

    // delete at end, very complicated
    if( nullptr != ptr_ptr_c_struct ) {
        for( size_t i = 0; i < size; i++ ) {
            if( nullptr != ptr_ptr_c_struct[i] ) {
                delete ptr_ptr_c_struct[i];   //-> ?? problem with applying the 'delete' operator, which, [] or other
            }
        }
        delete[] ptr_ptr_c_struct;
        ptr_ptr_c_struct = nullptr;
    }
    local::debug_stop = true;

}

// pointer to array of objects
// constructor didn't run, must be called by program
void c_demo_malloc( void ) {
    __scope_log__

    size_t size = local::size;

    // pointer to array
    c_struct_b* ptr_c_struct  = nullptr;

    if( nullptr == ptr_c_struct ) {

        ptr_c_struct = static_cast< c_struct_b*>(malloc( size * sizeof( c_struct_b ) ) );
        // 'C' style 'constructor', set all values to '0'
        memset( ptr_c_struct, 0, size * sizeof( c_struct_b ) );
    }

    // use the array
    c_struct_b* ptr_use = ptr_c_struct;
    ptr_use += 2;
    ptr_use->b = 4;
    ( *ptr_use ).b = 444; // ok
    //(*ptr_use) = 4;  // error, C can't cast an int to an object, C++ can do this

    ptr_use += 2;
    ptr_use->b = 6;

    int control = ptr_c_struct[2].b;


    // simulate early return
    if( ptr_use->b == 7 ) {

        // no delete[], --> memory leak
        std::cout << "memory leak" << std::endl;
        return;
    }



    if( nullptr != ptr_c_struct ) {
        // no call of destructors
        // memory leak, if class contains pointers
        free( ptr_c_struct );
        ptr_c_struct = nullptr;
    }
    local::debug_stop = true;
}


// C++ ways for an array, in old C style, dangerous

// replacement of 'malloc' with 'operator new'
// code ist similar to C code
// constructor runs for each object in 'new'
// destructor runs for each object in delete[]
void cpp_demo_new( ) {
    __scope_log__

    size_t size = local::size;


    cpp_b* ptr_cpp_b  = nullptr;

    // pointer to array of elements
    if( nullptr == ptr_cpp_b ) {
        // constructor ist called for each element
        ptr_cpp_b = new cpp_b[ size ];

    }

    // use the array
    ptr_cpp_b[2].b = 444;
    ( *( ptr_cpp_b + 4 ) ).b = 7; // ok
    ( *( ptr_cpp_b + 4 ) ) = 6; // also ok, cast constructor is applied, se above

    int control = ptr_cpp_b[2].b;


    // simulate early return
    if( ptr_cpp_b[4].b == 7 ) {
        // no delete[], --> memory leak
        std::cout << "memory leak" << std::endl;
        return;
    }

    if( nullptr != ptr_cpp_b ) {
        // Destruktor Aufruf
        delete[] ptr_cpp_b;
        ptr_cpp_b = nullptr;
    }
    local::debug_stop = true;
}


// replacement of pointer with smart pointer
// code looks very different to C code
void cpp_demo_smart_pointer( ) {
    __scope_log__

    size_t size = local::size;


    boost::scoped_array<cpp_b> smart_ptr_cpp_b( new cpp_b[size] );


    // use the array
    smart_ptr_cpp_b[2] = 444;  // use cast constructor of class 'cpp_b'
    smart_ptr_cpp_b[4].b = 5;

    int control = smart_ptr_cpp_b[2].b;

    // simulate early return
    if( smart_ptr_cpp_b[4].b == 6 ) {

        // implicit delete[] and call of all destructors
        // no memory laek
        std::cout << "no memory leak" << std::endl;
        return;
    }

    local::debug_stop = true;
    // implicit delete[] and call of all destructors

}

// dynamically allocated STL vector
// very simple code
// no memory leaks possible
// constructor runs for each object in the 'vector' constructor of 'vector'
// destructor runs for each object in destructor of vector at the end of scope
void cpp_demo_STL() {
    __scope_log__

    size_t size = local::size;


    // create a vector with size elements, all are initialized
    vector<cpp_b> vector_b( size );

    // use the array
    vector_b[2] = 4;
    vector_b[4].b = 7; // cast from int to class, with cast constructor

    // simulate early return
    if( vector_b[4].b == 7 ) {

        // implicit delete[] and call of all destructors
        std::cout << "no memory leak" << std::endl;
        return;
    }

    local::debug_stop = true;
    // implicit delete[] and call of all destructors
}


// dynamically allocated C++11 array
// very simple code
// no memory leaks possible
// fast
// constructor runs for each object in the 'array' constructor of 'array'
// destructor runs for each object in destructor of vector at the end of scope
void cpp_demo_array( void ) {
    __scope_log__;

    const size_t size = local::size;


    // erzeuge size Elemente, alle sind initalisiert
    array<cpp_b, size> values;

    // use the array
    values[2].b = 4;
    values[4] = 7;
    local::debug_stop = true;
    // implicit delete[] and call of all destructors
}



// use debugger to stop in program and view values
int main( void ) {
    __scope_log__;

    // test of exception in string( const CharT* s,)
    string msg;
    try{
        std::string s(nullptr);
    }
    catch( std::exception &ex)
    {
        msg = ex.what();  //  "basic_string::_M_construct null not valid"
    }

    demo_sizeof_structs( );
    cpp_demo_c_style();
    c_demo_malloc();
    cpp_demo_new();
    cpp_demo_smart_pointer();
    cpp_demo_STL();
    return 0;
}

