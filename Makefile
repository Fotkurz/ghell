GCC=$(shell which gcc)

all:
	$(GCC) -std=c99 -Wall -Werror -g \
		ghell.c \
		cmd.c \
		parser.c \
		pwd.c \
		-o ghell.out

clean:
	rm -rf ghell.out

# add -std=c99 -Wall -Werror

# -std=c99 will compile to c99 version
# -Wall will add warnings to the compiler that will help you fixing bugs prior to running
# -Werror threat warnings as errors