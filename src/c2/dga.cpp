#include "dga.h"

std::string dga(int key)
{
    int fullkey[4];
    char domain[16];
    static std::string chars = "abcdefghijklmnopqrstuvwxyz0123456789";

    time_t rawtime;
    struct tm* format_time;

    time(&rawtime);
    format_time = gmtime(&rawtime);

    fullkey[0] = format_time->tm_year;
    fullkey[1] = format_time->tm_mon;
    fullkey[2] = format_time->tm_mday;
    fullkey[3] = key;

    int tmpvar = fullkey[2] + fullkey[3];
    int pivot;
    char el;

    for (int ind = 0; ind < 16; ind++) {
        pivot = ind ^ fullkey[0] ^ fullkey[1] ^ tmpvar;
        el = chars[pivot % chars.length()];
        domain[ind] = el;
        tmpvar = pivot ^ tmpvar ^ (int)el;
    }
    return std::string(&domain[0], 16);

}
