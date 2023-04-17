SRCS=$(wildcard src/*.c)
NAME=prassis

.PHONY: master sprt

all:
	gcc $(SRCS) -o $(NAME) -pthread -O2

master:
	rm -rf live/*.exe
	gcc $(SRCS) -o live/master -pthread -O2

sprt:
	cutechess-cli -engine cmd=prassis -engine cmd=live/master \
		-each proto=uci tc=10+0.1 -games 2 -rounds 99999 -repeat \
		-openings file=./8moves_v3.pgn order=random \
		-sprt elo0=0 elo1=10 alpha=0.05 beta=0.05 -concurrency 6 -ratinginterval 10