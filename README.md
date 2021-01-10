```
  __ _   (_)  ___   (_) ____  __ _    ___   |_  / ____   ___   / / ___ _  __ __ ___   ____
 /  ' \ / /  / _ \ / / /___/ /  ' \  / _ \ _/_ < /___/  / _ \ / / / _ `/ / // // -_) / __/
/_/_/_//_/  /_//_//_/       /_/_/_/ / .__//____/       / .__//_/  \_,_/  \_, / \__/ /_/   
                                   /_/                /_/               /___/             

```
## What is mini-mp3-player?
Mini-mp3-player is a minimal, distracionless terminal mp3 player.

## How do you use it?
First thing you have to do is compiling the source code. Mini-mp3-player is coded in C and uses [SDL2](https://www.libsdl.org/download-2.0.php) and [Ncurses](https://invisible-mirror.net/archives/ncurses/) libreries.
 
The code and compilation has only been tested in a Ubuntu system, the followed step are:
```
apt-get install libsdl2-dev libsdl2-mixer-2.0-0 libncurses5-dev libncursesw5-dev && make install
```

## Usage
Once the code has been compiled run:
```
sudo mini-mp3-player -c {path-to-music-collection}
```
This command will create or refresh a collection file used by mini-mp3-player.
If you already have a collection, only run:
```
sudo mini-mp3-player
```

Skip to next song pressing -n- or quit pressing -q-
