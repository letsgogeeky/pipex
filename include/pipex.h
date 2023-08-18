/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 01:11:32 by ramoussa          #+#    #+#             */
/*   Updated: 2023/08/18 01:11:34 by ramoussa         ###   ########.fr       */
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

char	*validate_infile(char *infile);
char	*validate_outfile(char *outfile);
char	*find_path(char **envp, char *program);
int		is_here_doc(char **argv);
void	abort_and_exit(char *message, char **on_cmd, int code);

#endif