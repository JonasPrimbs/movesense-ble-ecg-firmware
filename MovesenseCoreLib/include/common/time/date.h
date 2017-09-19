#pragma once
#include <stdint.h>

/**
   A structure for holding a date information.
*/
struct Date
{
    /**
       Year of a date. E.g. 2003.
    */
    uint16_t year;

    /**
       Month of a date.
       Range 1..12, January corresponding to 1.
    */
    uint8_t month;

    /**
       Day of month.
    */
    uint8_t day;
};

#define MONDAY 0    //!< A literal value for monday
#define TUESDAY 1   //!< A literal value for tuesday
#define WEDNESDAY 2 //!< A literal value for wednesday
#define THURSDAY 3  //!< A literal value for thursday
#define FRIDAY 4    //!< A literal value for friday
#define SATURDAY 5  //!< A literal value for saturday
#define SUNDAY 6    //!< A literal value for sunday
