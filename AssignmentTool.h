//
// Created by pedrodomingos on 2026-03-21.
//

#ifndef PROJETO1_ASSIGNMENTTOOL_H
#define PROJETO1_ASSIGNMENTTOOL_H

#include "DataStructures.h"
#include "Graph.h"
#include "Parser.h"
#include <vector>
#include <string>
#include <queue>

using namespace std;

Graph<string>* createGraph(const string& filename);

void runGenerateAssignments(Graph<string>* g, vector<Submission> subs, vector<Reviewer> revs, Control ctrl);

void runRiskAnalysis();

template <class T>
void runMaxFlowEdmondsKarp(Graph<T> *g, string source, string target);




#endif //PROJETO1_ASSIGNMENTTOOL_H