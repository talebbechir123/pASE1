#include "../include/replication.h"



void hashtable_replicate(hashtable *ht, char *filename){
    //create a file or open a file
    FILE *fp = fopen(filename, "w+");
    if(fp == NULL){
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    int i = 0;
    hashtable_element *pair;
    for(i = 0; i < ht->size; i++){
        pair = ht->table[i];
        while(pair != NULL && pair->key != NULL){
            fprintf(fp, "%s => %s\n", pair->key, (char *)pair->value);
            pair = pair->next;
        }
    }
    fclose(fp);
}


// convert a text file to a hashtable

hashtable *hashtable_convert(char *filename){
    FILE *fp = fopen(filename, "r");
    if(fp == NULL){
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    hashtable *ht = hashtable_new(65536);
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *key;
    char *value;
    while((read = getline(&line, &len, fp)) != -1){
        key = strtok(line, "=>");
        value = strtok(NULL, "=>");
        hashtable_set(ht, key, value);
    }
    fclose(fp);
    if(line){
        free(line);
    }
    return ht;
}

// write a hashtable as a json file

void hashtable_write_json(hashtable *ht, char *filename){
    FILE *fp = fopen(filename, "w");
    if(fp == NULL){
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    int i = 0;
    hashtable_element *pair;
    fprintf(fp, "{\n");
    for(i = 0; i < ht->size; i++){
        pair = ht->table[i];
        while(pair != NULL && pair->key != NULL){
            fprintf(fp, "\t\"%s\": \"%s\"", pair->key, (char *)pair->value);
            if(pair->next != NULL){
                fprintf(fp, ",");
            }
             fprintf(fp, ",");
            fprintf(fp, "\n");
            //add a comma if there is another pair


             
            pair = pair->next;
        }
        //for the last pair, remove the comma
         
       
           

    }
    fprintf(fp, "}\n");
    fclose(fp);
}

