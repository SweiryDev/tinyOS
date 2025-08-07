#ifndef __DRIVER_RTC_H
#define __DRIVER_RTC_H

#include <types.h>

// Date and time struct
typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint32_t year;
} rtc_time_t;

// Read the current time
void rtc_read_time(rtc_time_t *time_struct);

// Convert time struct to formatted time string
char *time_string(rtc_time_t *time_struct);

// Convert time struct to formatted date string
char *date_string(rtc_time_t *time_struct);

#endif