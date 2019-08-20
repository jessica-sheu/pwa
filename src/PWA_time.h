#ifndef PWA_TIME_H
#define PWA_TIME_H

#include <ctime>

using namespace std;

class PWA_time
{
public:
    PWA_time();

    long  get_CPU_time(void);
    char *get_CPU_time_string(void);

    char *get_end_date(void);
    char *start_date;

private:
    long begin_time;

};  // PWA_time

#endif  // PWA_TIME_H
