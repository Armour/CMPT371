/*------------------------------------------------------
 *
 *  Chord.cpp
 *
 *  Project         : Chord_DHT
 *  Name            : Chong Guo
 *  Student ID      : 301295753
 *  Language        : C++
 *  SFU username    : armourg
 *
 *  Created by Armour on 25/10/2015
 *  Copyright (c) 2015 Armour. All rights reserved.
 *
 *------------------------------------------------------
 */

#include <iostream>
#include <cstring>
#include <cmath>

#define HASH_SPACE (s + 1)
#define MAX_FINGER_NUM floor(log2(HASH_SPACE))

using namespace std;

typedef struct Finger {
    int number;
    int expect_succ;                /* Expected successor number */
    int real_succ;                  /* Real successor number */
} Finger;

typedef struct Key {
    int value;                      /* Key value */
    struct Key *next;               /* Stored as lists */
} Key;

typedef struct SuccTable {
    int count;
    Finger **f;                     /* This is the fingers of this node */
    Key *k;                         /* This is the keys that stored in this node */
} SuccTable;

int s, n, m;
int *exist_key;                     /* Used to store existed key */
SuccTable **st;                     /* An list of SuccTable */
FILE *fin, *fout;
char *file_in, *file_out;            /* The file name of input and output file */

/*
 * Function: Create_succ_table
 * -------------------
 *   This function is used to create a successor table of one ndoe
 *
 *   Parameters:
 *      id: the index of one node
 *
 *   Returns:
 *      void
 */

void create_succ_table(int id) {
    st[id] = (SuccTable *)malloc(sizeof(SuccTable));
    st[id]->f = (Finger **)malloc(sizeof(Finger *) * MAX_FINGER_NUM);
    for (int i = 0; i < MAX_FINGER_NUM; i++) {
        st[id]->f[i] = (Finger *)malloc(sizeof(Finger));
        st[id]->f[i]->number = i;
        st[id]->f[i]->expect_succ = (id + (int)pow(2, (double)i)) % HASH_SPACE;     /* Get expect successor number */
        for (int k = st[id]->f[i]->expect_succ; ; k++) {
            if (k == HASH_SPACE) k = 0;
            if (st[k] != NULL) {
                st[id]->f[i]->real_succ = k;            /* Get real successor number */
                break;
            }
        }
    }
    st[id]->k = (Key *)malloc(sizeof(Key));
    st[id]->k->value = -1;
    st[id]->k->next = NULL;
}

/*
 * Function: Update_succ_table
 * -------------------
 *   This function is used to update successor table
 *
 *   Parameters:
 *      id: the index of the node that need to update
 *
 *   Returns:
 *      void
 */

void update_succ_table(int id) {
    for (int i = 0; i < MAX_FINGER_NUM; i++) {
        for (int k = st[id]->f[i]->expect_succ; ; k++) {
            if (k == HASH_SPACE) k = 0;
            if (st[k] != NULL) {
                st[id]->f[i]->real_succ = k;            /* Update real successor number */
                break;
            }
        }
    }
}

/*
 * Function: Print_succ_table
 * -------------------
 *   This function is used to print the state of one successor table,
 *   mainly used for debugging
 *
 *   Parameters:
 *      no parameters
 *
 *   Returns:
 *      void
 */

void print_succ_table(void) {
    for (int id = 0; id < HASH_SPACE; id++) {
        if (st[id]) {
            cout << "Finger: " << endl;
            for (int i = 0; i < MAX_FINGER_NUM; i++) {          /* Print all the fingers */
                cout << id << " " << i << " " << st[id]->f[i]->real_succ << endl;
            }
            cout << "Key: " << endl;
            Key *p = st[id]->k;
            cout << p->value << " ";
            while (p->next != NULL) {
                p = p->next;
                cout << p->value << " ";                        /* Print all the keys */
            }
            cout << endl;
        }
    }
}

/*
 * Function: Store_key
 * -------------------
 *   This function is used to store one key to one node,
 *   it will automatically choose where to store this key
 *
 *   Parameters:
 *      key: the key that we want to store
 *
 *   Returns:
 *      void
 */

void store_key(int key) {
    int i = key;
    while (st[i] == NULL) {                 /* Find a exist node */
        i++;
        if (i == HASH_SPACE)
            i = 0;
    }
    Key *p = st[i]->k;
    while (p->next != NULL)                 /* Until find the end */
        p = p->next;
    p->next = (Key *)malloc(sizeof(Key));
    p->next->value = key;                   /* Add key */
    p->next->next = NULL;
}

/*
 * Function: Chord_query
 * -------------------
 *   This function is used to query one key's position
 *
 *   Parameters:
 *      id: the node who give a query
 *      key: the asked key
 *      path: counter for how many hops has already been made
 *
 *   Returns:
 *      void
 */

void chord_query(int id, int key, int path) {
    int i;
    Key *p = st[id]->k;
    while (p->next != NULL) {
        p = p->next;
        if (p->value == key) {              /* If found that key in this node */
            fprintf(fout, "%d\n", path);
            return;
        }
    }
    for (i = MAX_FINGER_NUM - 1; i > 0; i--) {
        int succ = st[id]->f[i]->real_succ;             /* Get the real successor node */
        if (succ < id) succ += HASH_SPACE;
        if (succ <= (id < key? key: key + HASH_SPACE))
            break;
    }
    chord_query(st[id]->f[i]->real_succ, key, path + 1);        /* If not found, go next ndoe */
}

/*
 * Function: Get_output_file
 * -------------------
 *   This function is used to get the name of the output file
 *
 *   Parameters:
 *      input: the name of the input file
 *
 *   Returns:
 *      the name of the output file
 */

char *get_output_file(char *input) {
    char *output;
    output = (char *)malloc(sizeof(char) * 1024);
    strcpy(output, input);
    char *end = output + strlen(output) - 1;
    while (end > output) {
        if (*end == '.') {                  /* If found extension name */
            *(end + 1) = 'o';
            *(end + 2) = 'u';
            *(end + 3) = 't';
            *(end + 4) = '\0';
            return output;                  /* Change extension name to .out and return it */
        }
        end--;
    }
    end = output + strlen(output) - 1;      /* If not found extension name */
    *(end + 1) = '.';
    *(end + 2) = 'o';
    *(end + 3) = 'u';
    *(end + 4) = 't';
    *(end + 5) = '\0';                      /* Just add .out extension at the end of file name */
    return output;
}

/*
 * Function: Main
 * -------------------
 *   The main function
 *
 *   Parameters:
 *      argc: parameter count
 *      argv[]: parameter array
 *
 *   Returns:
 *      void
 */

int main() {
    int id, key;
    file_in = (char *)malloc(sizeof(char) * 1024);
    cout << "Please input a file name:" << endl;
    scanf("%s", file_in);               /* Get input file name */
    fin = fopen(file_in, "r+");
    if (!fin) {                         /* If input file is not exist or can't open */
        cout << "Can't open input file " <<  file_in << "! " << endl;
        return 1;
    }
    file_out = get_output_file(file_in);
    fout = fopen(file_out, "w");
    fscanf(fin, "%d", &s);
    fscanf(fin, "%d %d", &n, &m);
    if (n == 0) {                       /* There is no joined node */
        cout << "No joined node!" << endl;
        return 0;
    }
    exist_key = (int *)malloc(sizeof(int) * HASH_SPACE);
    for (int i = 0; i < HASH_SPACE; i++) {
        exist_key[i] = 0;               /* Initial exist key array */
    }
    st = (SuccTable **)malloc(sizeof(SuccTable *) * (s + 1));
    for (int i = 0; i < HASH_SPACE; i++)
        st[i] = NULL;                   /* Initial all node to not-joined state */
    for (int i = 0; i < n; i++) {
        fscanf(fin, "%d", &id);
        create_succ_table(id);          /* Join one node and create successor table for that node */
        for (int j = 0; j < HASH_SPACE; j++)
            if (st[j] && j != id)
                update_succ_table(j);   /* Update other nodes' successor table */
    }
    for (int i = 0; i < m; i++) {
        fscanf(fin, "%d", &key);
        exist_key[key] = 1;
        store_key(key);                 /* Store key */
    }
    //print_succ_table();
    while (1) {
        fscanf(fin, "%d %d", &key, &id);
        if (key == -1 && id == -1)      /* Until end */
            break;
        if (st[id] == NULL)             /* If that query node is not exist */
            fprintf(fout, "Node %d is not exist!\n", id);
        else if (!exist_key[key]) {     /* If this key is not exist */
            fprintf(fout, "Can't find this key!\n");
        } else {
            chord_query(id, key, 0);    /* Answer query */
        }
    }
}
