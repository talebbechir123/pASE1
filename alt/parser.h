#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct {
    char *type;
    char *data;
} RESP;




// parser for redis serialization protocol
// read the first byte of the data and return the type
// + for simple string
// - for error
// : for integer
// $ for bulk string
// * for array
// _ for null
// # for boolean
// , for double
// < for big number
// ! for bulk error
// = for verbatim string
// % for maps
// ~ for sets
// > pushes

// returns the type of the data
/*
  Input:
  Simple string: +ok\r\n
  Error: -error message\r\n
  Bulk string: $5\r\nhello\r\n, $0\r\n\r\n(empty), $-1\r\n(null)
  Integer: :1000\r\n
  Array: *2\r\n$3\r\nhey\r\n$5\r\nthere\r\r(2 strings), *3\r\n:1\r\n:2\r\n:3\r\n(3 integers), *0\r\n(empty), *-1\r\n(null)
    - Nested array: *2\r\n*3\r\n:1\r\n:2\r\n:3\r\n*2\r\n+Hello\r\n-World\r\n
*/

/*
  Algorithm:
  - Read the first character to determine the input RESP type(simple string, error, integer, bulk string, array)
    - simple string: read input until CRLF, return RESP
    - error: read input until CRLF, return RESP
    - bulk string:
        - read input until CRLF to get number of bytes in the bulk string
        - read input until CRLF to read the string
        - if number of bytes match the input string, return RESP. Otherwise, return custom error
    - integer: read input until CRLF, return RESP
    - array:
        - read input until CRLF to get number of elements in the array
        - for 1..n, recursively call parse_resp() to parse each element in the array

  Utility:
    - Read input until CRLF: return a tuple of (output RESP, remaining input after CRLF)
    - Check whether we have reached the end of the input
    - Custom errors: unrecognised first character, CRLF not found, incomplete input(bulk string, array)
*/



// implement the parser
// return the type of the data
char *parse_type(char *input);



// parse a simple string
void parse_simple_string(char *input, char *output);




// parse an error
char *parse_error(char *input, char *output);

// parse an integer
char *parse_integer(char *input, char *output);

// parse a bulk string
char *parse_bulk_string(char *input, char *output);




int size_of_RESP(char *input);

// function to parse until CRLF
char *parse_until_CRLF(char *input);

//function that creates a new char* substing using the start and end index
char *substring(char *input, int start, int end);

// function that uses substring and parse_until_CRLF to parse a bulk string


// parse an array 
// get the number of elements in the array
// then put each element into the array
char **parse_array(char *input);


// function to copy a string

