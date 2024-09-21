CFLAGS := -lglfw -lGL -lstdc++ -Wall -Wextra #-fsanitize=address,undefined

INCLUDE := ./include
SRC := ./src
BIN := ./bin

TARGET = $(BIN)/main

all: $(BIN) $(TARGET)

$(BIN):
	@if [ ! -d $(BIN) ]; then mkdir $(BIN); fi

$(TARGET): $(BIN)/glad.o $(BIN)/application.o $(BIN)/shader.o $(BIN)/quad.o $(BIN)/border_radius.o $(BIN)/dim.o ./main.cpp
	$(CXX) -I. -I./glad/include/glad $(BIN)/glad.o $(BIN)/application.o $(BIN)/shader.o $(BIN)/quad.o $(BIN)/border_radius.o $(BIN)/dim.o ./main.cpp -o $(TARGET) $(CFLAGS)

$(BIN)/application.o: $(INCLUDE)/application.hpp $(SRC)/application.cpp
	$(CXX) -I. -I$(INCLUDE) -c $(SRC)/application.cpp -o $(BIN)/application.o $(CFLAGS)

$(BIN)/shader.o: $(INCLUDE)/shader.hpp $(SRC)/shader.cpp
	$(CXX) -I. -I$(INCLUDE) -c $(SRC)/shader.cpp -o $(BIN)/shader.o $(CFLAGS)

$(BIN)/quad.o: $(INCLUDE)/quad.hpp $(SRC)/quad.cpp
	$(CXX) -I. -I$(INCLUDE) -c $(SRC)/quad.cpp -o $(BIN)/quad.o $(CFLAGS)

$(BIN)/border_radius.o: $(INCLUDE)/border_radius.hpp $(SRC)/border_radius.cpp
	$(CXX) -I. -I$(INCLUDE) -c $(SRC)/border_radius.cpp -o $(BIN)/border_radius.o $(CFLAGS)

$(BIN)/dim.o: $(INCLUDE)/dim.hpp $(SRC)/dim.cpp
	$(CXX) -I. -I$(INCLUDE) -c $(SRC)/dim.cpp -o $(BIN)/dim.o $(CFLAGS)

$(BIN)/glad.o: $(INCLUDE)/glad.h $(INCLUDE)/khrplatform.h $(SRC)/glad.c
	$(CXX) -I. -I$(INCLUDE) -c $(SRC)/glad.c -o $(BIN)/glad.o $(CFLAGS)

clean:
	@if [ -d $(BIN) ]; then rm -rf $(BIN); fi