//
// Created by pedrodomingos on 2026-03-21.
//
#include "Graph.h"
#include "DataStructures.h"
#include <vector>
#include <iostream>
#include <string>
#include "Parser.h"
#include "AssignmentTool.h"
using namespace std;


int main() {
    graph_info gi=populateInfo("input/dataset14.csv");
    Graph<string>* g=createGraph(gi);
    for (auto v:g->getVertexSet()) {
        cout<<"Vértice: "<<v->getInfo()<<endl;
        cout<<"Adjacents: ";
        for (auto e: v->getAdj()) {
            cout<<"Flow: "<<e->getFlow()<<" "<<e->getDest()->getInfo()<<" ";
        }
        cout<<endl;
    }
    vector<int> risk=runRiskAnalysis(gi);
    for (int i:risk) {
        cout<<i<<endl;
    }
    cout<<"lista de necessários a cima"<<endl;
    return 0;
}