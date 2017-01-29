C       = g++
LIB     =  -I/usr/csshare/pkgs/csim_cpp-19.0/lib
CSIM    = /usr/csshare/pkgs/csim_cpp-19.0/lib/csim.cpp.a
RM      = /bin/rm -f
CFLAG   = -g -DCPP -DGPP
 
INPUT = new.csim.c
TARGET = new

all: $(INPUT)

$(TARGET): $(INPUT)
	$(C) $(CFLAG) $(LIB) $(INPUT) $(CSIM) -lm -o $(TARGET)

clean:
	$(RM) *.o *.output core *~ *# $(TARGET) $(TARGET2)

