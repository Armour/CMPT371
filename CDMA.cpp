/*------------------------------------------------------
 *
 *  CDMA.cpp
 *
 *  Project         : CDMA
 *  Name            : Chong Guo
 *  Student ID      : 301295753
 *  Language        : C++
 *  SFU username    : armourg
 *
 *  Created by Armour on 05/12/2015
 *  Copyright (c) 2015 Armour. All rights reserved.
 *
 *------------------------------------------------------
 */

#include <cstdio>
#include <iostream>
#include <fstream>
#include <cmath>

#define MAX_USERS_NUMBER 8
#define MAX_CODE_LENGTH 16
#define MAX_DATABITS_LENGTH 8
#define MAX_FILE_NAME_LENGTH 1024

using namespace std;

int n;                                                  /* The number of users */
int len_code;                                           /* The length of code */
int len_dbs;                                            /* The length of data bits */
int code[MAX_USERS_NUMBER][MAX_CODE_LENGTH];            /* The code for different users */
int dbs[MAX_USERS_NUMBER][MAX_DATABITS_LENGTH];         /* The data bits for different users */
int encode[MAX_DATABITS_LENGTH][MAX_CODE_LENGTH];       /* The encoded bits */

/*
 * Function: Init
 * -------------------
 *   This function is used to read input from input file
 *
 *   Parameters:
 *      no parameters
 *
 *   Returns:
 *      void
 */

void init(void) {
    char file_in[MAX_FILE_NAME_LENGTH];                     /* The input file name */
    cout << "Please input a file name (with extensions):" << endl;
    cin >> file_in;                                         /* Get input file name */
    ifstream fin(file_in);
    if (!fin.is_open()) {                                   /* If input file is not exist or can't open */
        cout << "Can't open input file " <<  file_in << "!" << endl;
        exit(0);
    }
    fin >> n >> len_code >> len_dbs;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < len_code; j++)
            fin >> code[i][j];                              /* Read code for different users */
    for (int i = 0; i < n; i++)
        for (int j = 0; j < len_dbs; j++)
            fin >> dbs[i][j];                               /* Read data bits for different users */
    fin.close();
}


/*
 * Function: Check
 * -------------------
 *   This function is used to check if the codes are valid
 *
 *   Parameters:
 *      no parameters
 *
 *   Returns:
 *      void
 */

void check(void) {
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {               /* Choose each pair of code */
            int tmp = 0;
            for (int k = 0; k < len_code; k++)
                tmp += code[i][k] * code[j][k];
            if (tmp != 0) {                             /* Test if codes are orthogonal to each other */
                cout << "no" << endl;
                exit(EXIT_FAILURE);
            }
        }
    }
}

/*
 * Function: CDMA
 * -------------------
 *   This function is used to implemented CDMA algorithm
 *
 *   Parameters:
 *      no parameters
 *
 *   Returns:
 *      void
 */

void cdma(void) {
    memset(encode, 0, sizeof(encode));
    for (int i = 0; i < len_dbs; i++)
        for (int j = 0; j < len_code; j++)
            for (int k = 0; k < n; k++)
                encode[i][j] += dbs[k][i] * code[k][j];         /* Calculate the encoded code */
    cout << "Yes" << endl;
    for (int i = 0; i < len_dbs; i++) {
        cout << encode[i][0];
        for (int j = 1; j < len_code; j++) {
            cout << " " << encode[i][j];                        /* Output the encoded code */
        }
        cout << endl;
    }
}

/*
 * Function: Main
 * -------------------
 *   The main function
 *
 *   Parameters:
 *      no parameters
 *
 *   Returns:
 *      void
 */

int main() {
    init();
    check();
    cdma();
}
