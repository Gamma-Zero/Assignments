Background Music: Kaine/Escape from NieR Replicant (Keiichi Okabe, Emi Evans).

Game Controls:
Move with numkeys.
Throw bomb with E.
Shoot arrow with Q.

Game Mechanics:
Arrows do 20 damage.
Bombs do 50 damage. Friendly fire is on.
On death, enemies drop lootboxes. Lootboxes may contain arrows, bombs, score boosters and health refills. 
Game time limit is 150 seconds. 
If a player dies, the other player wins. 
At the end of the time limit, the player with more score wins. 

How to Run:
make all to generate executables Server and Client. Server takes IP address and port as input. Client has to be given the same IP address and port as input.
make runserver and make runclient runs server and client instances. Client instance has to be run after server instance.
IP and PORT can be set in makefile using IP=<IP> PORT=<PORT>.  Default is 127.0.0.1 (localhost) and 8080.
