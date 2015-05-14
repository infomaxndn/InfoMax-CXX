CC = g++
INC = -I. -Ilibs
FLAGS = -g -W -Wall -std=c++11

all: InfoMaxProducerTester InfoMaxConsumerTester parser randomTreeGenerator

InfoMaxProducerTester: TreeNode.o Prioritizers.o InfoMaxProducer.o InfoMaxProducerTester.cpp
	$(CC) $(FLAGS) $(INC) $^ -o $@ `pkg-config --cflags --libs libndn-cxx`

InfoMaxConsumerTester: TreeNode.o InfoMaxConsumer.o InfoMaxConsumerTester.cpp
	$(CC) $(FLAGS) $(INC) $^ -o $@ `pkg-config --cflags --libs libndn-cxx`

InfoMaxConsumer.o: InfoMaxConsumer.cpp InfoMaxConsumer.hpp InvalidPrefixException.hpp
	$(CC) -c $(FLAGS) $(INC) $< -o $@

InfoMaxProducer.o: InfoMaxProducer.cpp InfoMaxProducer.hpp InvalidPrefixException.hpp
	$(CC) -c $(FLAGS) $(INC) $< -o $@

randomTreeGenerator: TreeNode.o randomTreeGenerator.cpp
	$(CC) $(FLAGS) $(INC) $^ -o $@ `pkg-config --cflags --libs libndn-cxx`	

TreeNode.o: TreeNode.cpp TreeNode.hpp
	$(CC) -c $(FLAGS) $(INC) $< -o $@

Prioritizers.o: Prioritizers.cpp Prioritizers.hpp
	$(CC) -c $(FLAGS) $(INC) $< -o $@

parser.o: parser.cpp
	$(CC) -c $(FLAGS) $(INC) $< -o $@	

clean:
	$(RM) -r *.o InfoMaxProducerTester InfoMaxConsumerTester parser randomTreeGenerator