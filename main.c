#include <SDL2/SDL.h>
#include "SDL2/SDL_mixer.h"
#include <ncurses.h>
#include <dirent.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char random_file[255];
WINDOW *window;
Mix_Music *music;
int NUM_OF_FILES;

void init_sdl() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("Failed to init SDL\n");
        exit(1);
    }
    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
}

void play(char *file){
    music = Mix_LoadMUS(file);
    Mix_PlayMusic(music, 1);
}

WINDOW *init_window(){
    WINDOW *window = initscr();
    curs_set(0);
    cbreak();
    noecho();
    return window;
}

void refresh_screen(){
    erase();
    box(window, ACS_VLINE, ACS_HLINE);
    move(0,2);
    addstr("mini-mp3-player");
    move(window->_maxy, window->_maxx - 33);
    addstr("Press n to jump to the next song");
}

void display_title(char *title){
    move(window->_maxy / 2, (window->_maxx / 2) - (strlen(title) / 2));
    addstr(title);
}

void clear_conf_file(){
    FILE *file;
    file = fopen("/usr/local/etc/mini-mp3-player/mini-mp3-player.conf", "w");
    fclose(file);
}

void append_to_conf_file(char *line){
    FILE *file;
    file = fopen("/usr/local/etc/mini-mp3-player/mini-mp3-player.conf", "a");
    fprintf(file, "%s\n", line);
    fclose(file);
}

// readdir also reads ".." and "." as directories, if this was not filtered, the recursion would be infinite
int is_valid_dir(char *dir_name){
    return strcmp(dir_name,"..") != 0 && strcmp(dir_name,".") != 0;
}

char *get_file_extension(char *path){
    char *ext = strrchr(path, '.');
    if (!ext) {
        return "";
    } else {
        return ext + 1;
    }
}


// Recursive file reading from folder
void list_dir(char *path) {

    DIR *d;
    struct dirent *dir;

    d = opendir(path);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            char *join = "/";
            char *buff = malloc(strlen(path) + strlen(join) + strlen(dir->d_name) + 1);
            strcpy(buff,path);
            strcat(buff,join);
            strcat(buff,dir->d_name);
            if (dir->d_type == DT_DIR && is_valid_dir(dir->d_name)){
                list_dir(buff);
            }else{
                if(is_valid_dir(dir->d_name) && strcmp(get_file_extension(buff),"mp3") == 0)
                    append_to_conf_file(buff);
            }
            free(buff);
        }
        closedir(d);
    }
    return;
}

void read_file_line(char *line,char *path,int num){
    FILE *file;
    file = fopen(path, "r");
    int i = 0;
    while (fgets(line,255, file) && i <= num){
        i++;
    }
    fclose(file);
}

void get_random_file( char *buff){
    read_file_line(buff,"/usr/local/etc/mini-mp3-player/mini-mp3-player.conf",rand() % NUM_OF_FILES);
    //Remove new line and white space in the end of the file names
    buff[strlen(buff)-1] = '\0';
}

void seed_randomness(){
    srand(time(NULL));
}

void play_next(){
    Mix_FreeMusic(music);
    refresh_screen();
    get_random_file(random_file);
    play(random_file);
    display_title(random_file);
}

char *get_argument(char *argv[],int argc, char *flag){
    for(int i = 0; i < argc; i++){
        if(strcmp(argv[i],flag) == 0){
            return argv[i+1];
        }
    }
    return NULL;
}

int count_lines_in_file(char *path){
    FILE *file = fopen(path,"r");
    int lines = 0;
    while(!feof(file)){
        char ch = fgetc(file);
        if(ch == '\n'){
            lines++;
        }
    }
    return lines;
}

void check_conf_file(){
    if(!access("/usr/local/etc/mini-mp3-player/mini-mp3-player.conf", F_OK) == 0) {
        fprintf( stderr, "ERROR: Collection file does not exist in /usr/local/etc/mini-mp3-player. Run mini-mp3-player with flag -c and the collection path to generate the file\n");
        exit(0);
    }
}

int main(int argc, char *argv[]) {

    if(get_argument(argv,argc,"-c") != NULL){
        clear_conf_file();
        list_dir(get_argument(argv,argc,"-c"));
    }

    check_conf_file();
    NUM_OF_FILES = count_lines_in_file("/usr/local/etc/mini-mp3-player/mini-mp3-player.conf");

    init_sdl();
    window = init_window();
    seed_randomness();
    get_random_file(random_file);

    play(random_file);
    refresh_screen();
    display_title(random_file);

    while (!SDL_QuitRequested()) {
        SDL_Delay(250);
        char c = getch();
        timeout(1000);
        if(c == 'q' || c == 'Q'){
            break;
        }
        if(c == 'n' || c == 'N'){
            play_next();
        }
        if(Mix_PlayingMusic() == 0){
            play_next();
        }
    }

    Mix_FreeMusic(music);
    SDL_Quit();
    endwin();
    return 0;
}
