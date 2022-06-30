/*
 * parse.c
 *
 * Uses Hash Table ADT
 *
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "hash.h"
#include <stdlib.h>

#define WORD_SIZE 40
#define NUM_BUCKETS 1000

typedef struct data_ { // Data stored
    char word[WORD_SIZE];
    int frequency;
} data_t;

void lower_case_word(char *);
void dump_dictionary(Phash_table table);
void print(void *data);

int main(void) {
  Phash_table table = new_hash(NUM_BUCKETS, print);
  char c, word[WORD_SIZE];
  int char_index = 0, num_words = 0;

  printf("Parsing input ...\n");
  /* Read in characters until end is reached */
  while ((c = getchar()) != EOF) {
    if ((c == ' ') || (c == ',') || (c == '.') || (c == '!') || (c == '"') ||
        (c == ':') || (c == '\n')) {
      /* End of a word */
      if (char_index) {
        /* Word is not empty */
        word[char_index] = '\0';
        lower_case_word(word);
        data_t *cur;
        cur = find_hash(table, word);
        if (!cur) {
            // Data is not in hash table
            data_t *new;
            new = (data_t *)malloc(sizeof(data_t));
            strcpy(new -> word, word);
            new -> frequency = 1;
            insert_hash(table, word, new);
        }
        else {
            // Data is already in hash table
            cur -> frequency++;
        }
        char_index = 0;
        num_words++;
      }
    } else {
      /* Continue assembling word */
      word[char_index++] = c;
    }
  }
  dump_dictionary(table);
}


void lower_case_word(char *w) {
  int i = 0;

  while (w[i] != '\0') {
    w[i] = tolower(w[i]);
    i++;
  }
}

void dump_dictionary(Phash_table table) {
    int total;
    float avg_search;
    int worst_search;
    stat_hash(table, &total, &avg_search, &worst_search);
    printf("Number of unique words:  %d\n", total);
    printf("Average search:  %.2f\n", avg_search);
    printf("Worst-case search:  %d\n", worst_search);
    dump_hash(table);
}

void print(void *data) { // Prints stored data
    data_t *cur;
    cur = (data_t *)data;
    if (cur -> frequency > 1) {
        printf("%s:  %d\n", cur->word, cur->frequency);
    }
}
