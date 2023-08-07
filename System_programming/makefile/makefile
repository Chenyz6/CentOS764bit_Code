src = $(wildcard *.c)
targets = $(patsubst %.c,%, $(src))

CFLAGS = Wall -g

ALL:$(targets)

$(targets):%:%.c
	gcc $< -o $@ $(CFLAGS)

.PHONY ALL clean

clean:
	-rm -rf $(targets)