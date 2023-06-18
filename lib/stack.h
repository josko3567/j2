#define push(sp, n) (*((sp)++) = (n))
#define pop(sp) (*--(sp))

struct j2stack {

        int size;
        int filled;
        void ** array;

};

int j2stack_push( )