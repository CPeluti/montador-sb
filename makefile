NAME=montador

CPP_SOURCE=$(wildcard src/*.cpp)

H_SOURCE=${wildcard src/*.h}

OBJ=$(CPP_SOURCE:.cpp=.o)

CC=g++

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $@ $^

%.o: %.c %.h
	$(CC) -o $@ $< $(CC_FLAGS)

main.o: tradutor.cpp $(H_SOURCE)
	$(CC) -o $@ $< $(CC_FLAGS)

clean:
	rm -rf src/*.o $(NAME) *~
