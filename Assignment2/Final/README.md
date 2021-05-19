Background Music: Kaine/Escape from NieR Replicant (Keiichi Okabe, Emi Evans).

Installation Instructions:
This code runs on Ubuntu (Linux). It uses C++.
Libraries used for this project are sys/socket.h, arpa/inet.h, unistd.h, SDL2/SDL.h, SDL2/SDL_image.h, SDL2/SDL_ttf.h, SDL2/SDL_mixer.h and bits/stdc++.h.
bits/stdc++.h, arpa/inet.h, unistd.h and sys/socket.h come with the standard g++ installation.
Command: sudo apt install build-essential
Command for SDL2: sudo apt-get install libsdl2-dev
Command for SDL2_image: sudo apt-get install libsdl2-image-dev
Command for SDL2_ttf: sudo apt-get install libsdl2-ttf-dev
Command for SDL2_mixer: sudo apt-get install libsdl2-mixer-dev

Resources:
1) https://lazyfoo.net/tutorials/SDL
2) https://www.geeksforgeeks.org/socket-programming-cc/
3) Wikipedia 
4) StackExchange
5) https://discourse.libsdl.org/c/sdl-development/6
6) itch.io (For Game Assets)
7) http://gaurav.munjal.us/Universal-LPC-Spritesheet-Character-Generator/#

How to Run:
make all to generate executables Server and Client. Server takes IP address and port as input. Client has to be given the same IP address and port as input.
make runserver and make runclient runs server and client instances. Client instance has to be run after server instance.
IP and PORT can be set in makefile using IP=<IP> PORT=<PORT>.  Default is 127.0.0.1 (localhost) and 8080.
