#!/bin/sh -ex

V_SDL2=2.0.12
V_SDL2_image=2.0.5
V_SDL2_ttf=2.0.15
V_SDL2_mixer=2.0.4

pwd=$(pwd)

mkdir SDL2_libraries
cd SDL2_libraries

sudo apt-get install curl
sudo apt-get install net-tools
#sudo apt-get install pulseaudio
#sudo apt-get install libasound2-dev libpulse-dev

curl -OL https://www.libsdl.org/release/SDL2-${V_SDL2}.tar.gz
curl -OL https://www.libsdl.org/projects/SDL_image/release/SDL2_image-${V_SDL2_image}.tar.gz
curl -OL https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-${V_SDL2_ttf}.tar.gz
curl -OL https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-${V_SDL2_mixer}.tar.gz

tar -zxvf SDL2-${V_SDL2}.tar.gz
tar -zxvf SDL2_image-${V_SDL2_image}.tar.gz
tar -zxvf SDL2_ttf-${V_SDL2_ttf}.tar.gz
tar -zxvf SDL2_mixer-${V_SDL2_mixer}.tar.gz

sudo apt-get install cmake libfreetype6-dev libfontconfig1-dev xclip
sudo apt-get install xorg-dev

cd SDL2-${V_SDL2}
./configure --prefix=${pwd}/SDL2 --disable-video-mir
sudo make
sudo make install

export PATH=${pwd}/SDL2/bin:$PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${pwd}/SDL2/lib

cd ../SDL2_image-${V_SDL2_image}
./configure --prefix=${pwd}/SDL2
sudo make
sudo make install

cd ../SDL2_ttf-${V_SDL2_ttf}
./configure --prefix=${pwd}/SDL2
sudo make
sudo make install

cd ../SDL2_mixer-${V_SDL2_mixer}
./configure --prefix=${pwd}/SDL2
sudo make
sudo make install


export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${pwd}/SDL2/lib

echo "\e[31m\e[5mWarning ! \e[0mYou need to execute \e[93mpulseaudio -D\e[0m to get some sound"
