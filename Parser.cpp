//
// Created by ricardo-alves on 3/16/26.
//

#include "Parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "AssignmentTool.h"

// Helper function to remove spaces, \n, and quotes from a string
void trim(std::string &s) {
    size_t start = s.find_first_not_of(" \n\"");
    size_t end = s.find_last_not_of(" n\"");
    if (start == std::string::npos) s = "";
    else s = s.substr(start, end - start + 1);
}

bool parseDataset(const std::string& filename, std::vector<Submission>& subs, std::vector<Reviewer>& revs, Parameters& params, Control& ctrl)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: couldn't open file " << filename << std::endl;
        return false;
    }
    enum Section { NONE, SUBMISSIONS, REVIEWERS, PARAMETERS, CONTROL };

    Section section = NONE;
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty()) continue;
        if (line[0] == '#')
        {
            if (line=="#Submissions") section = SUBMISSIONS;
            else if (line=="#Reviewers") section = REVIEWERS;
            else if (line=="#Parameters") section = PARAMETERS;
            else if (line=="#Control") section = CONTROL;
            continue;
        }

        std::stringstream ss(line);

        if (section == SUBMISSIONS)
        {
            std::string id, title, authors, email, primary, secondary;

            std::getline(ss,id,',');
            std::getline(ss,title,',');
            std::getline(ss,authors,',');
            std::getline(ss,email,',');
            std::getline(ss,primary,',');
            std::getline(ss,secondary,',');

            trim(id);
            trim(title);
            trim(authors);
            trim(email);
            trim(primary);
            trim(secondary);

            Submission sub;
            sub.id = std::stoi(id);
            sub.title = title;
            sub.authors = authors;
            sub.email = email;
            sub.primary = std::stoi(primary);
            if (secondary.empty()) {sub.secondary=0;}
            else sub.secondary = std::stoi(secondary);

            subs.push_back(sub);
        }

        else if (section == REVIEWERS)
        {
            std::string id, name, email, primary, secondary;

            std::getline(ss,id,',');
            std::getline(ss,name,',');
            std::getline(ss,email,',');
            std::getline(ss,primary,',');
            std::getline(ss,secondary,',');

            trim(id);
            trim(name);
            trim(email);
            trim(primary);
            trim(secondary);

            Reviewer rev;

            rev.id = std::stoi(id);
            rev.name = name;
            rev.email = email;
            rev.primary = std::stoi(primary);
            if (secondary.empty()) {rev.secondary=0;}
            else rev.secondary = std::stoi(secondary);

            revs.push_back(rev);
        }

        else if (section == PARAMETERS||section == CONTROL)
        {
            std::string key, value;

            std::getline(ss,key,',');
            std::getline(ss,value,',');

            trim(key);
            trim(value);

            if (section == PARAMETERS)
            {
                if (key == "MinReviewsPerSubmission") params.minReviewsSub = std::stoi(value);
                else if (key == "MaxReviewsPerReviewer") params.maxReviewsRev = std::stoi(value);
                else if (key == "PrimaryReviewerExpertise") params.primaryExpertise = std::stoi(value);
                else if (key == "SecondaryReviewerExpertise") params.secondaryExpertise = std::stoi(value);
                else if (key == "PrimarySubmissionDomain") params.primaryDomain = std::stoi(value);
                else if (key == "SecondarySubmissionDomain") params.secondaryDomain = std::stoi(value);
            }
            else
            {
                if (key == "GenerateAssignments") ctrl.genAssignments = std::stoi(value);
                else if (key == "RiskAnalysis") ctrl.riskAnalysis = std::stoi(value);
                else if (key == "OutputFileName") ctrl.outputFileName = value;
            }
        }
    }
    file.close();
    return true;
}
