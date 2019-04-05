gcc init-shared.c -o is 
gcc server-pembeli.c -o sb
gcc -pthread server-penjual.c -o sp
gcc client.c -o cc
gcc client-two.c -o ct
