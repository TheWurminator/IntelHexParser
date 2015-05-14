all: 
	gcc HexParser.c -o hexparse -lm

clean:
	rm hexparse
