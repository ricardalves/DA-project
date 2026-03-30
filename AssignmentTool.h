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


//fiz esta struct porque tornou muito mais versátil o uso das funções para evitar chamar a funcão do parser
struct graph_info {
    vector<Submission> submissions;
    vector<Reviewer> reviewers;
    Parameters parameters;
    Control control;
};


graph_info populateInfo(const string& filename);

Graph<string>* createGraph(graph_info info);

void runGenerateAssignments(Graph<string>* g, vector<Submission> subs, vector<Reviewer> revs, Control ctrl);


vector<int> runRiskAnalysis(graph_info);

template <class T>
void runMaxFlowEdmondsKarp(Graph<T> *g, string source, string target);

bool isAssignmentValid(const Graph<string>* g,Parameters params);



#endif //PROJETO1_ASSIGNMENTTOOL_H