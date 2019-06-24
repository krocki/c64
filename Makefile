GCC=gcc
#GCC=arm-none-eabi

C_FLAGS=-Ofast -fPIC -I./6502/
OS:=$(shell uname)

ifeq ($(OS),Darwin) #OSX
  GL_FLAGS=-lglfw -framework OpenGL -lpthread
else # Linux or other
  GL_FLAGS=-lglfw -lGL -lpthread
endif

all: c64_nolcd

c64_nolcd: mmu.o 6502/6502.o 6502/6502.h Makefile c64_nolcd.o
	${GCC} mmu.o c64_nolcd.o 6502/6502.o ${C_FLAGS} ${GL_FLAGS} -o $@

c64_lcd: mmu.o 6502/6502.o 6502/6502.h Makefile c64_lcd.o
	${GCC} mmu.o c64_lcd.o 6502/6502.o ${C_FLAGS} ${GL_FLAGS} -o $@

%.o: %.c
	${GCC} ${C_FLAGS} -c $< -o $@

clean:
	rm -rf *.o c64_nolcd
