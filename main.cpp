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
    Graph<string>* graph=createGraph("input/dataset1.csv");
    for (auto v:graph->getVertexSet()) {
        cout<<"Vértice: "<<v->getInfo()<<endl;
        for (auto e: v->getAdj()) {
            cout<<e->getDest()->getInfo()<<" ";
        }
        cout<<endl;
    }
    return 0;
}