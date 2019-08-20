#ifndef PWA_OPTION_H
#define PWA_OPTION_H

#include "PWA_file.h"
#include "PWA_message.h"

class PWA_option
{
public:
    PWA_option();

    void parse_command_line(int argc, char *argv[], \
                            PWA_file    *file_obj,  \
                            PWA_message *msg_obj);

    bool scoring_specified;
    char chosen_option; // n for nucleotide, p for protein,
                        // x for none chosen

private:
    bool check_if_option_chosen(PWA_message *msg_obj);

};  // PWA_option

#endif  // PWA_OPTION_H

