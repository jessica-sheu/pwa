/*=======================================================================*/
/* Filename: PWA_main.cpp                                                */
/* Last updated: November 27, 2017                                       */
/*=======================================================================*/
/* Creates objects and calls methods to perform pairwise sequence        */
/* alignment based on the user's command-line option selections.         */
/*=======================================================================*/
#include "PWA_alignment.h"
#include "PWA_file.h"
#include "PWA_message.h"
#include "PWA_option.h"
#include "PWA_time.h"

#include <iostream>
#include <stdlib.h>

using namespace std;


/*=======================================================================*/
/* Function: main()                                                      */
/*-----------------------------------------------------------------------*/
/* Calls methods based on the user's selected options.                   */
/*=======================================================================*/
int main(int argc, char *argv[])
{
    PWA_file    *file_obj   = new PWA_file();
    PWA_message *msg_obj    = new PWA_message();
    PWA_option  *option_obj = new PWA_option();
    PWA_time    *time_obj   = new PWA_time();

    option_obj->parse_command_line(argc, argv, file_obj, msg_obj);

    if (option_obj->chosen_option == 'n')
    {
        msg_obj->print_option_selected("Nucleotide",
                                       file_obj->input_filename);

        PWA_alignment *nucleotide_obj = new PWA_alignment();

        // Note: 'scoring_specified' is 0 because no scoring matrix
        // for nucleotide PWA in this project.

        file_obj->get_contents_from_file(nucleotide_obj);
        nucleotide_obj->begin_PWA_alignment(msg_obj);
        file_obj->print_output_to_file(nucleotide_obj);
    }
    else if (option_obj->chosen_option == 'p')
    {
        msg_obj->print_option_selected("Protein",
                                       file_obj->input_filename);

        PWA_alignment *protein_obj = new PWA_alignment();

        if (option_obj->scoring_specified == 1)
        {
            file_obj->get_scoring_map(protein_obj);
        }

        file_obj->get_contents_from_file(protein_obj);
        protein_obj->begin_PWA_alignment(msg_obj);
        file_obj->print_output_to_file(protein_obj);
    }
    else
    {
        // No option selected.
        msg_obj->print_no_option();
    }

    msg_obj->end_PWA(time_obj, file_obj->output_filename);

    return (1);

}   // End main().

