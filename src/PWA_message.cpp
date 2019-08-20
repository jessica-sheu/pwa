/*=======================================================================*/
/* Filename: PWA_message.cpp                                             */
/*=======================================================================*/
/* Contains all message-printing methods for PWA.                        */
/*=======================================================================*/
#include "PWA_message.h"
#include "PWA_time.h"

#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;


/*=======================================================================*/
/* Constructor: PWA_message                                              */
/*-----------------------------------------------------------------------*/
/* Prints header for PWA.                                                */
/*=======================================================================*/
PWA_message::PWA_message()
{
    cout << endl;
    cout << "==================";
    cout << " PAIRWISE SEQUENCE ALIGNMENT ";
    cout << "==================";
    cout << endl << endl;

}   // End PWA_message::PWA_message().


/*=======================================================================*/
/* Method: PWA_message::print_help_message()                             */
/*-----------------------------------------------------------------------*/
/* Prints help message containing the program's usage and command        */
/* option information.                                                   */
/*=======================================================================*/
void PWA_message::print_help_message(void)
{

    cout << "Description:";
    cout << endl;
    cout << "    This program performs the Needleman-Wunsch algorithm on";
    cout << endl;
    cout << "    two nucleotide/protein sequences. If the input file";
    cout << endl;
    cout << "    specified contains more than two sequences, this program";
    cout << endl;
    cout << "    will only perform PWA on the first two.";
    cout << endl << endl;

    cout << "Usage:";
    cout << endl;
    cout <<  "   ./PWA [-h] [-n FILE] [-p FILE]";
    cout << " [-s FILE] [-o FILE]" << endl;
    cout << endl;

    cout << "Options:" << endl;
    cout << "    -h             : Shows this help message and exits.";
    cout << endl;

    cout << "    -n FILE        : Reads and performs nucleotide";
    cout << " pairwise" << endl;
    cout << "                     sequence alignment from sequences in";
    cout << " FILE." << endl;

    cout << "    -p FILE        : Reads and performs protein pairwise";
    cout << endl;
    cout << "                     sequence alignment from sequences in";
    cout << " FILE.";
    cout << endl;

    cout << "    -s FILE        : Aligns a pair of protein sequences";
    cout << " with" << endl;
    cout << "                     the scoring table in FILE. If this";
    cout << " option" << endl;
    cout << "                     is not selected, a simple +1/-1";
    cout << " scoring" << endl;
    cout << "                     for matches/mismatches will be";
    cout << " used." << endl;

    cout << "    -o FILE        : Writes output to FILE specified by";
    cout << " user." << endl;
    cout << "                     WARNING: If the output file chosen";
    cout << " already" << endl;
    cout << "                     exists, it will be overwritten.";
    cout << endl << endl;

    cout << "Examples to run PWA:" << endl;
    cout << "    ./PWA -h" << endl;
    cout << "    ./PWA -n DNA_sequences.txt -o my_alignment.txt";
    cout << endl;
    cout << "    ./PWA -p protein_sequences.txt -s BLOSUM.txt";
    cout << endl << endl;

    cout << "Default output saved to ./PWA_output.txt.";

    cout << endl << endl;
    cout << "-----------------------------------";
    cout << "-----------------------------";

    cout << endl << endl;

}   // End PWA_message::print_help_message().


/*=======================================================================*/
/* Method: PWA_message::print_extra_option_detected()                    */
/*-----------------------------------------------------------------------*/
/* User has specified more than one "-n" or "-p" option. Prints warning  */
/* message and only aligns first file with first option chosen.          */
/*=======================================================================*/
void PWA_message::print_extra_option_detected(void)
{
    cout << "WARNING: Extra option detected. PWA will continue with";
    cout << endl;
    cout << "         aligning only the first option and FILE specified.";
    cout << endl << endl;

}   // End PWA_message::print_extra_option_detected().

 
/*=======================================================================*/
/* Method: PWA_message::print_option_selected()                          */
/*-----------------------------------------------------------------------*/
/* Prints beginning message after option has been chosen. Indicates      */
/* which option has been chosen and informs user that the program is     */
/* now beginning to execute that option's sequence alignment.            */
/*=======================================================================*/
void PWA_message::print_option_selected(string option,
                                         char *input_filename)
{
    print_help_message();
    cout << option << " sequences selected!" << endl;

    transform(option.begin(), option.end(), option.begin(), ::tolower);

    cout << "Commencing " << option << " PWA of ";
    cout << input_filename << " ..." << endl << endl;
 
}   // End PWA_message::print_option_selected().


/*=======================================================================*/
/* Method: PWA_message::print_no_option()                                */
/*-----------------------------------------------------------------------*/
/* If no option is selected, prints this message.                        */
/*=======================================================================*/
void PWA_message::print_no_option(void)
{
    print_help_message();

    cout << "ERROR: No option to align nucleotide or protein";
    cout << endl;
    cout << "       sequences has been chosen. Please refer to";
    cout << endl;
    cout << "       the above help message for correct usage of PWA.";
    cout << endl << endl << endl;

    cout << "Thank you! Exiting program." << endl;
    cout << "=======================================================";
    cout << "=========";
    cout << endl << endl;

    exit(-1);

}   // End PWA_message::print_no_option().


/*=======================================================================*/
/* Method: PWA_message::end_PWA()                                        */
/*-----------------------------------------------------------------------*/
/* Now that all output has been completed, prints end message, program   */
/* starting time and date, program ending time and date, and total CPU   */
/* taken for pairwise sequence alignment to complete.                    */
/*=======================================================================*/
void PWA_message::end_PWA(PWA_time *time_obj, char *output_filename)
{
    cout << endl;
    cout << "Finished output to file " << output_filename;
    cout << "." << endl << endl;

    cout << "Start: " << time_obj->start_date;
    cout << "End:   " << time_obj->get_end_date();
    cout << "TOTAL CPU TIME = " << time_obj->get_CPU_time_string();
    cout << endl << endl;

    cout << "Thank you! Exiting program." << endl;
    cout << "=======================================================";
    cout << "=========";
    cout << endl << endl;

}   // End PWA_message::end_PWA().

