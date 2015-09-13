include ../GNUmakefile

flags := $(flags) -pthread -I../..
#flags := $(flags) -g
flags := $(flags) -O3
link := $(link) -pthread

ifeq ($(platform),windows)
  link += -lodbc32
else
  link += -lsqlite3
endif

all:
	$(compiler) $(cppflags) $(flags) -o test.o -c test.cpp
	$(compiler) $(cppflags) $(flags) -o object.o -c object.cpp
	$(compiler) -o test test.o object.o $(link)
	-@$(call delete,*.o)

clean:
	-@$(call delete,test)
