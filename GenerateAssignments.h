//
// Created by pedrodomingos on 2026-03-21.
//

#ifndef PROJETO1_GENERATEASSIGNMENTS_H
#define PROJETO1_GENERATEASSIGNMENTS_H

#include "DataStructures.h"
#include "Graph.h"
#include <vector>
using namespace std;

void link_subs_to_revs(vector<Vertex<Submission>*> subs, vector<Vertex<Reviewer>*> revs, Control ctrl);

#endif //PROJETO1_GENERATEASSIGNMENTS_H