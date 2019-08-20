/*=======================================================================*/
/* Filename: PWA_option.cpp                                              */
/* Last updated: November 26, 2017                                       */
/*=======================================================================*/
/* Parses command line and stores options/filenames specified by the     */
/* user.                                                                 */
/*=======================================================================*/
#include "PWA_file.h"
#include "PWA_message.h"
#include "PWA_option.h"

#include <cstring>
#include <iostream>
#include <stdlib.h>

using namespace std;


/*=======================================================================*/
/* Constructor: PWA_option                                               */
/*-----------------------------------------------------------------------*/
/* Initializes chosen_option as 'x'. If, at the end of parsing the       */
/* command line, chosen_option is still 'x', the program knows that no   */
/* option has been chosen.                                               */
/*                                                                       */
/* Also initializes scoring_specified as FALSE to indicate that no       */
/* specific scoring matrix file has yet been specified.                  */
/*=======================================================================*/
PWA_option::PWA_option()
{
    chosen_option = 'x';
    scoring_specified = 0;

}   // End PWA_option::PWA_option().


/*=======================================================================*/
/* Method: PWA_option::parse_command_line()                              */
/*-----------------------------------------------------------------------*/
/* Parses command line and saves filenames specified by user.            */
/*=======================================================================*/
void PWA_option::parse_command_line(int argc, char *argv[],
                                     PWA_file    *file_obj,
                                     PWA_message *msg_obj)
{
    if (argc == 1)
    {
        msg_obj->print_no_option();
        exit(-1);
    }

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0)
        {
            msg_obj->print_help_message();
            exit(0);
        }
        else if (strcmp(argv[i], "-n") == 0)
        {
            if (check_if_option_chosen(msg_obj) == 0)
            {
                chosen_option = 'n';
                file_obj->input_filename = strdup(argv[i+1]);
            }
            i++;
        }
        else if (strcmp(argv[i], "-s") == 0)
        {
            scoring_specified = 1;
            file_obj->scoring_filename = strdup(argv[i+1]);
            i++;
        }
        else if (strcmp(argv[i], "-p") == 0)
        {
            if (check_if_option_chosen(msg_obj) == 0)
            {
                chosen_option = 'p';
                file_obj->input_filename = strdup(argv[i+1]);
            }
            i++;
        }
        else if (strcmp(argv[i], "-o") == 0)
        {
            file_obj->output_filename = strdup(argv[i+1]);
            i++;
        }
    }   // End for.

    // If -o option not selected, sets default output file name.
    if (file_obj->output_filename == '\0')
    {
        file_obj->output_filename = strdup("PWA_output.txt");
    }

}   // End PWA_option::parse_command_line().


/*=======================================================================*/
/* Method: PWA_option::check_if_option_chosen()                          */
/*-----------------------------------------------------------------------*/
/* Checks if user has already selected an option. If the user has not    */
/* yet selected an option, the method returns 0. If the user has         */
/* already selected an option, the method returns 1, and the extra       */
/* option is ignored.                                                    */
/*=======================================================================*/
bool PWA_option::check_if_option_chosen(PWA_message *msg_obj)
{
    if (chosen_option != 'x')
    {
        msg_obj->print_extra_option_detected();
        return (1);
    }

    return (0);

}   // End PWA_option::check_if_option_chosen().

