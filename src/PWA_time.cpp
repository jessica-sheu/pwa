/*=======================================================================*/
/* Filename: PWA_time                                                    */
/* Last updated: November 26, 2017                                       */
/*=======================================================================*/
/* Uses beginning time and ending time of the PWA program to find the    */
/* total duration.                                                       */
/*=======================================================================*/
#include "PWA_time.h"

#include <ctime>
#include <stdio.h>

/*=======================================================================*/
/* Constructor: PWA_time                                                 */
/*-----------------------------------------------------------------------*/
/* Initializes start time of PWA program.                                */
/*=======================================================================*/
PWA_time::PWA_time()
{
    begin_time = get_CPU_time();
    time_t start = time(0);

    start_date = ctime(&start); 

}   // End PWA_time::PWA_time().


/*=======================================================================*/
/* Method: PWA_time::get_CPU_time()                                      */
/*-----------------------------------------------------------------------*/
/* Gets CPU time in long format.                                         */
/*=======================================================================*/
long PWA_time::get_CPU_time(void)
{
    long CPU_time;
    clock_t cpu_clock;

    cpu_clock = clock();
    CPU_time = (long) (cpu_clock / CLOCKS_PER_SEC);

    return (CPU_time);

}  // End PWA_time::get_CPU_time()


/*=======================================================================*/
/* Method: PWA_time::get_CPU_time_string()                               */
/*-----------------------------------------------------------------------*/
/* Stores CPU time in a char buffer to be printed in the format:         */
/* HH:MM:SS (example: 00:00:01).                                         */
/*=======================================================================*/
char* PWA_time::get_CPU_time_string(void)
{
    long        CPU_time;
    long        hours, minutes, seconds;
    static char buff[24];

    buff[0] = '\0';

    CPU_time = get_CPU_time() - begin_time;
 
    hours   =  CPU_time / 3600;
    minutes = (CPU_time - ((long)hours * 3600)) / 60;
    seconds =  CPU_time - ((long)hours * 3600) - \
                          ((long)minutes * 60);

    sprintf(buff, "%ld:%.2ld:%.2ld", hours, minutes, seconds);

    return (buff);

}  // End PWA_time::get_CPU_time_string().


/*=======================================================================*/
/* Method: PWA_time::get_end_date()                                      */
/*-----------------------------------------------------------------------*/
/* Returns ending date and time.                                         */
/*=======================================================================*/
char* PWA_time::get_end_date(void)
{
    time_t end_date = time(0);

    return (ctime(&end_date));

}   // End PWA_time::get_end_date().

