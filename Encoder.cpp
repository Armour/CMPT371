/*------------------------------------------------------
 *
 *  Encoder.cpp
 *
 *  Project         : Base64_Encoder
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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;

FILE *fin, *fout;
char *file_in, *file_out;            /* The file name of input and output file */

/*
 * Function: Base64_table
 * -------------------
 *   This function is used to map integer to character
 *   using base64 encode table.
 *
 *   Parameters:
 *      i: the integer that need to map
 *
 *   Returns:
 *      the character that mapped from i
 */

char base64_table(int i) {
    if (i >= 0 && i < 26)                   /* 'A'-'Z' */
        return (char)(i + 65);
    else if (i >= 26 && i < 52) {           /* 'a'-'z' */
        return (char)(i + 97 - 26);
    } else if (i >= 52 && i < 62) {         /* '0'-'9' */
        return (char)('0' + i - 52);
    } else if (i == 62)                     /*   '+'   */
        return '+';
    else                                    /*   '/'   */
        return '/';
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
    int newline = 0;        /* Used to decide when to add a newline character */
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
        count++;
        bin = (bin << 8) + (int)c;
        if (count == 3) {                   /* If already have 3 characters */
            fprintf(fout, "%c", base64_table(bin >> 18));
            bin = bin & 0x03ffff;
            if (++newline == 76) {
                newline = 0;
                fprintf(fout, "\n");
            }
            fprintf(fout, "%c", base64_table(bin >> 12));
            bin = bin & 0x000fff;
            if (++newline == 76) {
                newline = 0;
                fprintf(fout, "\n");
            }
            fprintf(fout, "%c", base64_table(bin >> 6));
            bin = bin & 0x00003f;
            if (++newline == 76) {
                newline = 0;
                fprintf(fout, "\n");
            }
            fprintf(fout, "%c", base64_table(bin));
            if (++newline == 76) {
                newline = 0;
                fprintf(fout, "\n");
            }
            count = 0;
            bin = 0;
        }
    }
    if (count == 1) {                       /* If at end there is only one charcater */
        bin <<= 4;
        fprintf(fout, "%c", base64_table(bin >> 6));
        bin = bin & 0x00003f;
        if (++newline == 76) {
            newline = 0;
            fprintf(fout, "\n");
        }
        fprintf(fout, "%c==", base64_table(bin));
    } else if (count == 2) {                /* If at end there are two characters */
        bin <<= 2;
        fprintf(fout, "%c", base64_table(bin >> 12));
        bin = bin & 0x000fff;
        if (++newline == 76) {
            newline = 0;
            fprintf(fout, "\n");
        }
        fprintf(fout, "%c", base64_table(bin >> 6));
        bin = bin & 0x00003f;
        if (++newline == 76) {
            newline = 0;
            fprintf(fout, "\n");
        }
        fprintf(fout, "%c=", base64_table(bin));
    }
    cout << "Encode done!" << endl;
}
