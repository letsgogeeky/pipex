#ifndef PIPEX_H
# define PIPEX_H

# include "baselib.h"
# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>
# include <errno.h>
# include <stdlib.h>

char	*validate_infile(char *infile);
char	*validate_outfile(char *outfile);
char	*find_path(char **envp, char *program);
int		is_here_doc(char **argv);
void	abort_and_exit(char *message, char **on_cmd, int code);

#endif