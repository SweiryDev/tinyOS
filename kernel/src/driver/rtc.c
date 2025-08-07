#include <driver/rtc.h>
#include <cpu/ports.h>
#include <utils/utils.h>
#include <memory/heap.h>

// RTC I/O ports
#define RTC_ADDRESS_PORT 0x70
#define RTC_DATA_PORT    0x71


// Get a specific register from the RTC
static uint8_t get_rtc_register(uint8_t reg) {
    byte_out(RTC_ADDRESS_PORT, reg);
    return byte_in(RTC_DATA_PORT);
}

// Convert a BCD value to binary
static uint8_t bcd_to_bin(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

// Read from the RTC
void rtc_read_time(rtc_time_t *time_struct) {
    // Read registers from the RTC
    uint8_t second = get_rtc_register(0x00);
    uint8_t minute = get_rtc_register(0x02);
    uint8_t hour = get_rtc_register(0x04);
    uint8_t day = get_rtc_register(0x07);
    uint8_t month = get_rtc_register(0x08);
    uint32_t year = get_rtc_register(0x09); // last two digits

    // BCD Conversion
    // The RTC stores values in Binary-Coded Decimal (BCD)
    time_struct->second = bcd_to_bin(second);
    time_struct->minute = bcd_to_bin(minute);
    time_struct->hour = bcd_to_bin(hour);
    time_struct->day = bcd_to_bin(day);
    time_struct->month = bcd_to_bin(month);
    // Assuming the century is 21st for simplicity
    time_struct->year = 2000 + bcd_to_bin(year);
}

// convert 1 digit string to 2 digits ("1" -> "01")
static void format_two_digits(char *str){
    if(str[1] == '\0'){
        str[1] = str[0];
        str[0] = '0';
        str[2] = '\0';
    }
}

char *time_string(rtc_time_t *time_struct){
    char *hour = itoa(time_struct->hour, 10);
    char *minute = itoa(time_struct->minute, 10);
    char *second = itoa(time_struct->second, 10);

    format_two_digits(hour);
    format_two_digits(minute);
    format_two_digits(second);

    char *timestr = (char*)kmalloc(9);
    timestr[0] = hour[0];
    timestr[1] = hour[1];
    timestr[2] = ':';
    timestr[3] = minute[0];
    timestr[4] = minute[1];
    timestr[5] = ':';
    timestr[6] = second[0];
    timestr[7] = second[1];
    timestr[8] = '\0';

    kfree(hour);
    kfree(minute);
    kfree(second);

    return timestr;
}

char *date_string(rtc_time_t *time_struct){
    char *day = itoa(time_struct->day, 10);
    char *month = itoa(time_struct->month, 10);
    char *year = itoa(time_struct->year, 10);

    format_two_digits(day);
    format_two_digits(month);

    char *datestr = (char*)kmalloc(9);

    datestr[0] = day[0];
    datestr[1] = day[1];
    datestr[2] = '/';
    datestr[3] = month[0];
    datestr[4] = month[1];
    datestr[5] = '/';
    datestr[6] = year[2];
    datestr[7] = year[3];
    datestr[8] = '\0';

    kfree(day);
    kfree(month);
    kfree(year);

    return datestr;
}