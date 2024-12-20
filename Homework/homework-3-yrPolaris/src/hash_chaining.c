#include "../inc/hash_chaining.h"
#include "../inc/hash_func.h"

hash_chaining *hash_chaining_init(uint32_t size)
{
  // TODO
  hash_chaining *hash_table = (hash_chaining *)malloc(sizeof(struct hash_chaining));
  if (hash_table == NULL)
  {
    return NULL;
  }
  hash_table->size = size;
  hash_table->slots = (list_node **)malloc(sizeof(struct list_node *) * size);
  if (hash_table->slots == NULL)
  {
    return NULL;
  }
  hash_table->parameters = generate_hash_parameters();

  return hash_table;
}

void hash_chaining_insert(hash_chaining *table, uint32_t key)
{
  // TODO
  uint32_t index = hash_func(key, table->parameters, table->size);
  list_node *Head_node = table->slots[index];
  list_node *current_node = table->slots[index];
  while (current_node != NULL)
  {
    if (current_node->key != key)
    {
      current_node = current_node->next;
    }
    else
    {
      break;
    }
  }
  if (current_node == NULL)
  {
    list_node *new_node = (list_node *)malloc(sizeof(list_node));
    if (new_node == NULL)
    {
      return;
    }
    new_node->key = key;
    new_node->next = Head_node->next;
    Head_node->next = new_node;
  }
}

bool hash_chaining_search(hash_chaining *table, uint32_t key)
{
  // TODO
  uint32_t index = hash_func(key, table->parameters, table->size);
  list_node *current_node = table->slots[index];
  while (current_node != NULL)
  {
    if (current_node->key != key)
    {
      current_node = current_node->next;
    }
    else
    {
      break;
    }
  }
  if (current_node == NULL)
  {
    return false;
  }
  return true;
}

void hash_chaining_destroy(hash_chaining *table)
{
  // TODO
  for (uint32_t i = 0; i < table->size; i++)
  {
    list_node *current_node = table->slots[i];
    list_node *next_node = current_node;
    while (current_node != NULL)
    {
      next_node = current_node->next;
      free(current_node);
      current_node = next_node;
    }
  }
  free(table->slots);
  free(table);
}