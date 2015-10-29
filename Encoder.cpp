#include<iostream>
#include<fstream>

using namespace std;

FILE *fin, *fout;

char base64_table(int i) {
    if (i >= 0 && i < 26)
        return (char)(i + 65);
    else if (i >= 26 && i < 52) {
        return (char)(i + 97 - 26);
    } else if (i >= 52 && i < 62) {
        return (char)('0' + i - 52);
    } else if (i == 62)
        return '+';
    else
        return '/';
}

int main() {
    int count = 0;
    int bin = 0;
    int newline = 0;
    char c;
    fin = fopen("origin.txt", "r");
    fout = fopen("encode.txt", "w+");
    if (!fin) {
        printf("File open failed!\n");
        return 0;
    }
    while (!feof(fin)) {
        fscanf(fin, "%c", &c);
        if (c == '\n') continue;
        count++;
        bin = (bin << 8) + (int)c;
        if (count == 3) {
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
    if (count == 1) {
        bin <<= 4;
        fprintf(fout, "%c", base64_table(bin >> 6));
        bin = bin & 0x00003f;
        if (++newline == 76) {
            newline = 0;
            fprintf(fout, "\n");
        }
        fprintf(fout, "%c==", base64_table(bin));
    } else if (count == 2) {
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
}
