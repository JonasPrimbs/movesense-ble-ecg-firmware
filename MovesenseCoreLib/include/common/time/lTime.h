#pragma once

//! A TimeHM class.
/******************************************************************************
        A structure for holding a time of day.
        This one has fields for hours and minutes.
        It is used by the dual clock time and alarms.
*/
struct TimeHM
{
    /**
            Hour is handled internally in 24 hour format. Range 0..23.
            Use utility functions for converting to 12h representation.
    */
    uint8 hour;

    /**
            Holds minutes in range 0..59.
    */
    uint8 minute;
};

//! A TimeHMS class.
/******************************************************************************
        A structure for holding a time of day.
        This one has fields for hours, minutes and seconds.
        Used for clock functions.
*/
struct TimeHMS
{
    /**
            Hours and minutes are handled with TimeHM struct.
    */
    TimeHM timeHM;

    /**
            Seconds. Range 0..59.
    */
    uint8 second;
};

//! A TimeHMST class.
/******************************************************************************
        A structure for holding a time of day.
        This one has fields for hours, minutes, seconds and tenths.
        It is needed for example for storing stopwatch times.
*/
struct TimeHMST
{
    /**
            Hours, minutes and seconds are handled with TimeHMS struct.
    */
    TimeHMS timeHMS;

    /**
            Tenths. Range 0..9.
    */
    uint8 tenth;
};

//! A TimeMS struct
/******************************************************************************
        A structure for holding a short time consisting of minutes and seconds
        This one has fields for minutes and seconds
        It is needed for example for storing speed times like min/km
*/
struct TimeMS
{
    /**
            Minutes. Range 0..255
    */
    uint8 minute;

    /**
            Seconds. Range 0..59.
    */
    uint8 second;
};

/******************************************************************************
        A structure for holding a clock drift correction values.
        Used to make clock more accurate.
*/
struct ClockDriftCorrConfig
{
    /**
            Magnitude of adjustment. A unit is 1/8th of a second.
    */
    int8 speedAdjust;

    /**
            The parameter for defining the adjustment interval. For example
            if the interval is 5 the adjusment is active every fifth minute.
    */
    uint16 interval;
};
