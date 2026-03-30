//
// Created by ricardo-alves on 3/16/26.
//
/**
 * @file Parser.h
 * @brief CSV parsing functions.
 */
#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <set>
#include <string>
#include "DataStructures.h"

/**
 * @brief Parses the dataset file into the respective data structures.
 * @param filename Path to the dataset file.
 * @param subs Vector to store parsed Submissions.
 * @param revs Vector to store parsed Reviewers.
 * @param params Struct to store algorithm parameters.
 * @param ctrl Struct to store control configurations.
 * @return True if parsing was successful, False otherwise.
 *
 */
bool parseDataset(const std::string& filename,
                  std::vector<Submission>& subs,
                  std::vector<Reviewer>& revs,
                  Parameters& params,
                  Control& ctrl);

#endif //PARSER_H
