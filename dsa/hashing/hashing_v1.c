/* ------------Hashing Version-1------------
 * Storing intergers and retrieving then efficiently
 * through hashing technique
 * Hashing technique: Multiplicative
 * Collision management: Open-Addressing (Quadratic Probing)
 * Flexible array based.
 */

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
int manage_collision(struct hash_table *, int, int, bool);
int generate_index(struct hash_table *, int, bool);
void insert(struct hash_table *, int);
void delete(struct hash_table *, int);
void update(struct hash_table *, int, int);
int get_key_index(struct hash_table *, int);

int main(void)
{
	/*int size = 1;
	int capacity = 0;
	int *arr = (int *) malloc(sizeof(int)*size);
	insert(arr, &size, &capacity, 3);
	insert(arr, &size, &capacity, 30);
	printf("%d\n", capacity);*/
	struct hash_table *table = init_table();
	table->insert(table, 21);
	table->insert(table, 39);
	table->insert(table, 77);
	table->insert(table, 94);
	table->insert(table, 15);
	table->insert(table, 62);
	table->insert(table, 8);
	table->insert(table, 46);
	table->insert(table, -1);
	table->delete(table, 21);
	table->delete(table, 39);
	table->delete(table, 77);
	table->delete(table, 94);
	table->delete(table, 15);
	table->delete(table, 62);
	table->delete(table, 8);
	table->delete(table, 46);
	table->delete(table, -1);
	table->insert(table, -1);
	table->update(table, -1, 0);
	printf("%d %d\n", table->size, table->capacity);
	for(int i = 0; i < table->size; i++)
		printf("%d ", table->arr[i]);
	puts("");
 	return 0;
}

struct hash_table *init_table(void)
{
	struct hash_table *ptr = (struct hash_table *) malloc(sizeof(struct hash_table));
	ptr->size = 8;
	ptr->capacity = 0;
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
	int diff = INT_MAX - table->size;
	int temp_size = diff >= table->size ? table->size*2 : diff;
	int previous_size = table->size;
	int *temp = (int *) malloc(sizeof(int) * temp_size);
	int *previous = table->arr;
	if(!temp)
	{
		return;
	}
	table->arr = temp;
	table->size = temp_size;
	for(int i = 0; i < previous_size; i++)
	{
		if(previous[i] == 0)
			continue;
		int index = generate_index(table, previous[i], false);
		//printf("Here: %d %d\n", index, previous[i]);
		table->arr[index] =  previous[i];
	}
	free(previous);
	return;
}

int manage_collision(struct hash_table *table, int key, int prev_index, bool bounce_zero)
{
	int c1 = 1, c2 = 2;
	
	int index;
	for(int i = 1; i <= INT_MAX; i++)
	{
		//index = collision_algorithm(table, key, prev_index, i);
		index = (prev_index + c1 * i + c2 * i^2) % table->size;

		if(index < 0)
		{
			index = -1;
			break;
		}
		if(table->arr[index] == 0)
		{
			if(!bounce_zero)
				break;
		}
		else if(table->arr[index] == key)
		{
			if(!bounce_zero)
				index = -1;
			break;
		}
		else
		{}
	}

	return index;
}

int generate_index(struct hash_table *table, int key, bool bounce_zero)
{
	float A = (sqrt(5) - 1) / 2;
	int index = floor(table->size * ((key * A) - floor(key * A)));

	//int index = index_generation_algorithm(table, key);
	
	if(table->arr[index] == 0)
	{
		if(bounce_zero)
			index = manage_collision(table, key, index, bounce_zero);
	}
	else if(table->arr[index] == key)
	{
		if(!bounce_zero)
			index = -1;
	}
	else
	{
		index = manage_collision(table, key, index, bounce_zero);
	}
	return index;
}

void insert(struct hash_table *table, int key)
{
	//printf("%d\n", key);
	if(key == 0)
	{
		puts("KeyError: Key cannot be zero");
		return;
	}
	if(table->capacity >= table->size)
		extend_arr(table);

	int index = generate_index(table, key, false);
	if(index != -1)
	{
		//printf("%d ", index);
		table->arr[index] = key;
		table->capacity++;
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
	if(table->capacity == 0)
		return;

	int index = generate_index(table, key, true);
	if(index < 0)
	{
		puts("No such key in table");
	}
	else
	{
		//printf("%d %d\n", index, key);
		table->arr[index] = 0;
		table->capacity--;
		puts("Value deleted");
	}
	return;
}

void update(struct hash_table *table, int key, int new)
{
	if(key == 0)
	{
		puts("KeyError: Key cannot be zero");
		return;
	}
	if(table->capacity <= 0)
		return;
	
	int index = generate_index(table, key, true);

	if(index < 0)
	{
		puts("No such element");
	}
	else
	{
		table->arr[index] = new;
		puts("Value updated");
	}
	return;
}

int get_key_index(struct hash_table *table, int key)
{
	return generate_index(table, key, true);
}
