/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 02:12:50 by ramoussa          #+#    #+#             */
/*   Updated: 2023/08/16 20:45:09 by ramoussa         ###   ########.fr       */
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
	idx = 0;
	while (envp && envp[idx] && ft_strncmp(envp[idx], "PATH=", 5))
		idx++;
	env_paths = ft_split(envp[idx], ':');
	idx = 0;
	while(env_paths[idx])
	{
		path = ft_strjoin(env_paths[idx], "/");
		path = ft_strjoin_s1_free(path, program);
		if(access(path, X_OK | F_OK) != -1)
			return (path);
		free(path);
		path = NULL;
		idx++;
	}
	return (program);
}
