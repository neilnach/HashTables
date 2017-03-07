#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hashtable.h"

#define PASS 1
#define FAIL 0
char* test_discriptions[] = {"Basic test to verify that the table is created correctly", 
    "Basic test to add a single key",
    "Basic set/get pair test",
    "Basic test for get() without key",
    "A few sets and gets, without any chaining",
    "Basic test to see if chaining works"};
    
int test1() {
    
    /**
     * Basic test to verify that the table is created correctly
     */
    int ret = PASS;
    hashtable_t* ht = create_hashtable(1024);
    if (ht == NULL) { 
    printf("Failed to create hashtable!\n");
        return FAIL;
    }
    if (ht->table_len != 1024) { 
    printf("The hashtable we just created has the wrong table_len!\n");
    ret= FAIL;   
    }
    int i;
    for (i = 0; i < 1024; i++)
        if (ht->table[i] != NULL) { 
    printf("The hashtable we just created has an entry != NULL even though we have not added anything!\n Remember to clear your newly allcoated memory, or use calloc()\n");
        ret= FAIL;
    }
    free_hashtable(ht);
    return ret;
}

/**
 * Basic test to add a single key
 */
int test2() {
    int ret = 1;
    hashtable_t* ht = create_hashtable(1024);
    if (ht == NULL) { 
    printf("Failed to create hashtable!\n");
        return FAIL;
    }
    if (set(ht, "Hi bob", 0) != 0) { 
    printf("set() Failed to set a simple key-value pair!\n");
        ret= FAIL;
    }
    free_hashtable(ht);
    return ret;
}

/**
 * Basic set/get pair test
 */
int test3() {
    int ret = PASS;
    double val;
    hashtable_t* ht = create_hashtable(1024);
    if (ht == NULL) { 
    printf("Failed to create hashtable!\n");
        return FAIL;
    }
    if (set(ht, "Hello", 4.5) != 0) { 
    printf("set() Failed to set a simple key-value pair!\n");
        ret= FAIL;
    }
    if (get(ht, "Hello", &val) != 0 || val != 4.5) { 
    printf("get() Failed to get a simple key-value pair we just set, or the value was off!\n");
        ret= FAIL;
    }
    free_hashtable(ht);
    return ret;
}

/**
 * Basic test for get() without key
 */
int test4() {
    int ret = PASS;
    double val;
    hashtable_t* ht = create_hashtable(1024);
    if (ht == NULL) { 
    printf("Failed to create hashtable!\n");
        return FAIL;
    }
    if (get(ht, "No such key", &val) != -1) ret = FAIL;
    free_hashtable(ht);
    return ret;
}
    /**
     * A few sets and gets, without any chaining
     */
int test5() {
    int ret = PASS;
    double val;
    hashtable_t* ht = create_hashtable(1024);
    if (ht == NULL){ 
    printf("Failed to create hashtable!\n");
        return FAIL;
    }
/* Use same buffer to catch shallow copies */
    char buf[1024];
    /* Add four keys */
    int i;
    for (i = 1; i <= 4; i++) {
        sprintf(buf, "MyKey:%d", i);
        if (set(ht, buf, 45.0 + i) != 0){
         printf("set() Failed to set key: %s\n", buf);   
        ret= FAIL;        
        }        
        if (key_exists(ht, buf) != 1) {
         printf("key_exists() Failed to find key we only just set: %s\n", buf);   
    ret= FAIL;        }   
    }

    /* Remove the middle key */
    strcpy(buf, "MyKey:2");
    if (remove_key(ht, buf) != 0){
         printf("remove_key() returned that it Failed to remove key: %s\n", buf);   
        ret= FAIL;        
        }   
    if (key_exists(ht, buf) != 0) {
         printf("key_exists() found the key we only just removed: %s\n", buf);   
        ret= FAIL;        
        }   

    /* Ensure we can access two of the others */
    strcpy(buf, "MyKey:1");
    if (get(ht, buf, &val) != 0 || val != 46.0) {
         printf("get() could not get: %s, or the value was wrong.\n", buf);   
        ret= FAIL;        
        }   

    strcpy(buf, "MyKey:4");
    if (get(ht, buf, &val) != 0 || val != 49.0) {
         printf("get() could not get: %s, or the value was wrong.\n", buf);   
        ret= FAIL;        
        }   

    free_hashtable(ht);
    return ret;
}
    /**
     * Basic test to see if chaining works
     */
int test6() {
    int ret = PASS;
    double val;
    hashtable_t* ht = create_hashtable(1); //This hashtable is WAYYYY TOO SMALL! This will cause chaining
    if (ht == NULL){ 
    printf("Failed to create hashtable!\n");
        return FAIL;
    }
    /* Use same buffer to catch shallow copies */
    char buf[1024];
    /* Add four keys */
    int i;
    for (i = 1; i <= 4; i++) {
        sprintf(buf, "MyKey:%d", i);
        if (set(ht, buf, 45.0 + i) != 0){
         printf("set() Failed to set key: %s\n", buf);   
        ret= FAIL;        
        }        
        if (key_exists(ht, buf) != 1) {
         printf("key_exists() Failed to find key we only just set: %s\n", buf);   
        ret= FAIL;        
        }   
    }
     /* Remove the middle key */
    strcpy(buf, "MyKey:2");
    if (remove_key(ht, buf) != 0){
         printf("remove_key() returned that it Failed to remove key: %s\n", buf);   
        ret= FAIL;        
        }   
    if (key_exists(ht, buf) != 0) {
         printf("key_exists() found the key we only just removed: %s\n", buf);   
        ret= FAIL;        
        }   

    /* Ensure we can access two of the others */
    strcpy(buf, "MyKey:1");
    if (get(ht, buf, &val) != 0 || val != 46.0) {
         printf("get() could not get: %s, or the value was wrong.\n", buf);   
        ret= FAIL;        
        }   

    strcpy(buf, "MyKey:4");
    if (get(ht, buf, &val) != 0 || val != 49.0) {
         printf("get() could not get: %s, or the value was wrong.\n", buf);   
        ret= FAIL;        
        }   
    

    free_hashtable(ht);
    return ret;
}

int dispatch_test(int (*test)(), int idx) {
    int ret = test();
    if(idx< 7){
        printf("\n********TEST %d: %s********\n", idx, test_discriptions[idx-1]);
    }
    if (ret == PASS) fprintf(stderr, "PASS: test%d\n", idx);
    else fprintf(stderr, "FAIL: test%d\n", idx);
    return ret;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <caseid>\n", argv[0]);
        return 1;
    }

    int (*tests[])() = {
        test1, test2, test3,
        test4, test5, test6
    };
    const int testcount = sizeof(tests)/sizeof(int(*)());
    int testcase = atoi(argv[1]);
    if (testcase == 0) {
        int idx, ret = 1;
        for (idx = 1; idx <= testcount; idx++)
            ret = dispatch_test(tests[idx - 1], idx) && ret;
        return !ret;
    } else {
        return !dispatch_test(tests[testcase - 1], testcase);
    }
}
