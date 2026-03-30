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

//esta função preenche os campos da struct usando a função do parser
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

//função para criar o grafo
Graph<string>* createGraph(graph_info info) {
    Graph<string>* graph= new Graph<string>;
    vector<Submission> subs_ver;
    vector<Reviewer> revs_ver;
    graph->addVertex("Source");
    graph->addVertex("Target");
    for (auto s: info.submissions) {
        graph->addVertex(s.title);//usei title e não o email, pq pode ser a mesma pessoa a postar o artigo
        subs_ver.push_back(s);
        graph->addEdge("Source",s.title,info.parameters.minReviewsSub);//conecta o Source aos vértices de submission
        //graph->findVertex(s.title)->setNum(s.id);
    }
    for (auto r: info.reviewers) {
        graph->addVertex(r.email);
        revs_ver.push_back(r);
        graph->addEdge(r.email,"Target",info.parameters.maxReviewsRev);//conecta os vértices dos reviewers ao vértice target
        graph->findVertex(r.email)->setNum(r.id);//este num vai ser necessário para ajudar no output do risk analysis
    }
    runGenerateAssignments(graph,subs_ver,revs_ver,info.control);//função que conecta submissions aos reviewers
    runMaxFlowEdmondsKarp(graph,"Source","Target");//aplicação do algortimo de maxflow (O(V*E²))
    return graph;
}



void runGenerateAssignments(Graph<string>* g, vector<Submission> subs, vector<Reviewer> revs, Control ctrl) {
    set<string> verify;//para evitar reviewers repetidos (olhar csv 2 e 3) (size_t pq o size de um set é desse tipo)
    size_t n=0;//variável para ir checkando se o set aumenta
    vector<Reviewer> rev_unique;//vector sem revisores duplicados
    for (auto rev: revs) {//vamos analisar o vector original
        verify.insert(rev.email);
        n++;
        if (n>verify.size()) {//se n for maior que o conjunto é para descartar o item pois é repetido
            n=verify.size();//endireitamos a variavel
            continue;//skipamos para o proximo elemento
        }
        rev_unique.push_back(rev);//se nao acontecer é pq é um novo revisor e ent adiconados ao vector filtrado
    }
    if (ctrl.genAssignments==0){//contando primárias e secundárias
        for (auto sub: subs) {
            for (auto rev: rev_unique) {
                if ((sub.primary==rev.primary)||(sub.primary==rev.secondary)||(sub.secondary==rev.primary)||(sub.secondary==rev.secondary && (sub.secondary!=0||rev.secondary!=0))) {
                    g->findVertex(sub.title)->addEdge(g->findVertex(rev.email),1);
                }
            }
        }
    }
    if (ctrl.genAssignments==1) {//contando só primárias
        for (auto sub: subs) {
            for (auto rev: rev_unique) {
                if (sub.primary==rev.primary) {
                    g->findVertex(sub.title)->addEdge(g->findVertex(rev.email),1);
                }
            }
        }
    }
    if (ctrl.genAssignments==2) {//contando primárias e secundárias das submission e as primárias dos reviewers
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



//esta função serve para verificar se é possível atribuir tudo certinho sem necessidade de precisar de mais reviewers
bool isAssignmentValid(const Graph<string>* g, Parameters params) {//será chamada na função abaixo, talvez percebam melhor
    for (auto e:g->findVertex("Source")->getAdj()) {
        if (e->getFlow()<params.minReviewsSub) return false;//se o flow que sair da Source para uma submission for menor que o mínimo preciso signifca que a ausencia de um certo reviewer faz a diferenca e esse será necessario
    }
    return true;
}


vector<int> runRiskAnalysis(graph_info info) {//recebe a informação mas o mais importante será os reviewers
    vector<int> ids;//neste vector estarão os ids dos reviewers que se ausentando farão a diferença
    int id_rev;//variável para guardar o ID de cada reviewer
    if (info.control.riskAnalysis==1) { //será analisado o caso se 1 deles faltar
        for (auto rev: info.reviewers) {//percorremos todos os reviewers
            vector<Reviewer> revs;//onde vamos guardar os reviewers que sao diferentes ao que tá a ser analisado no primeiro loop
            for (auto rev1: info.reviewers) {
                if (rev1.id!=rev.id) {//se nao for o reviewer do primeiro loop
                    revs.push_back(rev1);//adicionas ao vector
                }
                else id_rev=rev1.id;//se for guardamos o id dele na variável
            }
            info.reviewers=revs;//atualizamos a informacao, mais especificamente os reviewers, deixando de fora o do primeiro loop(é assim que deixamos de fora 1 a 1 para analisar cada casa de ausencia singular)
            Graph<string>* g = createGraph(info);//criamos um grafo chamando a funcao de criaçao coma info atualizada
            if (!isAssignmentValid(g,info.parameters)) ids.push_back(id_rev);//se não se conseguir chegar a uma atribuiçao o sujeito que tá ausente é necessário, entao adicionamos ao vector dos ID's dos necessários
            delete g;//damos delete no grafo criado para analisar o caso para economizar memória
        }
    }
    sort(ids.begin(), ids.end()); //este sort é para depois no ouput apresentar em ordem tal como pedido
    ids.erase(unique(ids.begin(), ids.end()), ids.end());//apagar possíveis elementos duplos
    return ids;//retornar o vector com os necessários
}
