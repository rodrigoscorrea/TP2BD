all: upload findrec seek1 seek2

upload: upload.cpp
	g++ -std=c++11 -o upload upload.cpp -O2

findrec: findrec.cpp
	g++ -std=c++11  -o findrec findrec.cpp

seek1: seek1.cpp
	g++ -std=c++11 -o seek1 seek1.cpp

seek2: seek2.cpp
	g++ -std=c++11 -o seek2 seek2.cpp

clean:
	rm -f upload findrec seek1 seek2
