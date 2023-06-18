ifeq ($(OS),Windows_NT)     # is Windows_NT on XP, 2000, 7, Vista, 10...
	SHELL := powershell.exe
	.SHELLFLAGS := -Command
endif

STATIC=j2.a

# Debug mode includes viwerr.h and -D__J2_DEV__
DEBUG  = t
# Debug/release flags...
DFLAGS =
# Viwerr library home directory...
VIWERR =
ifeq ($(DEBUG), f) 
	VIWERR = 
	DFLAGS = -O3
else
	ifeq ($(OS),Windows_NT)     # is Windows_NT on XP, 2000, 7, Vista, 10...
		VIWERR = ext/viwerr/viwerr.lib
	else
		VIWERR = ext/viwerr/viwerr.a
	endif
	DFLAGS = -g -D__J2_DEV__
endif

EXECUTE_TEST =
ifeq ($(OS),Windows_NT)     # is Windows_NT on XP, 2000, 7, Vista, 10...
    EXECUTE_TEST = ./test/a.exe
    STATIC = j2.lib
    REMOVE = Get-ChildItem * -Include *.o, *.lib, *.exe -Recurse | Remove-Item
else
    EXECUTE_TEST = ./test/a.out
    STATIC = j2.a
    REMOVE = rm -f $(OBJ) $(STATIC)
endif

# Compiler
CC = gcc

# C warning flags...
CFLAGS = -Wall -Wextra -Wpedantic \
	 -Wformat=2 -Wno-unused-parameter -Wshadow \
	 -Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
	 -Wredundant-decls -Wnested-externs -Wmissing-include-dirs \
	 -pipe -std=c2x


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
	@$(REMOVE)


.PHONY: test
test: $(STATIC)
	@echo "[Running test/test.c]"
	@$(CC) -lm -g test/test.c -o $(EXECUTE_TEST) $(STATIC) $(VIWERR)
	@$(EXECUTE_TEST)

.PHONY: rebuild
rebuild: clean test