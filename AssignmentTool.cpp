//
// Created by pedrodomingos on 2026-03-21.
//

#include "AssignmentTool.h"
#include "Parser.h"
#include <queue>
#include <iostream>
#include <string>
#include "Graph.h"
#include <vector>
using namespace std;

graph_info populateInfo(const string &filename) {
    graph_info info;
    vector<Submission> subs;
    vector<Reviewer> revs;
    Parameters params;
    Control ctrl;
    parseDataset(filename,subs,revs,params,ctrl);
    info.submissions=subs;
    info.reviewers=revs;
    info.parameters=params;
    info.control=ctrl;
    return info;
}

Graph<string>* createGraph(graph_info info) {
    Graph<string>* graph= new Graph<string>;
    vector<Submission> subs_ver;
    vector<Reviewer> revs_ver;
    graph->addVertex("Source");
    graph->addVertex("Target");
    for (auto s: info.submissions) {
        graph->addVertex(s.title);
        subs_ver.push_back(s);
        graph->addEdge("Source",s.title,info.parameters.minReviewsSub);
        graph->findVertex(s.title)->setNum(s.id);
    }
    for (auto r: info.reviewers) {
        graph->addVertex(r.email);
        revs_ver.push_back(r);
        graph->addEdge(r.email,"Target",info.parameters.maxReviewsRev);
        graph->findVertex(r.email)->setNum(r.id);
    }
    runGenerateAssignments(graph,subs_ver,revs_ver,info.control);
    runMaxFlowEdmondsKarp(graph,"Source","Target");
    return graph;
}



void runGenerateAssignments(Graph<string>* g, vector<Submission> subs, vector<Reviewer> revs, Control ctrl) {
    set<string> verify;
    size_t n=0;
    vector<Reviewer> rev_unique;
    for (auto rev: revs) {
        verify.insert(rev.email);
        n++;
        if (n>verify.size()) {
            n=verify.size();
            continue;
        }
        rev_unique.push_back(rev);
    }
    if (ctrl.genAssignments==0){
        for (auto sub: subs) {
            for (auto rev: rev_unique) {
                if ((sub.primary==rev.primary)||(sub.primary==rev.secondary)||(sub.secondary==rev.primary)||(sub.secondary==rev.secondary && (sub.secondary!=0||rev.secondary!=0))) {
                    g->findVertex(sub.title)->addEdge(g->findVertex(rev.email),1);
                }
            }
        }
    }
    if (ctrl.genAssignments==1) {
        for (auto sub: subs) {
            for (auto rev: rev_unique) {
                if (sub.primary==rev.primary) {
                    g->findVertex(sub.title)->addEdge(g->findVertex(rev.email),1);
                }
            }
        }
    }
    if (ctrl.genAssignments==2) {
        for (auto sub: subs) {
            for (auto rev: rev_unique) {
                if (sub.primary==rev.primary||sub.secondary==rev.primary) {
                    g->findVertex(sub.title)->addEdge(g->findVertex(rev.email),1);
                }
            }
        }
    }
    if (ctrl.genAssignments==3){
        for (auto sub: subs) {
            for (auto rev: rev_unique) {
                if ((sub.primary==rev.primary)||(sub.primary==rev.secondary)||(sub.secondary==rev.primary)||(sub.secondary==rev.secondary && (sub.secondary!=0||rev.secondary!=0))) {
                    g->findVertex(sub.title)->addEdge(g->findVertex(rev.email),1);
                }
            }
        }
    }
}

/**
 * @brief Auxiliary function used by BFS to visit an adjacent vertex and mark its augmenting path.
 * @param q Reference to the queue used in BFS.
 * @param residual The remaining capacity (residual flow) on the edge.
 */
template <class T>
void testAndVisit(std::queue< Vertex<T>*> &q, Edge<T> *e, Vertex<T> *w, double residual) {
    if (! w->isVisited() && residual > 0) {
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}

/**
 * @brief Finds an augmenting path in the residual graph using Breadth-First Search (BFS).
 * @return True if a valid augmenting path from source to target is found, False otherwise.
 */
template <class T>
bool findAugmentingPath(Graph<T> *g, Vertex<T> *s, Vertex<T> *t) {
    for (auto v: g->getVertexSet()) {
        v->setVisited(false);
    }
    queue<Vertex<T>*> q;
    q.push(s);
    s->setVisited(true);
    while (!q.empty()) {
        Vertex<T>* v=q.front();
        q.pop();
        for (auto e: v->getAdj()) {
            double residual=e->getWeight()-e->getFlow();
                testAndVisit(q, e,e->getDest(),residual);
        }
    }
    return t->isVisited();
}

/**
 * @brief Determines the minimum residual capacity (bottleneck) along a discovered augmenting path.
 * @return The bottleneck flow value that can be pushed through the path.
 */
template <class T>
double findMinResidualAlongPath(Vertex<T> *s, Vertex<T> *t) {
    double f = INF;
    Vertex<T>* v=t;
    while (v!=s) {
        if (v->getPath()->getWeight()-v->getPath()->getFlow()<f) {
            f=v->getPath()->getWeight()-v->getPath()->getFlow();
        }
        v=v->getPath()->getOrig();
    }
    return f;
}

/**
 * @brief Updates the flow along the augmenting path and inversely updates the residual edges.
 * @param f The flow value to add to the edges.
 */
template <class T>
void augmentFlowAlongPath(Vertex<T> *s, Vertex<T> *t, double f) {
    Vertex<T>* v=t;
    while (v!=s) {
        v->getPath()->setFlow(v->getPath()->getFlow()+f);
        v->getPath()->getReverse()->setFlow(v->getPath()->getReverse()->getFlow()-f);
        v=v->getPath()->getOrig();
    }
}

template <class T>
void runMaxFlowEdmondsKarp(Graph<T> *g, string source, string target) {
    Vertex<T>* s = g->findVertex(source);
    Vertex<T>* t = g->findVertex(target);
    vector<Edge<T>*> edges;
    for (auto v : g->getVertexSet()) {
        v->setVisited(false);
        for (auto e: v->getAdj()) {
            e->setFlow(0);
            edges.push_back(e);
        }
    }
    for (auto e: edges) {
        if (e->getReverse()==nullptr) {
            Edge<T>* edge=e->getDest()->addEdge(e->getOrig(),0);
            e->setReverse(edge);
            edge->setReverse(e);
            edge->setFlow(0);
        }
    }
    while (findAugmentingPath(g,s,t)) {
        double f = findMinResidualAlongPath(s, t);
        augmentFlowAlongPath(s,t,f);
    }
    for (auto v: g->getVertexSet()) {
        for (auto e: v->getAdj()) {
            if (e->getWeight()==0) {
                g->removeEdge(e->getOrig()->getInfo(),e->getDest()->getInfo());
            }
        }
    }
}



bool isAssignmentValid(const Graph<string>* g, Parameters params) {
    for (auto e:g->findVertex("Source")->getAdj()) {
        if (e->getFlow()<params.minReviewsSub) return false;
    }
    return true;
}


vector<int> runRiskAnalysis(graph_info info) {
    vector<int> ids;
    int id_rev;
    if (info.control.riskAnalysis==1) {
        for (auto rev: info.reviewers) {
            vector<Reviewer> revs;
            for (auto rev1: info.reviewers) {
                if (rev1.id!=rev.id) {
                    revs.push_back(rev1);
                }
                else id_rev=rev1.id;
            }
            info.reviewers=revs;
            Graph<string>* g = createGraph(info);
            if (!isAssignmentValid(g,info.parameters)) ids.push_back(id_rev);
            delete g;
        }
    }
    sort(ids.begin(), ids.end());
    ids.erase(unique(ids.begin(), ids.end()), ids.end());
    return ids;
}
