//
// Created by pedrodomingos on 2026-03-21.
//

#include "Output.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

struct Assignment {
    int subId;
    int revId;
    int matchDomain;
};

struct Missing {
    int subId;
    int domain;
    int missingCount;
};

// finds out what domain "matched" the sub and the rev
int getMatch(const Submission& sub, const Reviewer& rev, int genAssign) {
    if (sub.primary == rev.primary) return sub.primary;
    if (genAssign == 3 || genAssign == 0) {
        if (sub.primary == rev.secondary && rev.secondary != 0) return sub.primary;
        if (sub.secondary == rev.primary && sub.secondary != 0) return sub.secondary;
        if (sub.secondary == rev.secondary && sub.secondary != 0) return sub.secondary;
    }
    if (genAssign == 2 && sub.secondary == rev.primary && sub.secondary != 0) return sub.secondary;
    return sub.primary;
}

void generateOutput(const Graph<string>* g, const graph_info& info, const vector<int>& risk) {
    string filename = info.control.outputFileName;

    ofstream out(filename);
    if (!out.is_open()) {
        cerr << "Error creating the file\n"; return;
    }

    if (info.control.genAssignments != 0) {
        vector<Assignment> assigns;
        vector<Missing> missings;

        for (const auto& sub : info.submissions) {
            Vertex<string>* v = g->findVertex(sub.title);
            if (!v) continue;

            int currentReviews = 0;

            for (auto e : v->getAdj()) {
                if (e->getFlow() > 0 && e->getDest()->getInfo() != "Source") {
                    currentReviews++;
                    string revEmail = e->getDest()->getInfo();

                    for (const auto& rev : info.reviewers) {
                        if (rev.email == revEmail) {
                            assigns.push_back({sub.id, rev.id, getMatch(sub, rev, info.control.genAssignments)});
                            break;
                        }
                    }
                }
            }

            int missingCount = info.parameters.minReviewsSub - currentReviews;
            if (missingCount > 0) {
                missings.push_back({sub.id, sub.primary, missingCount});
            }
        }

        // sorting based on the subID and printing Submission -> Reviewer first
        sort(assigns.begin(), assigns.end(), [](Assignment a, Assignment b) {
            if (a.subId != b.subId) return a.subId < b.subId;
            return a.revId < b.revId;
        });
        out << "#SubmissionId,ReviewerId,Match\n";
        for (auto a : assigns) out << a.subId << ", " << a.revId << ", " << a.matchDomain << "\n";

        // sorting based on revID and then printing Reviewer -> Submission
        sort(assigns.begin(), assigns.end(), [](Assignment a, Assignment b) {
            if (a.revId != b.revId) return a.revId < b.revId;
            return a.subId < b.subId;
        });
        out << "#ReviewerId,SubmissionId,Match\n";
        for (auto a : assigns) out << a.revId << ", " << a.subId << ", " << a.matchDomain << "\n";

        out << "#Total: " << assigns.size() << "\n";

        if (!missings.empty()) {
            sort(missings.begin(), missings.end(), [](Missing a, Missing b) { return a.subId < b.subId; });
            out << "#SubmissionId,Domain,MissingReviews\n";
            for (auto m : missings) out << m.subId << ", " << m.domain << ", " << m.missingCount << "\n";
        }
    }

    if (info.control.riskAnalysis >= 1) {
        out << "#Risk Analysis: " << info.control.riskAnalysis << "\n";
        for (size_t i = 0; i < risk.size(); ++i) {
            out << risk[i] << (i < risk.size() - 1 ? ", " : "");
        }
        out << "\n";
    }

    out.close();
}