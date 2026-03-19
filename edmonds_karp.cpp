//
// Created by pedrodomingos on 2026-03-19.
//
#include "Graph.h"
#include <queue>
#include <vector>
using namespace std;

template <class T>
void testAndVisit(std::queue< Vertex<T>*> &q, Edge<T> *e, Vertex<T> *w, double residual) {
    if (! w->isVisited() && residual > 0) {
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}

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
void edmondsKarp(Graph<T> *g, int source, int target) {
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
