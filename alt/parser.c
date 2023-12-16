#include "parser.h"
#include <string.h>


// returns the type of the data

char *parse_type(char *input) {
    char *type = malloc(sizeof(char));
    *type = input[0];
    return type;
}   

void parse_simple_string(char *input, char *output) {
    // remove the type from the input
    char *input2 = input + 1;
    // read input until CRLF
    int i = 0;
    while (input2[i] != '\r') {
        output[i] = input2[i];
        i++;
    }
    output[i] = '\0';
    // remove the CRLF from the output end 
   
   
}

char *parse_bulk_string(char *input, char *output){
    
    // read input until CRLF to get number of bytes in the bulk string
    int i = 1;
    int j = 0;
    char *num_bytes = malloc(sizeof(char) * 100);
    while (input[i] != '\r') {
        num_bytes[j] = input[i];
        i++;
        j++;
    }
    num_bytes[j] = '\0';
    int num_bytes_int = atoi(num_bytes);

    // use num_bytes to read input until CRLF to read the string
    int k = 0;
    int l = 0;
    char *bulk_string = malloc(sizeof(char) * 100);
    while (k < num_bytes_int) {
        bulk_string[l] = input[i + 2 + k];
        k++;
        l++;
    }

    bulk_string[l] = '\0';
    return bulk_string;
}


char *parse_integer(char *input, char *output){
    // remove the type from the input
    char *input2 = input + 1;
    // read input until CRLF
    int i = 0;
    while (input2[i] != '\r') {
        output[i] = input2[i];
        i++;
    }
    output[i] = '\0';
    return output;
}


char *parse_error(char *input, char *output){
    // remove the type from the input
    char *input2 = input + 1;
    // read input until CRLF
    int i = 0;
    while (input2[i] != '\r') {
        output[i] = input2[i];
        i++;
    }
    output[i] = '\0';
    return output;
}


int size_of_RESP(char *input) {
    // read the first character to determine the input RESP type
    char *type = parse_type(input);
    // simple string: +ok\r\n
    if (*type == '+') {
        return 1;
    }
    // error: -error message\r\n
    else if (*type == '-') {
        return 1;
    }
    // bulk string: $5\r\nhello\r\n, $0\r\n\r\n(empty), $-1\r\n(null)
    else if (*type == '$') {
        // read input until CRLF to get number of bytes in the bulk string
        int i = 1;
        int j = 0;
        char *num_bytes = malloc(sizeof(char) * 100);
        while (input[i] != '\r') {
            num_bytes[j] = input[i];
            i++;
            j++;
        }
        num_bytes[j] = '\0';
        int num_bytes_int = atoi(num_bytes);
        return num_bytes_int;
    }
    // integer: :1000\r\n
    else if (*type == ':') {
        return 1;
    }

    // array: *2\r\n$3\r\nhey\r\n$5\r\nthere\r\r(2 strings), *3\r\n:1\r\n:2\r\n:3\r\n(3 integers), *0\r\n(empty), *-1\r\n(null)
    else if (*type == '*') {
        // read input until CRLF to get number of elements in the array
        int i = 1;
        int j = 0;
        char *num_elements = malloc(sizeof(char) * 100);
        while (input[i] != '\r') {
            num_elements[j] = input[i];
            i++;
            j++;
        }
        num_elements[j] = '\0';
        int num_elements_int = atoi(num_elements);
        return num_elements_int;
    }
    else {
        return -1;
    }



}

char **parse_array(char *input){

    
    // read input until CRLF to get number of elements in the array
    int size = size_of_RESP(input);
    char STRING_ARRAY[size];
   
   // remove the first element from the input
    char *input2 = input + 4;
   printf("input2: %s\n", input2);
    // for 1..n, recursively call parse_resp() to parse each element in the array
    int i = 0;
    int j = 0;
    char *ARRAY[size];
    char *output = malloc(sizeof(char) * 100);
    while(i <size){
        char *type = parse_type(input2);
        if (*type == '+') {
            char *simple_string =  malloc(sizeof(char) * 100);
            parse_simple_string(input2, output);
            input2 = input2 + strlen(simple_string) + 2 +4;
           // add the simple string to the output array
            printf("simple string: %s\n", simple_string);
            ARRAY[j] = simple_string;
            // use strcpy to copy the string to the array
           
            //output[j] = *simple_string;
             strcpy(&output[j],simple_string);
            j++;
        }else if (*type == '-') {
            char *error = parse_error(input2, output);
            input2 = input2 + strlen(error) + 2+4;
            // add the error to the output array
             strcpy(&output[j],error);
              ARRAY[j] = error;
            j++;
        }else if (*type == '$') {
            char *bulk_string = parse_bulk_string(input2, output);
            //get the length of the bulk string
            int length = strlen(bulk_string);
            input2 = input2 + strlen(bulk_string) + 2 +4 ;
            // add the bulk string to the output array
            printf("bulk string: %s\n", bulk_string);
            strcpy(&output[j], bulk_string);
            //output[i] = *bulk_string;
             ARRAY[j] = bulk_string;
            j++;

        }else if (*type == ':') {
            char *integer = parse_integer(input2, output);
            input2 = input2 + strlen(integer) + 2+4;
            // add the integer to the output array
           
             strcpy(&output[j],integer);
              ARRAY[j] = integer;
            j++;
        }
        i++;
        
    }


printf("--------------------------\n");
//print ARRAY
for(int i = 0; i < size; i++){
    printf("ARRAY: %s\n", ARRAY[i]);
}
printf("--------------------------\n");

    
    return ARRAY;
    
 

  

}


char *parse_until_CRLF(char *input){
    // read input until CRLF
    int i = 0;
    while (input[i] != '\r') {
        i++;
    }
    char *output = malloc(sizeof(char) * i);
    int j = 0;
    while (j < i) {
        output[j] = input[j];
        j++;
    }
    output[j] = '\0';
    return output;
}




// int main(int argc, char *argv[]) {

//     // // test parse_type()
//     // char *input = "*2\r\n$3\r\nhey\r\n$5\r\nthere\r\n";
//     // char *type = parse_type(input);
//     //  // test size_of_RESP()
//     // int size = size_of_RESP(input);
//     // printf("size: %d\n", size);
//     // printf("type: %s\n", type);
//     // printf("------------------------------------\n");
    
//     // // test parse_simple_string()
//     // // simple string: +ok\r\n
//      char *input2 = "+PING\r\n";
//      char *output = malloc(sizeof(char) * 100);
//      char *simple_string = parse_simple_string(input2, output);
//      printf("simple string: %s\n", simple_string);
//      printf("------------------------------------\n");

//     // // bulk string : $12\r\Amit shekhan\r\n
//     // char *input3 = "$12\r\nAmit shekhan\r\n";
//     //  // test size_of_RESP()
//     // int size2 = size_of_RESP(input3);
//     // printf("size: %d\n", size2);
//     // char *output2 = malloc(sizeof(char) * 100);
//     // char *bulk_string = parse_bulk_string(input3, output2);
//     // printf("bulk string: %s\n", bulk_string);
//     // printf("------------------------------------\n");
//     // // integer: :1000\r\n
//     // char *input4 = ":100\r\n";
//     // char *output3 = malloc(sizeof(char) * 100);
//     // char *integer = parse_integer(input4, output3);
//     // printf("integer: %s\n", integer);
//     // printf("------------------------------------\n");

//     // // error: -error message\r\n

//     // char *input5 = "-error message get \r\n";
//     // char *output4 = malloc(sizeof(char) * 100);
//     // char *error = parse_error(input5, output4);
//     // printf("error: %s\n", error);

    

//     // printf("------------------------------------\n");

//     // // array: *2\r\n$3\r\nhey\r\n$5\r\nthere\r\r(2 strings), *3\r\n:1\r\n:2\r\n:3\r\n(3 integers), *0\r\n(empty), *-1\r\n(null)
//     // char *input6 = "*2\r\n$3\r\nhey\r\n$5\r\nthere\r\n";
//     // // test size_of_RESP()
//     // int size3 = size_of_RESP(input6);
//     // printf("size: %d\n", size3);

//     // char *input7 = "*3\r\n$3\r\nSET\r\n$5\r\nmykey\r\n$7\r\nmyvalue\r\n";
//     // // test size_of_RESP()
//     // int size4 = size_of_RESP(input7);
//     // printf("size: %d\n", size4);

//     // // test parse_array() on input7
//     // char *output5 = malloc(sizeof(char) * 100);
//     // char *array = parse_array(input7, output5);

//     // test parse_until_CRLF()
//     // char *input8 = "*3\r\n$3\r\nSET\r\n$5\r\nmykey\r\n$7\r\nmyvalue\r\n";
//     // char *output6 = parse_array(input8);
//     // printf("--------------------------\n");
//     // printf("output: %s\n", output6);
//     // printf("--------------------------\n");

   
    
    


   
//     return 0;
// }

