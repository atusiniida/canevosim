#CXX = /usr/local/package/gcc/current/bin/g++
#CXXFLAGS += -std=c++11	

CXX = clang++                                                                                             
CXXFLAGS +=  -stdlib=libc++   -ggdb                                                                            

#CXXFLAGS +=  -g -O0                                                                                       
#LDFLAGS +=                                                                                                
#LIBS +=  -lm                                                                                              



TARGET = bep

CXXSRCS = \
	bep.cpp\
	tumor.cpp\
	mutationcontainer.cpp\
	statuscontainer.cpp\
	random.cpp\

INST_HDRS = \


NOINST_HDRS = \
	tumor.h\
	mutationcontainer.h\
	statuscontainer.h\
	random.h\

SRCS = $(CXXSRCS)

.SUFFIXES: .o .cpp
CXXOBJS = $(CXXSRCS:.cpp=.o)
OBJS = $(CXXOBJS)

all: $(TARGET)

$(TARGET): $(OBJS) 
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -o $@ -c $<

install:
	mkdir -p $(BINDIR)
	cp $(TARGET) $(BINDIR)

clean:
	$(RM) $(OBJS) $(TARGET)

depend:
	$(CXX) -MM -MG $(SRCS) $(CXXFLAGS) > Makefile.depend

-include Makefile.depend
