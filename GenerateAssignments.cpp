//
// Created by pedrodomingos on 2026-03-21.
//

#include "GenerateAssignments.h"

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

