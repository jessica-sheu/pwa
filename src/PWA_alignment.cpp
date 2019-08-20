/*=======================================================================*/
/* Filename: PWA_alignment.cpp                                           */
/* Last updated: November 26, 2017                                       */
/*=======================================================================*/
/* Contains all methods to perform the Needleman-Wunsch pairwise         */
/* sequence alignment algorithm.                                         */
/*=======================================================================*/
#include "PWA_alignment.h"
#include "PWA_message.h"
#include "PWA_option.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

using namespace std;

// Uncomment to display debugging print statements.
//#define USEDEBUG_ALIGNMENT
#ifdef USEDEBUG_ALIGNMENT
#define debuga(x) cout << x
#else
#define debuga(x)
#endif


/*=======================================================================*/
/* Constructor: PWA_alignment                                            */
/*-----------------------------------------------------------------------*/
/* Initializes all integer and boolean variables.                        */
/*=======================================================================*/
PWA_alignment::PWA_alignment()
{
    alignment_score   =  0;
    gap_penalty       = -2;
    number_aligned    =  0;
    max_score         =  0;
    number_aligned    =  0;
    width = height    =  0;

    scoring_specified =  0;

}    // End PWA_alignment::PWA_alignment().


/*=======================================================================*/
/* Method: PWA_alignment::begin_PWA_alignment()                          */
/*-----------------------------------------------------------------------*/
/* Calls methods for the Needleman-Wunsch pairwise sequence alignment    */
/* algorithm.                                                            */
/*=======================================================================*/
void PWA_alignment::begin_PWA_alignment(PWA_message *msg_obj)
{
    resize_alignment_matrix();

    fill_alignment_matrix();

    trace_back_steps();

    compute_alignment_score();

}    // End PWA_alignment::begin_PWA_alignment()


/*=======================================================================*/
/* Method: PWA_alignment::resize_alignment_matrix()                      */
/*-----------------------------------------------------------------------*/
/* Resizes the dynamic matrix to be ((length of sequence 1) + 1) by      */
/* ((length of sequence 2) + 1). The matrix only contains the integer    */
/* values and not the characters of the sequences themselves in order    */
/* to make the matrix of only int data type for easier computations.     */
/*=======================================================================*/
void PWA_alignment::resize_alignment_matrix(void)
{
    // Set matrix width and height.
    width  = sequences_vector[0].length() + 1;
    height = sequences_vector[1].length() + 1;

    // Resize number of rows.
    alignment_matrix.resize(height);

    // Resize number of columns.
    for (int i = 0; i < height; i++)
    {
        alignment_matrix[i].resize(width);
    }

}    // End PWA_alignment::resize_alignment_matrix().


/*=======================================================================*/
/* Method: PWA_alignment::fill_alignment_matrix()                        */
/*-----------------------------------------------------------------------*/
/* Fills the alignment_matrix. The first row and column are filled with  */
/* increasing gap penalty, then the values for the rest of the matrix    */
/* are computed.                                                         */
/*                                                                       */
/* Also contains debugging print statements to print the entire          */
/* contents of alignment_matrix and steps_vector.                        */
/*=======================================================================*/
void PWA_alignment::fill_alignment_matrix(void)
{
    // Initialize matrix.
    int fill = 0;
    int i    = 0;
    int j    = 0;

    // Fill first row.
    for (j = 0; j < width; j++)
    {
        alignment_matrix[i][j] = fill;
        fill += gap_penalty;
        steps_vector.push_back('L');
    }
    fill = -2;
    j    = 0;

    // Fill first column.
    for (i = 1; i < height; i++)
    {
        alignment_matrix[i][j] = fill;
        fill += gap_penalty;
    }

    // Compute rest of matrix.
    for (i = 1; i < height; i++)
    {
        steps_vector.push_back('U');
        for (j = 1; j < width; j++)
        {
            get_max_score(i, j);
            alignment_matrix[i][j] = max_score;
        }
    }

#ifdef USEDEBUG_ALIGNMENT
    debuga(endl);
    debuga("Final matrix:" << endl);

    //Print all contents of matrix.
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            debuga(alignment_matrix[i][j] << "   ");
        }
        debuga(endl);
    }
    debuga(endl);

    // Print all step directions.
    debuga(endl << "Final step direction matrix:");
    int counter = 0;
    for (vector<char>::iterator it = steps_vector.begin();
         it != steps_vector.end(); ++it)
    {
        // For readability.
        if (counter%(width) == 0)
        {
            debuga(endl);
        } 
        debuga(*it << "   ");
        counter++;
    }
    debuga(endl << endl);
#endif

}    // End PWA_alignment::fill_alignment_matrix().


/*=======================================================================*/
/* Method: PWA_alignment::get_max_score()                                */
/*-----------------------------------------------------------------------*/
/* First computes the score from the diagonal. If a scoring matrix was   */
/* specified by the user, the program uses the scoring_map to look up    */
/* the score for the amino acid pair. Otherwise, the program scores -1   */
/* for mismatch and +1 for match.                                        */
/*                                                                       */
/* Then, finds the max_score out of the diagonal score, the score from   */
/* the left position, and the score from the upper position in the       */
/* the matrix. This resulting max_score is chosen for                    */
/* alignment_matrix[i][j].                                               */
/*=======================================================================*/
void PWA_alignment::get_max_score(int i, int j)
{
    // Get diagonal score.
    if (scoring_specified == 1)
    {
        string amino_pair = sequences_vector[0].substr(j-1, 1) + \
                            sequences_vector[1].substr(i-1, 1);

         transform(amino_pair.begin(), amino_pair.end(), \
                   amino_pair.begin(), ::toupper);

        diagonal_score = alignment_matrix[i-1][j-1] + \
                         scoring_map[amino_pair];
    } 
    else if (sequences_vector[0].at(j-1) ==
             sequences_vector[1].at(i-1))
    {
        // Default match score.
        diagonal_score = alignment_matrix[i-1][j-1] + 1;
    }
    else
    {
        // Default mismatch score.
        diagonal_score = alignment_matrix[i-1][j-1] - 1;
    }

    max_score = max(max(diagonal_score, \
                    alignment_matrix[i][j-1] + gap_penalty),
                    alignment_matrix[i-1][j] + gap_penalty);

    get_step_direction(alignment_matrix[i][j-1] + gap_penalty,
                       alignment_matrix[i-1][j] + gap_penalty);

 
}   // End PWA_alignment::get_max_score().


/*=======================================================================*/
/* Method: PWA_alignment::get_step_direction()                           */
/*-----------------------------------------------------------------------*/
/* Determines which direction the max_score was chosen from. Pushes      */
/* the direction to steps_vector for algorithm traceback later.          */
/* Currently, the program does not keep track of all possible            */
/* directions that max_score came from.                                  */
/*=======================================================================*/
void PWA_alignment::get_step_direction(int left_score, int up_score)
{
    if (max_score == diagonal_score)
    {
        steps_vector.push_back('D');
    }
    else if (max_score == left_score)
    {
        steps_vector.push_back('L');
    }
    else if (max_score == up_score)
    {
        steps_vector.push_back('U');
    }

}   // End PWA_alignment::get_step_direction().


/*=======================================================================*/
/* Method: PWA_alignment::trace_back_steps()                             */
/*-----------------------------------------------------------------------*/
/* Performs traceback of steps from where the score in the matrix        */
/* position was calculated. Traces back from the end of steps_vector,    */
/* indicating the last (right-most and bottom-most) position in the      */
/* matrix.                                                               */
/*                                                                       */
/* The string sequence_1 will hold the final sequence 1 with gaps        */
/* inserted. The string sequence_2 will hold the final sequence 2 with   */
/* gaps inserted. The string alignments will be printed between          */
/* sequence_1 and sequence_2 in the final output, and will indicate the  */
/* alignments between the two sequences with "|" characters.             */
/*                                                                       */
/* If the direction is "up," a gap is inserted into sequence_1 and the   */
/* char is taken from sequence_2. If the direction is "left," a gap      */
/* is inserted into sequence_2 and the char is taken from sequence_1.    */
/* If the direction is "diagonal," the char is taken from both           */
/* sequence_1 and sequence_2, and a "|" is inserted into the alignments  */
/* string to indicate the alignment between the two sequences.           */
/*                                                                       */
/* Since the program is backtracing from the last step, the characters   */
/* are also being taken from the end of the sequences first. For         */
/* example, in a sequence ABCD, the program would add the characters     */
/* D, to C, to B, then to A. Therefore, in order to keep the original    */
/* order of the sequence, the gaps, characters, and spaces are inserted  */
/* to the beginning of the strings.                                      */
/*                                                                       */
/* In the above example:                                                 */
/* "D" --> "C" + "D" = "CD" --> "B" + "CD" = "BCD"                       */
/*     --> "A" + "BCD" = "ABCD"                                          */
/*                                                                       */
/* In the end, clears sequences_vector of the original sequences and     */
/* replaces the contents with the final sequence and alignment strings   */
/* with gaps inserted, to be printed as the final output.                */
/*=======================================================================*/
void PWA_alignment::trace_back_steps(void)
{
    // Strings to hold the final sequence alignment strings
    // with gaps and alignments indicated.
    string sequence_1 = "";
    string sequence_2 = "";
    string alignments = "";

    string::reverse_iterator iterator_1 = sequences_vector[0].rbegin();
    string::reverse_iterator iterator_2 = sequences_vector[1].rbegin();

    for (vector<char>::reverse_iterator it = steps_vector.rbegin();
         it != steps_vector.rend()-1; ++it)
    {
        if (*it == 'U')
        {
            sequence_1 = "-" + sequence_1;
            sequence_2 = *iterator_2 + sequence_2;
            alignments = " " + alignments;

            it+=width-1;
            ++iterator_2;
        }
        else if (*it == 'D')
        {
            sequence_1 = *iterator_1 + sequence_1;
            sequence_2 = *iterator_2 + sequence_2;

            if (*iterator_1 == *iterator_2)
            {
                alignments = "|" + alignments;
            }
            else
            {
                alignments = " " + alignments;
            }

            it+=width;
            number_aligned+=1;
            ++iterator_1;
            ++iterator_2;
        }
        else if (*it == 'L')
        {
            // Increment normally.
            sequence_2 = "-" + sequence_2;
            sequence_1 = *iterator_1 + sequence_1;
            alignments = " " + alignments;

            ++iterator_1;
        }
    }

    sequences_vector.clear();
    sequences_vector.push_back(sequence_1);
    sequences_vector.push_back(alignments);
    sequences_vector.push_back(sequence_2);

}   // End PWA_alignment::trace_back_steps().


/*=======================================================================*/
/* Method: PWA_alignment::compute_alignment_score()                      */
/*-----------------------------------------------------------------------*/
/* Computes final alignment score with the following scoring method:     */
/*     Match (no scoring matrix specified): +1                           */
/*     Mismatch (no scoring matrix specified): -1                        */
/*     Match = Mismatch (score dependent on scoring matrix specified)    */
/*     Gaps: -2                                                          */
/*=======================================================================*/
void PWA_alignment::compute_alignment_score(void)
{
    string::iterator iterator_2 = sequences_vector[2].begin();

    for (string::iterator iterator_1 = sequences_vector[0].begin();
         iterator_1 != sequences_vector[0].end();
         ++iterator_1)
    {
        if ((*iterator_1 == '-') || (*iterator_2 == '-'))
        {
            alignment_score+=gap_penalty;
        }
        else if ((scoring_specified == 1))
        {
             string amino_pair = "";
             amino_pair.push_back(*iterator_1);
             amino_pair.push_back(*iterator_2);
             transform(amino_pair.begin(), amino_pair.end(), \
                       amino_pair.begin(), ::toupper);

             alignment_score+=scoring_map[amino_pair];
            
        }
        else if (*iterator_1 == *iterator_2)
        {
            alignment_score+=1;
        }
        else if (*iterator_1 != *iterator_2)
        {
            alignment_score-=1;
        }
        ++iterator_2;
    }

}   // End PWA_alignment::compute_alignment_score().

