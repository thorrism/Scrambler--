PROGRAMS = obfuscate deobfuscate minify unminify
CC	= gcc-4.4
CXX	= g++-4.4
CXXFLAGS = -pthread -g

# ROSE installation directory
ROSE_DIR = /home/users/norris/soft/pat
DEFINES	= -DBOOST_REGEX_MATCH_EXTRA
INCLUDES = -I$(ROSE_DIR)/include
JVMLIB_DIR = /usr/lib/jvm/java-7-openjdk-amd64/jre/lib/amd64/server

LIBS = -Wl,-rpath,$(ROSE_DIR)/lib -L$(ROSE_DIR)/lib -lrose \
     -lboost_date_time -lboost_thread -lboost_filesystem -lboost_program_options \
     -lboost_regex  -lboost_system -lboost_wave -lpthread -lz  \
     -Wl,-rpath,$(JVMLIB_DIR) -L$(JVMLIB_DIR) -ljvm

# Default make rule to use
all: $(PROGRAMS)

%.o : %.C
	$(CXX) -c $(CXXFLAGS) $(DEFINES) $(INCLUDES) $<
	
$(PROGRAMS): $(PROGRAMS:=.o)
	$(CXX) -o $@.exe $@.o $(LIBS)

clean:
	$(RM) *.o $(PROGRAMS) *.ti

