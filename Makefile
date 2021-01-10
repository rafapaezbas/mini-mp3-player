install: *.c
	mkdir -p /etc/mini-mp3-player
	rm -f ./usr/bin/mini-mp3-player
	sudo  gcc *.c -lSDL2 -lSDL2_mixer -lmpg123 -lncurses -o /usr/bin/mini-mp3-player
debug: *.c
	rm -f /usr/bin/mini-mp3-player
	sudo  gcc *.c -lSDL2 -lSDL2_mixer -lmpg123 -lncurses -o /usr/bin/mini-mp3-player --debug
