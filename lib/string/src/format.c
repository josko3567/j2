#include "../string.h"
#ifdef __J2_DEV__
#include "../../../../viwerr/viwerr.h"
#endif
long
j2string_format(
        const_restrict_j2string * string,
        const char * __restrict format,
        ... )
{

#ifdef __J2_DEV__
        if(!string) {
                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code    = EINVAL,
                        .name    = (char*)"j2err: Invalid parameters",
                        .message = (char*)"j2string_format() / "
                                   "j2.string.format(): "
                                   "parameter \"string\" is a NULL pointer.",
                        .group   = (char*)"j2"
                });
                return 0;
        }
        if(!format) {
                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code    = EINVAL,
                        .name    = (char*)"j2err: Invalid parameters",
                        .message = (char*)"j2string_format() / "
                                   "j2.string.format(): "
                                   "parameter \"format\" is a NULL pointer.",
                        .group   = (char*)"j2"
                });
                return 0;
        }
        if(!(*string)) {
                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code    = EINVAL,
                        .name    = (char*)"j2err: Invalid parameters",
                        .message = (char*)"j2string_format() / "
                                   "j2.string.format(): "
                                   "parameter \"string\" points to a NULL "
                                   "pointer.",
                        .group   = (char*)"j2"
                });
                return 0;
        }
        if(J2_STRING_INFO((*string)) == NULL) {
                errno = EINVAL;
                viwerr(VIWERR_PUSH, &(viwerr_package){
                        .code    = EINVAL,
                        .name    = (char*)"j2err: Invalid parameters",
                        .message = (char*)"j2string_format() / "
                                   "j2.string.format(): "
                                   "parameter \"string\" does not contain "
                                   "a valid j2string... "
                                   "J2_STRING_INFO((*string)) returned NULL.",
                        .group   = (char*)"j2"
                });
                return 0;
        }
#endif

        struct j2string_st * info = J2_STRING_INFO((*string));

        for(size_t i = 0; format[i] != '\0'; i++) {

                char ch = format[i];
                if(ch == '%') {

                        if(format[i+1] == 'd') {
                                
                                

                        }

                }

        }

}