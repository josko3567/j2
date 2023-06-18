#include "../utf8.h"

utf8_t utf8_init(
        uint8_t head )
{

        utf8_t symbol;
        symbol.type = is_utf8_head_w_type(head);
        symbol.payloads = 0;
        symbol.sequence = head;
        return symbol;

}

utf8_t utf8_cat(
        utf8_t symbol, 
        uint8_t byte ) 
{

        if(symbol.type != symbol.payloads) {
                
                symbol.sequence = (symbol.sequence << 8) | byte;
                symbol.payloads++;

        }

        return symbol;

}