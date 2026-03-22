//
// Created by pedrodomingos on 2026-03-21.
//

#ifndef PROJETO1_OUTPUT_H
#define PROJETO1_OUTPUT_H

#include "DataStructures.h"
#include <vector>
using namespace std;

bool output(const string& filename,
                  vector<Submission>& subs,
                  vector<Reviewer>& revs,
                  Parameters& params,
                  Control& ctrl);


#endif //PROJETO1_OUTPUT_H