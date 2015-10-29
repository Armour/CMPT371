/*------------------------------------------------------
 *
 *  Decoder.cpp
 *
 *  Project         : Base64_Decoder
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
#include <fstream>

using namespace std;

FILE *fin, *fout;
char *file_in, *file_out;            /* The file name of input and output file */

/*
 * Function: Base64_table
 * -------------------
 *   This function is used to map character to integer
 *   using base64 encode table.
 *
 *   Parameters:
 *      c: the character that need to map
 *
 *   Returns:
 *      the integer that mapped from c
 */

int base64_table(char c) {
    if (c >= 'A' && c <= 'Z')               /* 'A'-'Z' */
        return (int)(c - 'A');
    else if (c >= 'a' && c <= 'z') {        /* 'a'-'z' */
        return (int)(c - 'a' + 26);
    } else if (c >= '0' && c <= '9') {      /* '0'-'9' */
        return (char)(c - '0' + 52);
    } else if (c == '+')                    /*   '+'   */
        return 62;
    else                                    /*   '/'   */
        return 63;
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
 *      no parameters
 *
 *   Returns:
 *      void
 */

int main() {
    int count = 0;          /* Used to decide when to decode */
    int bin = 0;            /* Used to store the binary number */
    char c;
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
    while (!feof(fin)) {
        fscanf(fin, "%c", &c);
        if (c == '\n') continue;            /* New line character should be ignore */
        if (c == '=') {                     /* If read until the end of file */
            if (count == 3) {               /* If just 3 characters in the end */
                bin >>= 2;
                fprintf(fout, "%c", bin >> 8);
                bin = bin & 0x0000ff;
                fprintf(fout, "%c", bin);
            } else if (count == 2) {        /* If just 2 characters in the end */
                bin >>= 4;
                fprintf(fout, "%c", bin);
            }
            return 0;
        }
        count++;
        bin = (bin << 6) + base64_table(c);     /* Update binary number using base64 table */
        if (count == 4) {                       /* If already have 4 characters */
            fprintf(fout, "%c", bin >> 16);
            bin = bin & 0x00ffff;
            fprintf(fout, "%c", bin >> 8);
            bin = bin & 0x0000ff;
            fprintf(fout, "%c", bin);
            count = 0;                          /* Update count and bin to initial value */
            bin = 0;
        }
    }
    cout << "Decode done!" << endl;
}
