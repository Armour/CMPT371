/*------------------------------------------------------
 *
 *  LS_Routing.cpp
 *
 *  Project         : LS_Routing
 *  Name            : Chong Guo
 *  Student ID      : 301295753
 *  Language        : C++
 *  SFU username    : armourg
 *
 *  Created by Armour on 06/12/2015
 *  Copyright (c) 2015 Armour. All rights reserved.
 *
 *------------------------------------------------------
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>

#define EDGE pair<int, int>
#define MAX_NODES 26
#define INF (1 << 30)
#define MAX_FILE_NAME_LENGTH 1024

using namespace std;

int n;                                      /* The number of nodes */
map<int, char> nodes;                       /* Map the nodes' name */
vector<int> dist(MAX_NODES, INF);           /* The minimal distance from source to this node */
vector<int> pre(MAX_NODES, 0);              /* The previous node from source to this node */
vector< vector< EDGE > > network(MAX_NODES, vector<EDGE>());        /* The network graph */
set< EDGE > paths;                          /* The paths that in queue */

/*
 * Function: Init
 * -------------------
 *   This function is used to read network direct links
 *
 *   Parameters:
 *      no parameters
 *
 *   Returns:
 *      void
 */

void init(void) {
    int len;
    char name, space;
    char file_in[MAX_FILE_NAME_LENGTH];                     /* The input file name */
    cout << "Please input a file name (with extensions):" << endl;
    cin >> file_in;                                         /* Get input file name */
    ifstream fin(file_in);
    if (!fin.is_open()) {                                   /* If input file is not exist or can't open */
        cout << "Can't open input file " <<  file_in << "!" << endl;
        exit(0);
    }
    fin >> n;
    fin.get(space);
    for (int i = 0; i < n; i++) {
        fin.get(name);                                      /* Read node name */
        fin.get(space);
        nodes.insert(make_pair(i, name));                   /* Map each node to one character */
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fin >> len;
            if (len > 0) {
                network[i].push_back(make_pair(len, j));    /* Read the network direct link for node i */
            }
        }
    }
    fin.close();                                            /* Close input file */
}

/*
 * Function: Dijkstra
 * -------------------
 *   This funciont is used to implement the routing algorithm
 *
 *   Parameters:
 *      no parameters
 *
 *   Returns:
 *      void
 */

void dijkstra(void) {
    dist[0] = 0;                            /* Init source node distance to 0 */
    paths.insert(make_pair(0, 0));          /* Insert source node to paths */
    while (!paths.empty()) {
        EDGE top = *paths.begin();          /* Get the nearest node */
        paths.erase(paths.begin());
        int v1 = top.second;
        for (auto link: network[v1]) {      /* Iterate through each link start from the nearest node */
            int len2 = link.first;
            int v2 = link.second;
            if (dist[v1] + len2 < dist[v2]) {               /* If find a better path to one node v2 */
                if (dist[v2] != INF)                        /* Erase old pair for node v2 in queue */
                    paths.erase(paths.find(make_pair(dist[v2], v2)));
                dist[v2] = dist[v1] + len2;                 /* Update the shortest path of v2 */
                pre[v2] = v1;                               /* Update the previous node of v2 */
                paths.insert(make_pair(dist[v2], v2));      /* Add new pair for node v2 into queue */
            }
        }
    }
}

/*
 * Function: Print
 * -------------------
 *   This function is used to print out the result path
 *
 *   Parameters:
 *      no parameters
 *
 *   Returns:
 *      void
 */

void print(void) {
    for (int i = 1; i < n; i++) {
        cout << nodes.find(i)->second << " (";
        int p = i;
        while (pre[p] != 0) {                               /* Find previous node until source node */
            p = pre[p];
        }
        cout << nodes.find(0)->second << "-" << nodes.find(p)->second << ") ";          /* Print the forwarding table */
        cout << dist[i] << endl;                            /* Print the shortest path distance */
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
    init();                 /* Input network graph */
    dijkstra();             /* Routing algorithm */
    print();                /* Output forwarding table and shortest path */
}

