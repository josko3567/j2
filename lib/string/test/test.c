// #define J2_DEBUG

#include "../../../viwerr.h"
#include "../string.h"
#include <stdio.h>
#include <math.h>

void
j2string_print_info(
        j2string string)
{

        if(J2_STRING_INFO(string) == NULL)
                return;
        
        struct j2string_st * info = J2_STRING_INFO(string);

        printf("__ j2string info: _______\n");
        printf("\t info->allocated : %d\n", info->allocated);
        printf("\t info->filled    : %d\n", info->filled);
        printf("\t info->max       : %d\n", info->max);
        printf("\t info->padding   : %d\n", info->padding);
        printf("\t info->ensure    : %x\n", info->ensure);
        printf("_________________________\n");
        return;

}

int main( void ) {

/*******************************************************************************
 * @brief 
 * Test J2_STRING_GET_NEW_SIZE
 */
        int conduct_j2_test_1 = 0;
        if( conduct_j2_test_1 == 1 ){
                for( int i = 0; i < 100; i++ ){

                        long oldsize = 10;
                        long newsize = 40;
                        long padding = 10;
                        long max = i;

                        printf(
                        "oldsize = %3d | newsize = %3d | "
                        "padding = %3d | max = %3d | -> CALCULATED: %d \n",

                        oldsize, newsize, padding, max,
                        J2_STRING_GET_NEW_SIZE(oldsize, newsize, padding, max)
                        );


                }
        }

/*******************************************************************************
 * @brief 
 * Test j2string
 */

        int conduct_j2_test_2 = 0;
        if( conduct_j2_test_2 == 1 ){
                for( int i = 0; i < 100; i+=4 ){

                        long allocate = 56;
                        long padding = i;
                        long max = 100;
                        j2string string = j2string_init("Hello world & Bye Bye!",
                                (j2string_param){
                                        .allocate = allocate,
                                        .max      = max,
                                        .padding  = padding,
                                }
                        );
                        printf("\n\n\n intalized string, contents are [%s]\n", string);
                        printf("\t Size calculated by strlen %d\n", strlen(string));
                        j2string_print_info(string);

                        j2string_free(string);


                }
        }

/*******************************************************************************
 * @brief 
 * Test j2string_cpy
 */

        int conduct_j2_test_3 = 0;
        if( conduct_j2_test_3 == 1 ){
                for( int i = 0; i < 100; i+=4 ){

                        long allocate = 1;
                        long padding = i;
                        long max = 100;
                        j2string string = j2string_init("Hello world & Bye Bye!",
                                (j2string_param){
                                        .allocate = allocate,
                                        .max      = max,
                                        .padding  = padding,
                                }
                        );
                        j2string insertion = j2string_init("Hello world!",
                                (j2string_param){
                                        .allocate = allocate,
                                        .max      = max,
                                        .padding  = padding,
                                }
                        );
                        printf("\n\n\n%ld", j2string_cpy(&string, insertion));
                        if(J2_STRING_ASSERT_FILLED(string)) printf("\nString is properly filled");
                        printf("\n intalized string, contents are [%s]\n", string);
                        printf("\t Size calculated by strlen %d\n", strlen(string));
                        j2string_print_info(string);

                        j2string_free(string);


                }
        }

/*******************************************************************************
 * @brief 
 * Test j2string_cmp
 */     
        int conduct_j2_test_4 = 1;
        if( conduct_j2_test_4 == 1 ){
                j2string str1 = j2string_init("Baga", (j2string_param){});
                j2string str2 = j2string_init("Woolly", (j2string_param){});
                j2string str3 = j2string_init("Caga", (j2string_param){});
                printf("Comparisons:\n");
                printf("%s and %s = %d\n", str1, str2, j2string_cmp(str1, str2));
                printf("%s and %s = %d\n", str2, str2, j2string_cmp(str2, str2));
                printf("%s and %s = %d\n", str3, str2, j2string_cmp(str3, str2));
                printf("%s and %s = %d\n", str2, str3, j2string_cmp(str2, str3));
        }


        return 0;

}
/*

        printf("Bits = %llu\n", J2_STRING_GET_MEMORY_SIZE(100));
        j2string string = j2string_init("1222222222222222222200", (j2string_param){
                .allocate = 15,
                .padding = 25,
                .max = 170,
        });
        printf("%s \n", string);
        struct j2string_st * strst = J2_STRING_REACH_FOR_STRUCT(string);
        if(strst != NULL){
                printf("%d\n", strst->allocated);
        }
        printf("%d\n", 
                J2_STRING_REACH_FOR_STRUCT(string)->filled
        );
        printf("%x\n", J2_STRING_ENSURE_HEXSPEAK);

        // malloc(10);
        printf("%p\n", string);
        j2string_cpy_c(&string, "Goodbkkkkkkkkkkkkk");
        printf("%p\n", string);

        printf("%d\n", j2string_strlen_sn(string,100));

        for(int i = 0; string[i] != '\0'; i++) 
                putchar(string[i]);
        putchar('\n');
        printf("%s\n", string);
        // errno = ENOMEM;

        // viwerr_package ** packages = _viwerr_list_init();

        if(viwerr(VIWERR_OCCURED, NULL) != NULL){
                
                fprintf(stdout, "Error occured!\n");
                for( viwerr_package * package = viwerr(VIWERR_PRINT, NULL); package != NULL;package = viwerr(VIWERR_PRINT, NULL));
                
        }

        J2_STRING_GET_NEW_SIZE(100, 200, 1, 201);



        j2string_free(string);
        return 0;*/