SRCS=$(wildcard src/*.c)
NAME=prassis

.PHONY sprt

all:
	gcc $(SRCS) -o $(NAME) -pthread -O2

clean: live
	rm -rf

sprt:
	cutechess-cli -engine cmd=prassis option.Threads=1 -engine conf=Vice option.Threads=1 \
	-each proto=uci tc=inf/10+0.1 -games 2 -rounds 2500 -repeat 2 -maxmoves 200 \
	-sprt elo0=0 elo1=10 alpha=0.05 beta=0.05 -concurrency 6 -ratinginterval 10