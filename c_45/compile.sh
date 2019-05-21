
rm main
g++ -std=c++11 -c *.cc -lpthread
g++ -std=c++11 *.o -o main -lpthread
rm *.o
