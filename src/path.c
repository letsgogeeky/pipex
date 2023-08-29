/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 01:14:26 by ramoussa          #+#    #+#             */
/*   Updated: 2023/08/30 01:16:47 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*find_path_in_env(char **envp, char *program)
{
	int		idx;
	char	*path;
	char	**env_paths;

	idx = 0;
	while (envp[idx] && ft_strncmp(envp[idx], "PATH=", 5))
		idx++;
	path = ft_substr(envp[idx], 5, ft_strlen(envp[idx]) - 5);
	env_paths = ft_split(path, ':');
	free(path);
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
	str_arr_free(env_paths);
	return (NULL);
}

char	*find_path(char **envp, char *program)
{
	if (!envp || str_arr_len(envp) == 0)
		return (ft_strjoin(ft_strdup("/usr/bin/"), program));
	if (ft_strncmp(program, "./", 2) == 0)
		return (ft_strdup("/bin/sh"));
	return (find_path_in_env(envp, program));
}
