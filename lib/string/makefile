STATIC=string.a

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic \
	 -Wformat=2 -Wno-unused-parameter -Wshadow \
	 -Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
	 -Wredundant-decls -Wnested-externs -Wmissing-include-dirs \
	 -pipe -std=c2x -O0

SRC = string.c

OBJ = $(SRC:.c=.o)

$(STATIC): $(OBJ)
	@echo "[Link (Static)]"
	@ar rcs $@ $^

.c.o:
	@echo [Compile] $<
	@$(CC) -lm -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJ) $(STATIC)

.PHONY: test
test: 
	@$(CC) -lm test/test.c -o test/a.out $(STATIC) 


