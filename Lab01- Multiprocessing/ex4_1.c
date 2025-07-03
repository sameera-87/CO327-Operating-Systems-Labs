#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    execl("/bin/ls", "ls", "-l", argv[1], NULL);
    puts("Program ls has terminated");
}
