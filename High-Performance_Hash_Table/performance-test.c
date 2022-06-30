
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

#define WORD_SIZE 40
#define HASH_SIZE 1000
#define NUM_WORDS 109582

char fname[] = "words.txt";
char word_data[NUM_WORDS][WORD_SIZE];

void create_hash_table(Phash_table table) {
  FILE *inFile;
  char cur_word[WORD_SIZE];
  int i = 0, num, worst_search;
  float average, avg_search;
  char *word;

  inFile = fopen(fname, "r");
  while (fscanf(inFile, "%s", cur_word) != EOF) {
    word = (char *)malloc(sizeof(char)*(strlen(cur_word)+1));
    strcpy(word_data[i], cur_word);
    insert_hash(table, cur_word, word);
    i++;
  }
}


void search(Phash_table table) {
  int i;
  void *word;
  int num, worst_search;
  float avg_search;

  for (i = 0; i < NUM_WORDS; i++) {
    word = find_hash(table, word_data[i]);
  }

  stat_hash(table, &num, &avg_search, &worst_search);
  printf("\nNumber of words:  %d\n", num);
  printf("Average search:  %f\n", avg_search);
  printf("Worst-case search:  %d\n", worst_search);
}


int main(int argc, char *argv[]) {
  Phash_table table;
  int num;
  int worst_search;
  float avg_search;
  int i;

  table = new_hash(HASH_SIZE, NULL);
  create_hash_table(table);
  search(table);

  for (i = 1; i < 40; i++) {
    resize_hash(table, HASH_SIZE + i*250);
    search(table);
  }

  free_hash(table);
}
