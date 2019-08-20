#ifndef PWA_MESSAGE_H
#define PWA_MESSAGE_H

#include "PWA_time.h"

#include <fstream>
#include <string>

using namespace std;

class PWA_message
{
public:
    PWA_message();

    void print_help_message();
    void print_extra_option_detected(void);
    void print_option_selected(string option, \
                               char *input_filename);
    void print_no_option(void);
    void end_PWA(PWA_time *time_obj, char *output_filename);

};  // PWA_message

#endif  // PWA_MESSAGE_H

