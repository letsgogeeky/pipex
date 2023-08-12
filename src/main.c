#include "pipex.h"

int main(int argc, char **argv)
{
    pid_t pid;
    int fd;
    int io_src[2];

    if (argc < 4)
        return (1);
    fd = open(argv[1], O_RDONLY);
    dup2(fd, 0);
    close(fd);
    pipe(io_src);
    pid = fork();
    
    return (0);
}