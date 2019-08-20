/*=======================================================================*/
/* Filename: PWA_file.cpp                                                */
/* Last updated: November 26, 2017                                       */
/*=======================================================================*/
/* Handles all file operations and stores all filenames used in PWA.     */
/*=======================================================================*/
#include "PWA_alignment.h"
#include "PWA_file.h"

#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

// Uncomment to display debugging print statements.
//#define USEDEBUG_FILE
#ifdef USEDEBUG_FILE
#define debug(x) cout << x
#else
#define debug(x)
#endif


/*=======================================================================*/
/* Constructor: PWA_file                                                 */
/*-----------------------------------------------------------------------*/
/* Initializes all filenames to NULL.                                    */
/*=======================================================================*/
PWA_file::PWA_file()
{
    input_filename   = '\0';
    output_filename  = '\0';
    scoring_filename = '\0';

}   // End PWA_file::PWA_file().


/*=======================================================================*/
/* Method: PWA_file::check_file_status()                                 */
/*-----------------------------------------------------------------------*/
/* Checks whether the file was able to be opened. If not, prints an      */
/* error message.                                                        */
/*=======================================================================*/
void PWA_file::check_file_status(fstream &file, char *filename)
{
    if (file.fail())
    {
        cout << "Failed to access/read file ";
        cout << "'" << filename << "'." << endl;
        cout << "Exiting ...";
        cout << endl << endl;
        exit(-1);
    }

}   // End PWA_file::check_file_status().


/*=======================================================================*/
/* Method: PWA_file::get_scoring_map()                                   */
/*-----------------------------------------------------------------------*/
/* If scoring file is specified with the command -s for a pair of        */
/* proteins, the scoring_map is created with the amino acid pairs        */
/* as the key, and the integer score as the value. For example, if the   */
/* scoring file contains BLOSUM62 scores, scoring_map["FY"] = 3.         */
/*                                                                       */
/* Additionally contains debugging print statements to print the entire  */
/* contents of scoring_map.                                              */
/*=======================================================================*/
void PWA_file::get_scoring_map(PWA_alignment *protein_obj)
{
    string line = "";

    scoring_filename = remove_hidden_end_characters(scoring_filename);
    ifstream scoring_file(scoring_filename);

    while (getline(scoring_file, line))
    {
        if (line.find_first_not_of(" ") != string::npos)
        {
            istringstream ss(line);
            string pairs = "";
            int score    = 0;
            ss >> pairs >> score;

            protein_obj->scoring_map[pairs] = score;
        }
    }

#ifdef USEDEBUG_FILE
    // Print contents of scoring_map.
    for (map<string, int>::const_iterator it = \
         protein_obj->scoring_map.begin();     \
         it != protein_obj->scoring_map.end(); ++it)
    {
        debug(it->first << " " << it->second << endl);
    }
    debug(endl << endl);
#endif

    scoring_file.close();
    remove(scoring_filename);

    protein_obj->scoring_specified = 1;

}   // End PWA_file::get_scoring_map().


/*=======================================================================*/
/* Method: PWA_file::get_contents_from_file()                            */
/*-----------------------------------------------------------------------*/
/* Gets sequences and sequence names from the input file and saves them  */
/* into sequences_vector and names_vector, respectively. The contents    */
/* of names_vector will be included in the final output.                 */
/*                                                                       */
/* If the input file contains more than two sequences, only the first    */
/* two sequences are aligned.                                            */
/*                                                                       */
/* Additionally contains debugging print statements of the contents of   */
/* names_vector and sequences_vector after reading and saving from       */
/* input file.                                                           */
/*=======================================================================*/
void PWA_file::get_contents_from_file(PWA_alignment *PWA_obj)
{
    string contents = "";
    string line     = "";
    fstream input_file;

    input_filename = remove_hidden_end_characters(input_filename);

    input_file.open(input_filename, fstream::in);
    check_file_status(input_file, input_filename);

    while (getline(input_file, line))
    {
        if (line.find(">") != string::npos)
        {
            // Add name of sequence to include in final output.
            line.erase(line.find(">"), 1);
            PWA_obj->names_vector.push_back(line);

            if (contents != "")
            {
                PWA_obj->sequences_vector.push_back(contents);
                contents.clear();
            }
        }
        else
        {
            contents = contents + line;
        }
    }
    PWA_obj->sequences_vector.push_back(contents);

#ifdef USEDEBUG_FILE
    // Print contents of names_vector.
    for (size_t i = 0; i < PWA_obj->names_vector.size(); i++)
    {
        debug(PWA_obj->names_vector[i] << endl);
    }
    debug(endl);

    // Print contents of sequences_vector.
    for (size_t i = 0; i < PWA_obj->sequences_vector.size(); i++)
    {
        debug(PWA_obj->sequences_vector[i] << endl);
    }
    debug(endl << endl);
#endif

    input_file.close();
    remove(input_filename);

}   // End PWA_file::get_contents_from_file().


/*=======================================================================*/
/* Method: PWA_file::print_output_to_file()                              */
/*-----------------------------------------------------------------------*/
/* Prints final output to file.                                          */
/*                                                                       */
/* The header contains the names of the sequences taken from the input   */
/* file. For the sake of readability, the sequence alignment is printed  */
/* and split onto multiple lines if the sequences are longer than        */
/* line_length.                                                          */
/*                                                                       */
/* The output also includes the total number of nucleotide or amino      */
/* acid alignments, as well as the total alignment score.                */
/*=======================================================================*/
void PWA_file::print_output_to_file(PWA_alignment *PWA_obj)
{
    fstream output_file;
    int i = 0;

    output_file.open(output_filename, fstream::out | fstream::trunc);
    check_file_status(output_file, output_filename);

    // Print names.
    output_file << "Alignment results for:" << endl;
    output_file << "1. " << PWA_obj->names_vector[0] << endl;
    output_file << "2. " << PWA_obj->names_vector[1];
    output_file << endl << endl;

    // Print sequence alignment.
    for (vector<string>::iterator it = PWA_obj->sequences_vector.begin();
         it != PWA_obj->sequences_vector.end(); ++it)
    {
        if ((*it).length() > line_length)
        {
              if ((i + line_length) < (*it).length())
              {
                  output_file << (*it).substr(i, line_length) << endl;
                  if (it == PWA_obj->sequences_vector.end()-1)
                  {
                      output_file << endl;
                      it = PWA_obj->sequences_vector.begin()-1;
                      i +=line_length;
                  }
              }
              else
              {
                  output_file << (*it).substr(i, (*it).length() - \
                                              line_length);
                  output_file << endl;
              }
        }
        else
        {
            output_file << *it << endl;
        }
    }
    output_file << endl;

    // Print total number of alignments. 
    output_file << "Total number alignments: ";
    output_file << PWA_obj->number_aligned << endl;

    // Print total alignment score.
    // For readability, also prints additional space
    // if the score is non-negative.
    output_file << "Total alignment score:  ";
    if (PWA_obj->alignment_score >= 0)
    {
        output_file << " ";
    }
    output_file << PWA_obj->alignment_score << endl << endl;

    output_file.close();

}   // End PWA_file::print_output_to_file().


/*=======================================================================*/
/* Method: PWA_file::remove_hidden_end_characters()                      */
/*-----------------------------------------------------------------------*/
/* Removes \r\n (^M) characters from files imported from or              */
/* edited with Windows programs. These hidden characters can appear on   */
/* Unix-based machines and cause issues with reading the file. In the    */
/* case that write_input_file is of shorter length than read_input_file  */
/* (in which case, the remnants of read_input_file would still be at     */
/* the end of the new contents of write_input_file, the program first    */
/* writes to a temporary file. So to not overwrite the original file,    */
/* this new_filename is returned and used as the filename. Once the      */
/* temporary file is no longer needed, it is removed.                    */
/*=======================================================================*/
char* PWA_file::remove_hidden_end_characters(char *filename)
{
    fstream read_input_file;
    fstream write_input_file;

	char *new_filename = (char*)malloc(500); 

    string read_line  = ""; 
    string write_line = "";

    sprintf(new_filename, "tmp_file.txt");

    read_input_file.open(filename, fstream::in);
    check_file_status(read_input_file, filename);

    write_input_file.open(new_filename, fstream::out | fstream::trunc);
    check_file_status(write_input_file, new_filename);

	while (getline(read_input_file, read_line))
	{
		int len;

                // Preserve lines from input file.
		write_line = read_line;
		len = write_line.length();

		while (len > 0)
		{
			if ((!write_line.empty()
                             && write_line[len-1] == '\r'))
			{
				write_line.erase(len-1);
				len--;
			}
			else
			{
				break;
			}
		}
		// Print to file.
		write_input_file << write_line << endl;
	}

    read_input_file.close(); 
    write_input_file.close();

    return (new_filename);

}   // End PWA_file::remove_hidden_end_characters().

