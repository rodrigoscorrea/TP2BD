all: upload findrec seek1 seek2

upload: upload.cpp
	g++ -o upload upload.cpp

findrec: findrec.cpp
	g++ -o findrec findrec.cpp

seek1: seek1.cpp
	g++ -o seek1 seek1.cpp

seek2: seek2.cpp
	g++ -o seek2 seek2.cpp

clean:
	rm -f upload findrec seek1 seek2