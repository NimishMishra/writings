/*
* AIM: To design a cache simulator with an efficient replacement policy
* AUTHOR: Nimish Mishra
* DATE: November 10, 2019
*/

#include <iostream>
#include <cmath>
#include <string>
using namespace std;

#define TEST_CASES 10000000000s

#define CUTOFF 500    // defines the average clock ticks the processor requests for addresses from a 
                    // particular program, and therefore principles of locality may be applied.

#define CLOSENESS 4 // defines how close to the previously accessed reference the references shall be
                    // simulated

// Cache table parameters
#define CACHE_ROWS 2
#define CACHE_COLS 7

int previous_referenced_address; // stores the previously referenced address. To simulate principles of
                                // locality, the new addresses must be close to this address.
int current_referenced_address;

// variables to record the number of cache hits and cache misses
long cache_hit = 0;
long cache_miss = 0;


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

   
   
   if(request_number % CUTOFF == 0){
        // if a new program is referenced by the processor, generate a new address request
        previous_referenced_address = 0 + floor(rand() % (9));
        current_referenced_address = previous_referenced_address;
   }
    else{
        // if the same program as before is being used, generate an address close to the one already
        // generated.
        current_referenced_address = int((previous_referenced_address + floor(rand() % (CLOSENESS)))) % 10;
    }

    
    // convert the integral value to a 32 bit address
    int byte_offset_integer = 0 + rand() % 4; // generate a random byte offset
    string byte_offset = integer_to_binary_formatted(byte_offset_integer); // convert the byte offset to binary

    // working on the index bit
    string index_bit = "";
    if(current_referenced_address % 2 == 0)
        index_bit = "0";
    else
        index_bit = "1";
    
    // generating the tag
    string tag = integer_to_binary_formatted(current_referenced_address);
    string address = tag + index_bit  + byte_offset;
    
    // broadcasting such that the length of the address is 32 bit
    while(address.length() != 32)
        address = "0" + address;
    
    cout << "Integral memory: " << current_referenced_address << "- 32 bit address: " << address;
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

            if(j == 0 || j == 3) {
                cache[i][0] = "0"; // Valid bit is set to 0
                cache[i][3] = "0";
            }
            else if(j == 6)
                cache[i][j] = "0"; // Set the LRU bit to nill for now
            else
                cache[i][j] = "-"; // set the rest of the values to NULL
        }
    }
}

void print_cache(){
    cout << "---------------------------------------------------" << endl;
    for (int i = 0; i  < CACHE_ROWS; i++){
        for(int j = 0; j < CACHE_COLS; j++)
            cout << cache[i][j] << " ";
        cout << endl;
    }
    cout << endl << "---------------------------------------------------" << endl;
}


void load_spatial_local_data(){
    /*
    *   @PARAMS: none
    *   @RETURN: none
    *   this function uses the concept of spatial locality to load data near what is curretly 
    *   referenced
    */

   // spatial data to be loaded
    int to_load_one =  (current_referenced_address + 1) % 10;
    int to_load_two = (current_referenced_address + 2) % 10;
    // generate address for these request 1
    int index_bit_integer;
    int byte_offset_integer = 0 + rand() % 4; // generate a random byte offset
    string byte_offset = integer_to_binary_formatted(byte_offset_integer); // convert the byte offset to binary
    // working on the index bit
    string index_bit = "";
    if(to_load_one % 2 == 0){
        index_bit = "0";
        index_bit_integer = 0;
    }
    else{
        index_bit = "1";
        index_bit_integer = 1;
    }
    // generating the tag
    string tag = integer_to_binary_formatted(to_load_one);
    string address1 = tag + index_bit  + byte_offset;
    // broadcasting such that the length of the address is 32 bit
    while(address1.length() != 32)
        address1 = "0" + address1;
    
    // load the data into the cache at the specified LRU_bit

    string LRU_string = cache[index_bit_integer][6];
   int LRU_integer;
   if(LRU_string == "0") {
       LRU_integer = 0;
       cache[index_bit_integer][0] = "1";
       cache[index_bit_integer][1] = address1.substr(0, 29);
       cache[index_bit_integer][2] = "Memory at " + address1;
    }
   else if(LRU_string == "1") {
       LRU_integer = 1;
        cache[index_bit_integer][3] = "1";
       cache[index_bit_integer][4] = address1.substr(0, 29);;
       cache[index_bit_integer][5] = "Memory at " + address1;
    }

//     //Generate address for request 2

    
    byte_offset_integer = 0 + rand() % 4; // generate a random byte offset
    byte_offset = integer_to_binary_formatted(byte_offset_integer); // convert the byte offset to binary
    // working on the index bit
    index_bit = "";
    if(to_load_two % 2 == 0){
        index_bit = "0";
        index_bit_integer = 0;
    }
    else{
        index_bit = "1";
        index_bit_integer = 1;
    }
    // generating the tag
    tag = integer_to_binary_formatted(to_load_two);
    string address2 = tag + index_bit  + byte_offset;
    // broadcasting such that the length of the address is 32 bit
    while(address2.length() != 32)
        address2 = "0" + address2;
    
//     // load the data into the cache at the specified LRU_bit

    LRU_string = cache[index_bit_integer][6];
    if(LRU_string == "0") {
       LRU_integer = 0;
       cache[index_bit_integer][0] = "1";
       cache[index_bit_integer][1] = address2.substr(0, 29);
       cache[index_bit_integer][2] = "Memory at " + address2;
    }
    else if(LRU_string == "1") {
       LRU_integer = 1;
        cache[index_bit_integer][3] = "1";
       cache[index_bit_integer][4] = address2.substr(0, 29);
       cache[index_bit_integer][5] = "Memory at " + address2;
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
    *       0              1        2              3            4           5         6
    *   Valid_bit 1a    Tag 1a    Data 1a     Valid_bit 1b    Tag 1b      Data 1b   LRU_bit
    *   Valid_bit 2a    Tag 2a    Data 2a     Valid_bit 2b    Tag 2b      Data 2b   LRU_bit
    */

    string byte_offset = address.substr(30,32);
    int index_integer = address.at(29) - 48;
    string tag = address.substr(0, 29);

    /* Cache access mechanism is as follows:
    * LRU accessed by [index_integer][6]
    *  For index_integer = 0, LRU_bit = 0:   access [0][0] for valid bit, [0][1] for tag, [0][2] for data
    *  For index_integer = 0, LRU_bit = 1:   access [0][3] for valid bit, [0][4] for tag, [0][5] for data
    * 
    *  For index_integer = 1, LRU_bit = 0:   access [1][0] for valid bit, [1][1] for tag, [1][2] for data
    *  For index_integer = 0, LRU_bit = 1:   access [1][3] for valid bit, [1][4] for tag, [1][5] for data
    */ 

   string LRU_string = cache[index_integer][6];
   int LRU_integer;
   if(LRU_string == "0") {
       LRU_integer = 0;
    }
   else if(LRU_string == "1") {
       LRU_integer = 1;
    }

    if(cache[index_integer][1] == tag || cache[index_integer][4] == tag) // first check for cache hits
    {
        cache_hit ++; // cache hit occurs
        // nothing more to do
        //cout << "Cache hit" << endl;
    }
    
    // if a cache miss occurs, search for the least recently used block and replace accordingly
    else if(LRU_integer == 0)
    {

        cache[index_integer][6] = "1";
        if(cache[index_integer][0] == "0") // if the valid bit is 0
        { 
            cache_miss ++; // record a cache miss
            cache[index_integer][0] = "1"; // change the valid bit to 1
            cache[index_integer][1] = tag; // load the tag into the cache
            cache[index_integer][2] = "Memory at " + address; // load the tag into the cache
            //cout << "Invalid Cache miss" << endl;
        }

        else // some other memory already exits here 
        {
            // implement write back or write through in a future version
            cache_miss ++;
            cache[index_integer][1] = tag; // load the tag into the cache
            cache[index_integer][2] = "Memory at " + address; // load the tag into the cache
            //cout << "Valid cache miss" << endl;
        }
        load_spatial_local_data();
    }

    else if(LRU_integer == 1)
    {   
        cache[index_integer][6] = "0";
        if(cache[index_integer][3] == "0") // if the valid bit is 0
        { 
            cache_miss ++; // record a cache miss
            cache[index_integer][3] = "1"; // change the valid bit to 1
            cache[index_integer][4] = tag; // load the tag into the cache
            cache[index_integer][5] = "Memory at " + address; // load the tag into the cache
            //cout << "Invalid Cache miss" << endl;
        }

        else // some other memory already exits here 
        {
            // implement write back or write through in a future version
            cache_miss ++;
            cache[index_integer][4] = tag; // load the tag into the cache
            cache[index_integer][5] = "Memory at " + address; // load the tag into the cache
            //cout << "Valid cache miss" << endl;
        }
        //load_spatial_local_data();
    }
    
}


int main(){

    unit_tests();
    initialize_cache();
    // simualate the cache
    cout << "Addresses generated: " << endl;
    string address;
    for (long i = 0; i < TEST_CASES; i++){
        address = simulate_addresses(i);
        update_cache(address);
        cout << " Test cases left: " << (TEST_CASES - i) << endl;
        //print_cache(); //<-- uncomment to observe the cache update through time
    }
    cout << "Hit rate: " << (float(cache_hit)/(cache_hit + cache_miss)) * 100 << endl;
    cout << "Miss rate: " << (float(cache_miss)/(cache_hit + cache_miss)) * 100  << endl;
}