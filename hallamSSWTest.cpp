#include <iostream>
#include <fstream>
#include <string>

#include "ssw_cpp.h"

typedef struct _AlignmentDetails {
    StripedSmithWaterman::Alignment* aln;

    std::string queryName;
    std::string querySequence;

    std::string targetName;
    std::string targetSequence;
} Details;

static void readQueryAndTargets(const char* queryFile, const char* targetFile);
static void printHeader();
static void printDetails(const Details& details);
static size_t computeIdentity(const Details& details); 

static const char TAB = '\t';

int main(int argc, char** argv) {
    // parse parameters
    // expected usage : ./hallam_ssw_test <target_db> <query>

    if (argc < 3 || argc > 3) {
        std::cout << "Invalid parameters! Usage : ./hallam_ssw_test <target_file> <query_file> \n";
        return 0;
    }

    const char* targetFile = argv[1];
    const char* queryFile = argv[2];
 
    printHeader();
    readQueryAndTargets(queryFile, targetFile);
}

static void readQueryAndTargets(const char* queryFile, const char* targetFile) {
    // read in each query sequence, run it against all target sequences
    std::ifstream queryStream(queryFile);

    if(queryStream.is_open()) {

        std::string queryLine;
        std::string queryName;
        std::string targetName;

        while (queryStream >> queryLine) {
            if (queryLine[0] == '>') {
                queryName = queryLine.substr(1);
            } else {

                // looking at the sequence
                std::string querySequence = queryLine;
                
                // run it against all targets
                std::string targetLine;

                std::ifstream targetStream(targetFile);
                if ( targetStream.is_open()) {
                    while (targetStream >> targetLine) {
                        if (targetLine[0] == '>') {
                            targetName = targetLine.substr(1);
                        } else {
                
                            StripedSmithWaterman::Aligner aligner;
                            StripedSmithWaterman::Filter filter;
                            StripedSmithWaterman::Alignment alignment;
                            
                            std::string targetSequence = targetLine;

                            aligner.Align(querySequence.c_str(), targetSequence.c_str(), querySequence.size(), filter, &alignment);

                            Details details;
                            details.aln = &alignment;
                            details.queryName = queryName;
                            details.targetName = targetName;
                            details.querySequence = querySequence;
                            details.targetSequence = targetSequence;
                           
                            printDetails(details);
                       } 
                    }
                }
                targetStream.close();
            }
        }
        queryStream.close();

    } else {
        std::cout << "Error opening files for reading!" << std::endl;
    }
}

static void printHeader() {
    std::cout << "Q_ID" << TAB
              << "S_ID" << TAB
              << "IDENT" << TAB
              << "ALIGN_LEN" << TAB
              << "MISMATCHES" << TAB
              << "GAPS" << TAB
              << "Q_BEG" << TAB
              << "Q_END" << TAB
              << "S_BEG" << TAB
              << "S_END" << TAB
              << "E_VAL" << TAB
              << "BIT_SCORE";

    std::cout << std::endl;
}

static size_t computeIdentity(const Details& details) {
    return 0;
}

static void printDetails(const Details& details) {

    size_t identity = 0;
    size_t gaps = 0;
    size_t alignmentSize = details.aln->ref_end - details.aln->ref_begin;
    size_t mismatches = details.aln->mismatches;
    double bitScore = 0;
    double evalue = 0;

    std::cout << details.queryName << TAB
              << details.targetName << TAB
              << identity << TAB
              << alignmentSize << TAB
              << mismatches << TAB
              << gaps << TAB
              << details.aln->query_begin << TAB
              << details.aln->query_end << TAB
              << details.aln->ref_begin << TAB
              << details.aln->ref_end << TAB
              << evalue << TAB
              << bitScore;

    std::cout << std::endl;
}
