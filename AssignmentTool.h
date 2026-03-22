//
// Created by pedrodomingos on 2026-03-21.
//

#ifndef PROJETO1_ASSIGNMENTTOOL_H
#define PROJETO1_ASSIGNMENTTOOL_H

#include "DataStructures.h"
#include "Graph.h"
#include <vector>
#include <string>
#include <queue>

using namespace std;

void runGenerateAssignments(vector<Vertex<Submission>*> subs, vector<Vertex<Reviewer>*> revs, Control ctrl);

void runRiskAnalysis();

template <class T>
void runMaxFlowEdmondsKarp(Graph<T> *g, int source, int target);


#endif //PROJETO1_ASSIGNMENTTOOL_H