#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*                                                                                                                                                                                            
 * Diagram for understanding what is happening.
 * every memory allocation performed will add 8 bytes that will contain
 * the size of the pointer. this is needed for my_free() 
 * 0                                                              63
 * ================================================================
 * SSSSSSSSaaaa
 *
 * void *base_ptr = 0
 * uint64_t size_ptr = base_ptr
 * void *user_ptr = base_ptr + 8
 * initially, the whole my_free_list is just one item, since we have 
 * made 0 allocations.
 */

static uint8_t MEMORY_POOL[64];
static uint64_t FREE_LIST_USED = 1;

struct free_entry {
  void *ptr;
  uint64_t size; ;
};

typedef struct free_entry free_entry_t; // free_entry_t now synonymous with struct free_entry;

// initially, there is 1 free block that equals the whole memory allocation. 
// FREE_LIST can grow up to 64 free blocks (non-coalesced version of the initial setup)
static free_entry_t FREE_LIST[64] = {
  (free_entry_t) {
    .ptr = MEMORY_POOL,
    .size = 64,
  },
};

void print_free_list() {
  printf("FREE LIST \n");
  for (uint64_t i = 0; i < FREE_LIST_USED; i++) {
    free_entry_t *entry;
    entry = &FREE_LIST[i];
    printf("%p %llu \n", entry->ptr, entry->size);
  }
}

free_entry_t *find_free_entry(size_t size) {
  //free_entry_t *best_entry = FREE_LIST;
  for (uint64_t i = 0; i < FREE_LIST_USED; i++) {
    free_entry_t *entry;
    entry = &FREE_LIST[i];
    if (entry->size >= size) {
      return entry;
      //best_entry = entry;
    }
  }
  abort();
  //return best_entry;
}

void *my_malloc(size_t size) {
  size +=8;
  free_entry_t *entry;
  entry = find_free_entry(size);
  print_free_list();

  void *base_ptr; 
  void *user_ptr;
  uint64_t *size_ptr;

  base_ptr = entry->ptr;
  size_ptr = base_ptr;
  user_ptr = base_ptr + 8;

  *size_ptr = size;

  entry->ptr += size;
  entry->size -= size;
  return user_ptr;
}

void *my_free(void *user_ptr) {
  // append to the end of the FREE_LIST
  free_entry_t *entry;
  entry = &FREE_LIST[FREE_LIST_USED];

  void *base_ptr; 
  uint64_t *size_ptr;
  uint64_t size;

  base_ptr = user_ptr - 8;
  size_ptr = base_ptr;

  printf("user_ptr: %p\n", user_ptr);
  size = *size_ptr;
  printf("size: %d\n", size);
  entry->ptr = base_ptr;
  entry->size = size;

  FREE_LIST_USED++;
  printf("IN FREE \n");
  print_free_list();
}

int main() {
  char* a;
  char* b;
  char* c;
  char* d;

  a = my_malloc(4);
  b = my_malloc(4);
  c = my_malloc(4);

  strcpy(a, "foo\n");
  strcpy(b, "bar\n");
  strcpy(c, "baz\n");

  printf("a = %p\n", a);
  printf("b = %p\n", b);
  printf("c = %p\n", c);

  my_free(b);
  d = my_malloc(4);
  printf("d = %p\n", d);
}
