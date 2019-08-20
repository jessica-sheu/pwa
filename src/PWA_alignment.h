#ifndef PWA_ALIGNMENT_H
#define PWA_ALIGNMENT_H

#include "PWA_message.h"

#include <map>
#include <string>
#include <vector>

using namespace std;

class PWA_alignment
{
public:
    PWA_alignment();
    void begin_PWA_alignment(PWA_message *msg_obj);

    map<string, int> scoring_map;

    vector<string> names_vector;
    vector<string> sequences_vector;
    bool scoring_specified;
    int alignment_score;
    int number_aligned;

private:
    void resize_alignment_matrix(void);
    void fill_alignment_matrix(void);
    void get_max_score(int i, int j);
    void get_step_direction(int left_score, int up_score);
    void trace_back_steps(void);
    void compute_alignment_score(void);

    vector<vector<int> >alignment_matrix;
    vector<char> steps_vector;

    int diagonal_score;
    int gap_penalty;
    int max_score;
    int width, height;

};  // PWA_alignment

#endif  // PWA_ALIGNMENT_H

