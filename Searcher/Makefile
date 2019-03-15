.PHONY:clean

FLAGS=-std=c++11 -lboost_filesystem -lboost_system \
	-I /home/wang/lib/cppjieba/include -I /home/wang/lib/cppjieba/deps -ljsoncpp -fpermissive
test:test.cc searcher.cc searcher.hpp
	g++ test.cc searcher.cc searcher.hpp $(FLAGS)
clean:
	rm test
