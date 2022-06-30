/*
 * Ezana Kebede
 * 05/10/21
 * ENEE150
 * High-Performance Hash Table ADT
*/

#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

int hash_function(int num_buckets, char *key); // Private function that finds index to hash table

Phash_table new_hash(int size, void (*print_func)(void *)) {
    // Creates hash table structure
    Phash_table hash_table_ptr;
    hash_table_ptr = (Phash_table)malloc(sizeof(hash_table));
    hash_table_ptr -> hash = (hash_entry **)malloc(sizeof(hash_entry *) * size);
    int i;
    for (i = 0; i < size; i++) {
        hash_table_ptr -> hash[i] = NULL; // Empties buckets in hash table
    }
    hash_table_ptr -> num_buckets = size;
    hash_table_ptr -> num_elements = 0;
    hash_table_ptr -> num_searches = 0;
    hash_table_ptr -> num_data_records_searched = 0;
    hash_table_ptr -> worst_search = 0;
    hash_table_ptr -> printing_func = print_func;
    return hash_table_ptr;
}

void free_hash(Phash_table table) {
    // Deallocates entire hash table
    int i;
    for (i = 0; i < (table -> num_buckets); i++) {
        hash_entry *cur;
        hash_entry *prev;
        cur = table -> hash[i];
        while (cur) {
            free(cur -> data);
            free(cur -> key);
            prev = cur;
            cur = cur -> next;
            free(prev);
        }
    }
    free(table -> hash);
    free(table);
}

int hash_function(int num_buckets, char *key) {
    // Finds index of buckets based on given key
    unsigned sum = 0;
    int i;
    for (i = 0; i < strlen(key); i++) {
        sum = sum + ((unsigned)key[i] * (unsigned)pow(2,i));
    }
    int index = sum % num_buckets;
    return index;
}

void insert_hash(Phash_table table, char *key, void *data) {
    // Inserts data record into hash table
    int index = hash_function(table -> num_buckets, key);
    hash_entry *cur;
    cur = (hash_entry *)malloc(sizeof(hash_entry));
    cur -> key = (char *)malloc(sizeof(char) * strlen(key) + 1);
    strcpy(cur -> key, key);
    cur -> data = data;
    cur -> next = table -> hash[index];
    table -> hash[index] = cur;
    table -> num_elements++;
}

void *find_hash(Phash_table table, char *key) {
    // Searched for data record in hash table
    table -> num_searches++;
    int index = hash_function(table -> num_buckets, key);
    hash_entry *cur;
    int num_traverses = 0;
    for (cur = table -> hash[index]; cur; cur = cur -> next) {
        table -> num_data_records_searched++;
        num_traverses++;
        if (strcmp(cur -> key, key) == 0) {
            break;
        }
    }
    if (!cur) {
        return NULL;
    }
    else {
        if (num_traverses > (table -> worst_search)) {
            table -> worst_search = num_traverses;
        }
        return cur -> data;
    }
}

void stat_hash(Phash_table table, int *total, float *avg_search, int *worst_search) {
    // Returns 3 statistics associated with hash table
    *total = table -> num_elements;
    *avg_search = (float)(table -> num_data_records_searched) / (float)(table -> num_searches);
    *worst_search = table -> worst_search;
}

void dump_hash(Phash_table table) {
    // Dumps all content in hash table
    int i;
    for (i = 0; i < (table -> num_buckets); i++) {
        hash_entry *cur;
        for (cur = table -> hash[i]; cur; cur = cur -> next) {
            (*(table -> printing_func))(cur -> data);
        }
    }
}

void resize_hash(Phash_table table, int size) {
    // Changed number of buckets in hash table, reinserts data
    hash_entry **new_hash;
    new_hash = (hash_entry **)malloc(sizeof(hash_entry *) * size);
    int i;
    for (i = 0; i < size; i++) {
        new_hash[i] = NULL; // Empties buckets in new hash table
    }
    for (i = 0; i < (table -> num_buckets); i++) {
        hash_entry *table_cur;
        hash_entry *new_table_cur;
        table_cur = table->hash[i];
        for (; table_cur; table_cur = table_cur -> next) {
            new_table_cur = (hash_entry *) malloc(sizeof(hash_entry));
            new_table_cur->key = (char *) malloc(sizeof(char) * strlen(table_cur -> key) + 1);
            strcpy(new_table_cur->key, table_cur->key);
            new_table_cur->data = table_cur->data;
            int index = hash_function(size, table_cur->key);
            new_table_cur->next = new_hash[index];
            new_hash[index] = new_table_cur;
        }
    }
    table -> num_buckets = size;
    table -> num_searches = 0;
    table -> num_data_records_searched = 0;
    table -> worst_search = 0;
    table -> hash = new_hash;
}
