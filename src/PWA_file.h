#ifndef PWA_FILE_H
#define PWA_FILE_H

#include "PWA_alignment.h"

#include <fstream>
#include <map>
#include <string>

using namespace std;

class PWA_file
{
public:
    PWA_file();
    void check_file_status(fstream &file, char *filename);
    void get_scoring_map(PWA_alignment *protein_obj);
    void get_contents_from_file(PWA_alignment *PWA_obj);
    void print_output_to_file(PWA_alignment *PWA_obj);
    char *remove_hidden_end_characters(char *file_name);

    char *input_filename;
    char *output_filename;
	char *scoring_filename;

private:
    static const int line_length = 50;

};  // PWA_file

#endif  // PWA_FILE_H

