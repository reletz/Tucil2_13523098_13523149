# Compile di Linux!

CXX = g++
MINGW =  x86_64-w64-mingw32
CXXFLAGS = -std=c++17
LDFLAGS = -lfreeimageplus -lfreeimage 
LWINDOWS = -I./src/header -L./src/lib
SRC = ./src/*.cpp
BIN = ./bin

WINDOWS = winver
LINUX = linuxver

install:
	sudo apt-get install libfreeimage3 libfreeimageplus3 libfreeimage-dev mingw-w64

windows:
	mkdir -p $(BIN)	
	$(MINGW)-$(CXX) $(CXXFLAGS) -o $(BIN)/$(WINDOWS).exe $(SRC) $(LWINDOWS) $(LDFLAGS)

linux: 
	mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) -o $(BIN)/$(LINUX) $(SRC) $(LDFLAGS)

build: windows linux

clean:
	rm -rf $(BIN)/*.exe
	rm -rf $(BIN)/$(LINUX)

all: install build