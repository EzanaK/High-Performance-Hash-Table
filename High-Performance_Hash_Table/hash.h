/*
 * hash.h
 *
 Interfaces for Hash Table ADT
 *
 */

typedef struct hash_entry_{
    char *key;
    void *data;
    struct hash_entry_ *next;
} hash_entry;

typedef struct hash_table_ {
    hash_entry **hash;
    int num_buckets;
    int num_elements;
    int num_searches;
    int num_data_records_searched;
    int worst_search;
    void (*printing_func)(void *);
} hash_table, *Phash_table;

Phash_table new_hash(int size, void (*print_func)(void *));
void free_hash(Phash_table table);
void insert_hash(Phash_table table, char *key, void *data);
void *find_hash(Phash_table table, char *key);
void stat_hash(Phash_table table, int *total, float *avg_search, int *worst_search);
void dump_hash(Phash_table table);
void resize_hash(Phash_table table, int size);
