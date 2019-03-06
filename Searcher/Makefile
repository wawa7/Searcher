.PHONY:clean

FLAGS=-std=c++11 -lboost_filesystem -lboost_system \
	-I ../../cppjieba/include -I ../../cppjieba/deps -ljsoncpp
test:test.cc searcher.cc searcher.hpp
	g++ test.cc searcher.cc searcher.hpp $(FLAGS)
clean:
	rm test
