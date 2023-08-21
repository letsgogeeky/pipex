/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 01:01:38 by ramoussa          #+#    #+#             */
/*   Updated: 2023/08/21 23:39:36 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	abort_and_exit(char *message, char **on_cmd, int code)
{
	if (on_cmd != NULL)
		free(on_cmd);
	message = ft_strjoin_s1_free(ft_strdup("pipex: "), message);
	ft_putendl_fd(message, 2);
	free(message);
	exit(code);
}

int	env_var_idx(char **envp, char *var)
{
	int		idx;

	idx = 0;
	while (envp && envp[idx] && ft_strncmp(envp[idx], var, ft_strlen(var)))
		idx++;
	if (!envp[idx])
		abort_and_exit(ft_strdup("Variable not found in env."), NULL, 1);
	return (idx);
}

char	**make_sh_cmd(char *sh_script)
{
	char	**cmd;

	cmd = (char **)malloc(4 * sizeof(char *));
	cmd[0] = ft_strdup("/bin/sh");
	cmd[2] = ft_strdup(sh_script);
	cmd[3] = NULL;
	return (cmd);
}
