install: *.c
	mkdir -m 777 -p /usr/local/etc/mini-mp3-player
	rm -f ./usr/local/bin/mini-mp3-player
	sudo  gcc *.c -lSDL2 -lSDL2_mixer -lmpg123 -lncurses -o /usr/local/bin/mini-mp3-player
debug: *.c
	rm -f /usr/local/bin/mini-mp3-player
	sudo  gcc *.c -lSDL2 -lSDL2_mixer -lmpg123 -lncurses -o /usr/local/bin/mini-mp3-player --debug
