/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 01:11:32 by ramoussa          #+#    #+#             */
/*   Updated: 2023/08/30 00:04:46 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "baselib.h"
# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>
# include <errno.h>
# include <stdlib.h>
# include <sys/wait.h>

typedef struct s_program
{
	int		argc;
	char	**argv;
	char	**envp;
	int		ipc[2];
	pid_t	pid;
}	t_program;

char	*validate_infile(char *infile);
char	*validate_outfile(char *outfile);
char	*find_path(char **envp, char *program);
int		is_here_doc(char **argv);
void	abort_and_exit(char *message, char **on_cmd, int code);
char	*sanitize_cmd(char *cmd);
char	**make_sh_cmd(char *sh_script);
char	**prep_cmd(char **cmd);
void	read_here_doc(char **argv);

char	*prepare_input(char **argv);
void	prepare_outfile(int argc, char **argv);

#endif