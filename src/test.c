#include "../include/replication.h"



int main(){

 

    // create a new hashtable

    hashtable *ht = hashtable_new(65536);
   // add some key/value pairs

   hashtable_set(ht, "key1", "inky");
   hashtable_set(ht, "key2", "pinky");
   hashtable_set(ht, "key3", "blinky");
   hashtable_set(ht, "key4", "floyd");
   hashtable_set(ht, "james", "floyd");
   hashtable_set(ht, "jane", "floyd");


    //print the contents of the hashtable
    hashtable_print(ht);

    // retrieve some key/value pairs
    char* k = hashtable_get(ht, "key1");

    printf("%s\n", k);

    // delete some key/value pairs
    hashtable_delete(ht, "key1");

    // print the contents of the hashtable
    hashtable_print(ht);


    // test writing to a file
    hashtable_replicate(ht, "test.txt");

    // test exporting to json
    hashtable_write_json(ht, "test.json");








  


    return 0;
}