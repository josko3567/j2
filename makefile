STATIC=j2.a

# Debug mode includes viwerr.h and -D__J2_DEV__
DEBUG  = 
# Debug/release flags...
DFLAGS =
# Viwerr library home directory...
VIWERR =
ifeq ($(DEBUG), f) 
	VIWERR = 
	DFLAGS = -O3
else
	VIWERR =../viwerr/viwerr.a
	DFLAGS = -g -D__J2_DEV__
endif

# Compiler
CC = gcc

# C warning flags...
CFLAGS = -Wall -Wextra -Wpedantic \
	 -Wformat=2 -Wno-unused-parameter -Wshadow \
	 -Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
	 -Wredundant-decls -Wnested-externs -Wmissing-include-dirs \
	 -pipe -std=c99


LIBS = ./lib/string/src
SRC  = $(foreach D,$(LIBS),$(wildcard $(D)/*.c))   
OBJ  = $(patsubst %.c,%.o,$(SRC))   


$(STATIC): $(OBJ) $(VIWERR)
	@echo "[Link (Static)]"
	ar rcs $@ $(OBJ)


%.o: %.c $(VIWERR)
	@echo [Compile] $<
	@$(CC) -lm -c $(DFLAGS) $(CFLAGS) $< -o $@


$(VIWERR):
	@echo "---:VIWERR:---"
	make -C ext/viwerr

.PHONY: clean
clean:
	rm -f $(OBJ) $(STATIC)


.PHONY: test
test: $(STATIC)
	@echo "[Running test/test.c]"
	@$(CC) -lm -g test/test.c -o test/a.out $(STATIC) ../viwerr/viwerr.a && \
	./test/a.out

.PHONY: rebuild
rebuild: clean test