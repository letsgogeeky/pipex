/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 02:12:50 by ramoussa          #+#    #+#             */
/*   Updated: 2023/08/21 20:57:24 by ramoussa         ###   ########.fr       */
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
	env_paths = ft_split(ft_substr(envp[idx], 5, ft_strlen(envp[idx]) - 5), ':');
	idx = 0;
	while(env_paths[idx])
	{
		path = ft_strjoin(env_paths[idx], "/");
		path = ft_strjoin_s1_free(path, program);
		// ft_putendl_fd(path, 2);
		if(!access(path, X_OK | F_OK))
			return (path);
		// {
		// 	if (ft_strnstr(path, "/bin", ft_strlen(path)) || ft_strnstr(path, "/sbin", ft_strlen(path)))
		// 		return (path);
		// 	return (ft_strdup("/bin/sh"));
		// }
		free(path);
		path = NULL;
		idx++;
	}
	return (NULL);
}
