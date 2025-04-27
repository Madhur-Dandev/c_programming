/* ------------Hashing Version-1------------
 * Storing intergers and retrieving then efficiently
 * through hashing technique
 * Hashing technique: Multiplicative
 * Collision management: Open-Addressing (Quadratic Probing)
 * Flexible array based.
 */

// to-do
// adjust table size based on load factor.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <stdbool.h>

struct hash_table
{	
	int *arr;
	int size;
	int capacity;
	void (*insert)(struct hash_table *, int);
	void (*delete)(struct hash_table *, int);
	void (*update)(struct hash_table *, int, int);
	int (*get_key_index)(struct hash_table *, int);
};

struct hash_table *init_table(void);
void delete_table(struct hash_table *);
void extend_arr(struct hash_table *);
int manage_collision(int *, int, int, int, bool);
int generate_index(int *, int, int, bool);
void insert(struct hash_table *, int);
void delete(struct hash_table *, int);
void update(struct hash_table *, int, int);
int get_key_index(struct hash_table *, int);

void print_arr(int *table, int size)
{
	for(int i = 0; i < size; i++)
		printf("%d ", table[i]);
	puts("");
	return;
}

int main(void)
{
	struct hash_table *table = init_table();
	table->insert(table, 21);
	print_arr(table->arr, table->capacity);
	table->insert(table, 39);
	print_arr(table->arr, table->capacity);
	table->insert(table, 77);
	print_arr(table->arr, table->capacity);
	table->insert(table, 94);
	print_arr(table->arr, table->capacity);
	table->insert(table, 15);
	print_arr(table->arr, table->capacity);
	table->insert(table, 62);
	print_arr(table->arr, table->capacity);
	table->insert(table, 8);
	print_arr(table->arr, table->capacity);
	table->insert(table, 46);
	print_arr(table->arr, table->capacity);
	table->insert(table, -1);
	print_arr(table->arr, table->capacity);
	table->insert(table, -68);
	print_arr(table->arr, table->capacity);
	table->insert(table, 99);
	print_arr(table->arr, table->capacity);
	table->insert(table, 234);
	print_arr(table->arr, table->capacity);
	/*table->delete(table, 21);
	table->delete(table, 39);
	table->delete(table, 77);
	table->delete(table, 94);
	table->delete(table, 15);
	table->delete(table, 62);
	table->delete(table, 8);
	table->delete(table, 46);
	table->delete(table, -1);
	table->insert(table, -1);*/
	table->update(table, -1, 10);
	print_arr(table->arr, table->capacity);
	printf("%d %d\n", table->size, table->capacity);
	printf("index: %d\n", get_key_index(table, 1000));
	delete_table(table);
 	return 0;
}

struct hash_table *init_table(void)
{
	struct hash_table *ptr = (struct hash_table *) malloc(sizeof(struct hash_table));
	ptr->size = 0;
	ptr->capacity = 8;
	ptr->arr = (int *) malloc(sizeof(int));
	ptr->insert = &insert;
	ptr->delete = &delete;
	ptr->update = &update;
	ptr->get_key_index = &get_key_index;
	return ptr;
}

void delete_table(struct hash_table *table)
{
	free(table->arr);
	free(table);
	puts("Table delete");
	return;
}

void extend_arr(struct hash_table *table)
{
	int diff = INT_MAX - table->capacity;
	int temp_capacity = diff >= table->capacity ? table->capacity*2 : (table->capacity + diff);
	int *temp = (int *) calloc(temp_capacity, sizeof(int));
	if(!temp)
	{
		return;
	}
	for(int i = 0; i < table->capacity; i++)
	{
		if(table->arr[i] == 0)
			continue;
		int index = generate_index(temp, temp_capacity, table->arr[i], false);
		//printf("Here: %d %d\n", index, table->arr[i]);
		temp[index] =  table->arr[i];
	}
	int *previous = table->arr;
	table->arr = temp;
	table->capacity = temp_capacity;
	free(previous);
	return;
}

int manage_collision(int *table, int table_size, int key, int prev_index, bool bounce_zero)
{
	int c1 = 1, c2 = 2;
	
	int index = -1;
	for(int i = 1; i <= table_size; i++)
	{
		//index = collision_algorithm(table, key, prev_index, i);
		index = (int) (prev_index + c1 * i + c2 * pow((double) i, 2.0f)) % table_size;

		if(index < 0)
		{
			//printf("%d %d\n", key, index);
			index = -1;
			break;
		}
		if(table[index] == 0)
		{
			if(!bounce_zero)
				break;
		}
		else if(table[index] == key)
		{
			if(!bounce_zero)
				index = -1;
			break;
		}
		else
		{}
	}

	if(table[index] != key)
		return -1;

	return index;
}

int generate_index(int *table, int table_size, int key, bool bounce_zero)
{
	float A = (sqrt(5) - 1) / 2;
	int index = floor(table_size * ((key * A) - floor(key * A)));
	//int index = index_generation_algorithm(table, key);
	
	if(table[index] == 0)
	{
		if(bounce_zero)
			index = manage_collision(table, table_size, key, index, bounce_zero);
	}
	else if(table[index] == key)
	{
		if(!bounce_zero)
			index = -1;
	}
	else
	{
		index = manage_collision(table, table_size, key, index, bounce_zero);
	}
	return index;
}

void insert(struct hash_table *table, int key)
{
	printf("%d ", key);
	if(key == 0)
	{
		puts("KeyError: Key cannot be zero");
		return;
	}
	if(table->size >= table->capacity)
		extend_arr(table);

	int index = generate_index(table->arr, table->capacity, key, false);
	if(index != -1)
	{
		//printf("%d ", index);
		table->arr[index] = key;
		table->size++;
		puts("Value inserted");
	}
	else
	{
		puts("Unable to insert value");
	}
	return;
}

void delete(struct hash_table *table, int key)
{
	if(key == 0)
	{
		puts("KeyError: Key cannot be zero");
		return;
	}
	printf("%d\n", key);
	if(table->size == 0)
		return;

	int index = generate_index(table->arr, table->capacity, key, true);
	if(index < 0)
	{
		puts("No such key in table");
	}
	else
	{
		//printf("%d %d\n", index, key);
		table->arr[index] = 0;
		table->size--;
		puts("Value deleted");
	}
	return;
}

void update(struct hash_table *table, int key, int new)
{
	// new value needs to get new index genereated
	// old value will get deleted from its position

	if(key == 0 || new == 0)
	{
		puts("KeyError: Key cannot be zero");
		return;
	}
	if(table->size <= 0)
		return;
	
	int index = generate_index(table->arr, table->capacity, key, true);

	if(index < 0)
	{
		puts("No such element");
	}
	else
	{
		int new_index = generate_index(table->arr, table->capacity, new, false);
		if(new_index < 0)
		{
			puts("Value cannot be updated");
			return;
		}
		table->arr[index] = 0;
		table->arr[new_index] = new;
		puts("Value updated");
	}
	return;
}

int get_key_index(struct hash_table *table, int key)
{
	return generate_index(table->arr, table->capacity, key, true);
}
