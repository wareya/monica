include nall/GNUmakefile

flags += -I. -O3

ifeq ($(platform),windows)
  link += -mconsole
endif

all: monica.exe

monica.o: monica.cpp
	$(compiler) $(cppflags) $(flags) -c monica.cpp -o monica.o

template.o: NLTemplate/NLTemplate.cpp
	$(compiler) $(cppflags) $(flags) -c NLTemplate/NLTemplate.cpp -o template.o

monica.exe: monica.o template.o
	$(compiler) $(flags) monica.o template.o -o monica.exe $(link)

