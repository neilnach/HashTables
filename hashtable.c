#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

static hashtable_ent_t* add_new(hashtable_ent_t** entry, const char* key) {
	if (entry == NULL || key == NULL) { 
    		return NULL;
	}
    	*entry = (hashtable_ent_t*)malloc(sizeof(hashtable_ent_t));
    	
	if (*entry == NULL) 
		return NULL;
    	
	(*entry)->key = strdup(key);
    	
	if ((*entry)->key == NULL) {
        	free(*entry);
        	return NULL;
    	}
    	
	(*entry)->next = NULL;
    	return *entry;
}

/*
 * Helper function that does most of the implemention work
 */
static hashtable_ent_t* locate(hashtable_t* ht, const char* key, int create_if_missing) {
	if (ht == NULL || key == NULL) 
		return NULL;
    	int h = hash(key) % ht->table_len;
    	hashtable_ent_t* node = ht->table[h];
    	if (node != NULL) {
        /* Search until we find a match or hit the end */
        	while (node->next != NULL && strcmp(key, node->key)) {
			node = node->next;
		}	
        	if (!strcmp(key, node->key)) {
            		return node;
        	} 
		else if (node->next == NULL && create_if_missing) {
            	/* If we failed to find the key, we can create an entry in place */
            		return add_new(&node->next, key);
        	}
    	} 
    	else if (create_if_missing) {
        	return add_new(&ht->table[h], key);
    	}
    return NULL;
}

hashtable_t *create_hashtable(int max_size) {
	hashtable_t * table1 =  malloc (sizeof(hashtable_t));
	table1 -> table_len = max_size;
	table1 -> table = malloc(max_size * sizeof(hashtable_ent_t*));
	return table1;
}

void free_hashtable(hashtable_t *ht) {
	int j;
	for (j = 0; j < ht -> table_len; j++) {
		if (ht -> table[j]) {
			hashtable_ent_t * ent = ht -> table[j];
			hashtable_ent_t * prev;
			while (ent -> next) {
				prev = ent;
				ent = ent -> next;
				free(prev);
			}
		}
	}
	free(ht);
}

int get(hashtable_t *ht, const char *key, double *value) {
    	if (ht == NULL || key == NULL || value == NULL) {
    		return -1;
    	}	
    	hashtable_ent_t * ent = locate(ht, key, 0);
    	if (ent == NULL) {
    		return -1;
	}		
    *value = ent -> value;
    return 0;
}

int set(hashtable_t *ht, const char *key, double value) {
	if (ht == NULL || key == NULL) {
		return -1;
	}
	hashtable_ent_t * entry = locate(ht, key, 1);
	entry -> value = value;
	return 0;
}


int key_exists(hashtable_t *ht, const char *key) {
	if (ht == NULL || key == NULL) {
		return -1;
	}
	if (locate(ht, key, 0) != NULL) {
		return 1;
	}
	return 0;
}

int remove_key(hashtable_t *ht, const char *key) {
	if (ht == NULL || key == NULL) {
		return -1;
	}
	hashtable_ent_t * entry = locate(ht, key, 0);
	if (entry == NULL) {
		return -1;
	}
	int i = hash(key) % ht -> table_len;
	hashtable_ent_t * bucket = ht -> table[i];
	if (strcmp(entry -> key, bucket -> key) == 0) {
		ht -> table[i] = bucket -> next;
		free(entry);
		return 0;
	}		
	else {
		hashtable_ent_t * prev;	
		while(strcmp(entry -> key, bucket -> key) != 0) {
			prev = bucket;
			bucket = bucket -> next;
		}
		prev -> next = bucket -> next;
		free(entry);
		return 0;
	}
}	
		
