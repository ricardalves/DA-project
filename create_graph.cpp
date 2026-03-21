//
// Created by pedrodomingos on 2026-03-21.
//

#include "Graph.h"
#include "Parser.h"
#include "DataStructures.h"
using namespace std;

void link_subs_to_revs(vector<Vertex<Submission>*> subs, vector<Vertex<Reviewer>*> revs, Control ctrl) {
    if (ctrl.genAssignments==0){
        for (auto sub: subs) {
            for (auto rev: revs) {
                if ((sub->getInfo().primary==rev->getInfo().primary)||(sub->getInfo().primary==rev->getInfo().secondary)||(sub->getInfo().secondary==rev->getInfo().primary)||(sub->getInfo().secondary==rev->getInfo().secondary)) {
                        sub->addEdge(rev,1);
                }
            }
        }
    }
    if (ctrl.genAssignments==1) {
        for (auto sub: subs) {
            for (auto rev: revs) {
                if (sub->getInfo().primary==rev->getInfo().primary) {
                    sub->addEdge(rev,1);
                }
            }
        }
    }
    if (ctrl.genAssignments==2) {
        for (auto sub: subs) {
            for (auto rev: revs) {
                if (sub->getInfo().primary==rev->getInfo().primary||sub->getInfo().secondary==rev->getInfo().primary) {
                    sub->addEdge(rev,1);
                }
            }
        }
    }
    if (ctrl.genAssignments==3){
        for (auto sub: subs) {
            for (auto rev: revs) {
                if ((sub->getInfo().primary==rev->getInfo().primary)||(sub->getInfo().primary==rev->getInfo().secondary)||(sub->getInfo().secondary==rev->getInfo().primary)||(sub->getInfo().secondary==rev->getInfo().secondary)) {
                    sub->addEdge(rev,1);
                }
            }
        }
    }
}

template <typename T>
Graph<T>* create_graph(const string& filename, vector<Submission>& subs, vector<Reviewer>& revs, Parameters& params, Control& ctrl) {
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
    link_subs_to_revs(subs_ver,revs_ver,ctrl);
    return graph;
}