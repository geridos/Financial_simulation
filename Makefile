CXX = g++

SERV="bin/server"
CLI="bin/client"

ssrc = $(wildcard src/server/*.cpp)
csrc = $(wildcard src/client/*.cpp)

CXXFLAGS = -pthread -fPIC -fomit-frame-pointer -march=native -Wno-write-strings -ffloat-store -ffast-math -fno-math-errno
LIBS= -pthread -lboost_program_options 

all: server client

server:
	$(CXX) --std=c++11 $(ssrc) -o $(SERV) $(LIBS) $(CXXFLAGS)

client:
	$(CXX) --std=c++11 $(csrc) -o $(CLI) $(LIBS) $(CXXFLAGS)

clean:
	@rm -f $(SERVER_BIN) $(CLIENT_BIN)
