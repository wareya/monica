threaded=true
include nall/GNUmakefile

flags += -I. -O3

all: monica

monica.o: monica.cpp
	$(compiler) $(cppflags) $(flags) -c monica.cpp -o monica.o

template.o: NLTemplate/NLTemplate.cpp
	$(compiler) $(cppflags) $(flags) -c NLTemplate/NLTemplate.cpp -o template.o

monica: monica.o template.o
	$(compiler) $(flags) monica.o template.o -o monica $(link)

