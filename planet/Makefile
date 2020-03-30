OPENGLLIB= -lGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/usr/local/lib 
CPPFLAGS=-I/usr/local/include

BIN1=bin/cube
SRC1=cube.cpp  include/utility.cpp include/shader.cpp
BIN2=bin/demo
SRC2=demo.cpp 
BIN3=bin/triangle
SRC3=triangle.cpp include/utility.cpp include/shader.cpp
BIN4=bin/camera_cube
SRC4=camera_cube.cpp include/utility.cpp include/shader.cpp
BIN5=bin/lighting
SRC5=lighting.cpp include/utility.cpp include/shader.cpp


all: $(BIN2) $(BIN3) $(BIN1) $(BIN4) $(BIN5)

$(BIN1): $(SRC1)
	g++ $(CPPFLAGS) $(SRC1) -o $(BIN1) $(LDFLAGS) $(LIBS)
$(BIN2): $(SRC2)
	g++ $(CPPFLAGS) $(SRC2) -o $(BIN2) $(LDFLAGS) $(LIBS)
$(BIN3): $(SRC3)	
	g++ $(CPPFLAGS) $(SRC3) -o $(BIN3) $(LDFLAGS) $(LIBS)
$(BIN4): $(SRC4)	
	g++ $(CPPFLAGS) $(SRC4) -o $(BIN4) $(LDFLAGS) $(LIBS)
$(BIN5): $(SRC5)	
	g++ $(CPPFLAGS) $(SRC5) -o $(BIN5) $(LDFLAGS) $(LIBS)




