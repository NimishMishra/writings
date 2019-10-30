/*
* AIM: To design a cache simulator with an efficient replacement policy
* AUTHOR: Nimish Mishra
* DATE: October 30, 2019
*/

#include <iostream>
#include <cmath>
#include <string>
using namespace std;


#define CUTOFF 10    // defines the average clock ticks the processor requests for addresses from a 
                    // particular program, and therefore principles of locality may be applied.

#define CLOSENESS 3 // defines how close to the previously accessed reference the references shall be
                    // simulated

// Cache table parameters
#define CACHE_ROWS 2
#define CACHE_COLS 7

int previous_referenced_address; // stores the previously referenced address. To simulate principles of
                                // locality, the new addresses must be close to this address.


string integer_to_binary(int number){
    /*
    *   @ PARAMS: number - a number to be converted into binary
    *   @ RETURN: the binary representation of the number in the parameter
    *   this function calculates the binary representation of the number sent as the parameter
    */
    
    if(number == 0)
        return "0";
    if(number == 1)
        return "1";
    int remainder_integer = number % 2;
    return integer_to_binary(number/2) + to_string(remainder_integer);
}

string integer_to_binary_formatted(int number){
    /*
    *   @ PARAMS: number - a number to be converted into binary
    *   @ RETURN: the binary representation of the number in the parameter
    *   uses the function INTEGER_TO_BINARY to calculate the binary representation and formats edge cases
    */

   string binary = integer_to_binary(number);
   // If the binary is one of the edge cases, format accordingly
   if(binary == "0") binary = "00"; 
   else if(binary == "1") binary = "01";
   return binary;
}


string simulate_addresses(long request_number){
    /*
    *   @ PARAMS: request_number - denotes which request has been made by the processor
    *   @ RETURN: a 32-bit address returned as a string
    *   Simulates the processor that generates address requests to the cache.
    *   Follows the principles of locality to simulate the processor address requests.
    */

   
   if(request_number % CUTOFF == 0)
        // if a new program is referenced by the processor, generate a new address request
        previous_referenced_address = 0 + floor(rand() % (9));
    else
        // if the same program as before is being used, generate an address close to the one already
        // generated.
        previous_referenced_address = previous_referenced_address + floor(rand() % (CLOSENESS));
        if(previous_referenced_address > 9)
            previous_referenced_address -= 9; // warping

    
    // convert the integral value to a 32 bit address
    int byte_offset_integer = 0 + rand() % 4; // generate a random byte offset
    string byte_offset = integer_to_binary_formatted(byte_offset_integer); // convert the byte offset to binary

    // working on the index bit
    string index_bit = "";
    if(previous_referenced_address % 2 == 0)
        index_bit = "0";
    else
        index_bit = "1";
    
    // generating the tag
    string tag = integer_to_binary_formatted(previous_referenced_address);
    string address = tag + index_bit  + byte_offset;
    
    // broadcasting such that the length of the address is 32 bit
    while(address.length() != 32)
        address = "0" + address;
    
    cout << previous_referenced_address << ": " << address << endl;
    return address;
}


void unit_tests(){

    // testing integer to binary conversion
    cout << "TESTING integer to binary conversion..." << endl;
    if(integer_to_binary(0) == "0") cout << "Test 1 passed" << endl;
    if(integer_to_binary(1) == "1") cout << "Test 2 passed" << endl;
    if(integer_to_binary(2) == "10") cout << "Test 3 passed" << endl;
    if(integer_to_binary(3) == "11") cout << "Test 4 passed" << endl;
}

string cache [CACHE_ROWS][CACHE_COLS];
/*  The cache is organized as follows:
*   Two way set associative cache:
*   Valid_bit 1a    Tag 1a    Data 1a     Valid_bit 1b    Tag 1b      Data 1b   LRU_bit
*   Valid_bit 2a    Tag 2a    Data 2a     Valid_bit 2b    Tag 2b      Data 2b   LRU_bit
*/


void initialize_cache(){

    /*
    *   @PARAMS: None
    *   @RETURN: None
    *   Function: helps to initialize the cache with the initial values.
    */
    for (int i = 0; i < CACHE_ROWS; i++){
        for(int j = 0; j < CACHE_COLS; j++){

            if(j == 0) 
                cache[i][0] = "0"; // Valid bit is set to 0
            else if(j == 6)
                cache[i][j] = "0"; // Set the LRU bit to nill for now
            else
                cache[i][j] = "-"; // set the rest of the values to NULL
        }
    }
}

void update_cache(string address){
    /*
    *   @PARAMS: address - the address requested by the processor
    *   @RETURN: none
    *   this function serves to perform one operation on the cache
    */


   /*  The cache is organized as follows:
    *   Two way set associative cache:
    *   Valid_bit 1a    Tag 1a    Data 1a     Valid_bit 1b    Tag 1b      Data 1b   LRU_bit
    *   Valid_bit 2a    Tag 2a    Data 2a     Valid_bit 2b    Tag 2b      Data 2b   LRU_bit
    */

    string byte_offset = address.substr(30,32);
    int index_integer = address.at(29) - 48;
    string tag = address.substr(0, 29);

    
}


int main(){

    unit_tests();
    initialize_cache();
    // simualate the cache
    cout << "Addresses generated: " << endl;
    string address;
    for (long i = 0; i <= 40; i++){
        address = simulate_addresses(i);
        update_cache(address);
    }
}