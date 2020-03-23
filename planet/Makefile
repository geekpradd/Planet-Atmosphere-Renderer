OPENGLLIB= -lGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/usr/local/lib 
CPPFLAGS=-I/usr/local/include

BIN1=bin/trial
SRC1=trial.cpp 
BIN2=bin/demo
SRC2=demo.cpp 
BIN3=bin/triangle
SRC3=triangle.cpp include/utility.cpp include/shader.cpp

all: $(BIN1) $(BIN2) $(BIN3)
$(BIN1): $(SRC1)
	g++ $(CPPFLAGS) $(SRC1) -o $(BIN1) $(LDFLAGS) $(LIBS)

$(BIN2): $(SRC2)
	g++ $(CPPFLAGS) $(SRC2) -o $(BIN2) $(LDFLAGS) $(LIBS)

$(BIN3): $(SRC3)
	g++ $(CPPFLAGS) $(SRC3) -o $(BIN3) $(LDFLAGS) $(LIBS)