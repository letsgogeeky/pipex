/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 02:12:50 by ramoussa          #+#    #+#             */
/*   Updated: 2023/08/21 23:39:46 by ramoussa         ###   ########.fr       */
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

char	*find_path(char **envp, char *program)
{
	int		idx;
	char	*path;
	char	**env_paths;

	if (!envp || str_arr_len(envp) == 0)
		return (ft_strjoin(ft_strdup("/usr/bin/"), program));
	if (ft_strncmp(program, "./", 2) == 0)
		return (ft_strdup("/bin/sh"));
	idx = 0;
	while (envp[idx] && ft_strncmp(envp[idx], "PATH=", 5))
		idx++;
	env_paths = ft_split(
			ft_substr(envp[idx], 5, ft_strlen(envp[idx]) - 5), ':');
	idx = 0;
	while (env_paths[idx])
	{
		path = ft_strjoin(env_paths[idx], "/");
		path = ft_strjoin_s1_free(path, program);
		if (!access(path, X_OK | F_OK))
			return (path);
		free(path);
		path = NULL;
		idx++;
	}
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
