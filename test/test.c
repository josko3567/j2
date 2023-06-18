#define VIWERR_SUBSCRIPTION_ERRNO
#include "../j2.h"
#include "../../viwerr/viwerr.h"
// #include "lib/utf8.h"
#include <errno.h>
#include <stdio.h>

#define utf8_is_char(v)\
        ((v>>6) == 2)

int main() {

        j2string stringy = j2.string.new.st("ššššš");
        int a = j2.string.noutf8.ccpy(&stringy, "šššš");
        j2.string.noutf8.ccat(&stringy, "🍌👍");

        

        printf("[%d]%s\n", J2_STRING_INFO(stringy)->length, stringy);

        j2string strog = j2.string.init("Šumi đumi!", (j2string_param){
                .allocate = 22,
                .max = J2_STRING_MAX_SIZE,
                .padding = 100
        });
        j2.string.ccat(&strog, " Cumi joši!");
        j2string str1 = j2.string.new.st("🏅🌖🏆🌖🏅");
        struct j2string_st * info  = J2_STRING_INFO(str1);
        printf("[%s], length: %d, filled: %d\n", str1, info->length, info->filled);
        info = J2_STRING_INFO(strog);
        printf("[%s] alloc: %lu, filled: %lu, length: %d, padding: %lu\n", 
                strog, info->allocated, info->filled, info->length, info->padding);

        if(viwerr(VIWERR_OCCURED, NULL) != NULL){
                
                fprintf(stdout, "Error occured!\n");
                for( viwerr_package * package = viwerr(VIWERR_PRINT, NULL); 
                        package != NULL;package = viwerr(VIWERR_PRINT, NULL));
                
        }
        return 0;

}