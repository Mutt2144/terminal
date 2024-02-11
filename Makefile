# Definindo variáveis
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Lista de arquivos de origem
SRCS = src/main.cpp

# Lista de arquivos de objeto

OBJS = $(SRCS:.cpp=.o)


# Nome do executável
TARGET = terminal

# Regras de compilação
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra para limpar os arquivos gerados
clean:
	echo $(OBJS)
	rm -f $(OBJS) $(TARGET)
