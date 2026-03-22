#include "Graph.h"
#include "DataStructures.h"
#include <vector>
#include <string>
#include "Parser.h"
#include "AssignmentTool.h"

using namespace std;
template <typename T>
Graph<T>* createGraph(const string& filename, vector<Submission>& subs, vector<Reviewer>& revs, Parameters& params, Control& ctrl) {
    Graph<T>* graph= new Graph<T>;
    vector<Vertex<Submission>*> subs_ver;
    vector<Vertex<Reviewer>*> revs_ver;
    parseDataset(filename, subs, revs, params, ctrl);
    graph->addVertex("Source");
    graph->addVertex("Target");
    for (auto s: subs) {
        Vertex<T>* v= new Vertex<T>(s);
        graph->addVertex(v);
        subs_ver.push_back(v);
        graph->addEdge(graph->findVertex("Source"),v,params.minReviewsSub);
    }
    for (auto r: revs) {
        Vertex<T>* v= new Vertex<T>(r);
        graph->addVertex(v);
        revs_ver.push_back(v);
        graph->addEdge(v,graph->findVertex("Target"),params.maxReviewsRev);
    }
    runGenerateAssignments(subs_ver,revs_ver,ctrl);
    runMaxFlowEdmondsKarp(graph,graph->findVertex("Source")->getInfo(),graph->findVertex("Target")->getInfo());
    return graph;
}