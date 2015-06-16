# Dependencies #

In order to have sound in dfish SDL\_mixer needs to be installed. Go to http://www.libsdl.org/projects/SDL_mixer/ and download:

```
# 32-bit
SDL_mixer-1.2.11-1.i586.rpm
SDL_mixer-devel-1.2.11-1.i586.rpm
```

or

```
# 64-bit
SDL_mixer-1.2.11-1.x86_64.rpm
SDL_mixer-devel-1.2.11-1.x86_64.rpm
```

If you need a debian package just use the alien command:

```
sudo alien -d SDL_mixer-1.2.11-1.x86_64.rpm 
sudo alien -d SDL_mixer-devel-1.2.11-1.x86_64.rpm
```

You may need to first remove the old version of libsdl-mixer that might be installed. Once you have the package in the correct format just use your package manager and install!

# Compiling dfish #

To compile dfish run:

```
make clean
make SND=1
```

Enjoy the music :)

# Sound Commands #

There are three commands which control sound in dfish

```
/snd-volume VOLUME (percent) - Set music volume
/snd-stop - Stop music
/snd-play music filename (no spaces) - Play specific music file
```

You can play any music you would like simply by providing the path to the music file, for example:

```
/snd-play /usr/home/alice/favoritesong.mp3
```

By default the music will play in a loop forever.