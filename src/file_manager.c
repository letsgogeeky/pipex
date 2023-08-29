/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 02:12:50 by ramoussa          #+#    #+#             */
/*   Updated: 2023/08/30 01:15:55 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*validate_infile(char *infile)
{
	if (access(infile, F_OK))
		return (ft_strjoin(infile, ": No such file or directory"));
	if (access(infile, R_OK))
		return (ft_strjoin(infile, ": permission denied."));
	return (NULL);
}

char	*validate_outfile(char *outfile)
{
	if (!access(outfile, F_OK) && access(outfile, W_OK) < 0)
		return (ft_strjoin(outfile, " :: write permission denied."));
	return (NULL);
}

char	*prepare_input(char **argv)
{
	int		arg_idx;
	char	*file_access;
	int		fd;

	arg_idx = 1;
	if (is_here_doc(argv))
	{
		read_here_doc(argv);
		return (NULL);
	}
	file_access = validate_infile(argv[arg_idx]);
	if (file_access)
		return (file_access);
	fd = open(argv[arg_idx], O_RDONLY);
	dup2(fd, 0);
	close(fd);
	return (NULL);
}

void	prepare_outfile(int argc, char **argv)
{
	char	*file_access;
	int		fd;

	file_access = validate_outfile(argv[argc - 1]);
	if (file_access != NULL)
		abort_and_exit(file_access, NULL, 1);
	if (is_here_doc(argv))
		fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(fd, 1);
	close(fd);
}
