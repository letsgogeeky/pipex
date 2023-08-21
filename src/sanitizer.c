/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 23:00:39 by ramoussa          #+#    #+#             */
/*   Updated: 2023/08/21 23:09:59 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*sanitize_shell_script(char *cmd)
{
	char	**sh_parts;
	char	*sh_cmd;
	int		parts_len;
	int		idx;

	idx = 0;
	sh_parts = ft_split(cmd, ' ');
	parts_len = str_arr_len(sh_parts);
	if (parts_len < 2)
		return (ra_replace(cmd, '\\', ' '), cmd);
	while (sh_parts && sh_parts[idx + 1])
	{
		sh_cmd = ft_strjoin(sh_cmd, ft_strjoin(sh_parts[idx], "\\ "));
		idx++;
	}
	sh_cmd = ft_strjoin(sh_cmd, sh_parts[idx]);
	return (sh_cmd);
}

char	*sanitize_cmd(char *cmd)
{
	int		idx;

	idx = 0;
	if (ft_strnstr(cmd, ".sh", ft_strlen(cmd)) || !ft_strncmp(cmd, "./", 2))
		return (sanitize_shell_script(cmd));
	if (ft_strncmp(cmd, "awk ", 4) != 0)
		return (ra_replace(cmd, '\"', ' '), ra_replace(cmd, '\'', ' '), cmd);
	idx = 4;
	while (ra_is_whiteshapce(cmd[idx]))
		idx++;
	cmd[4] = ' ';
	cmd[ft_strlen(cmd) - 1] = ' ';
	return (ra_replace(cmd, '\\', ' '), cmd);
}

char	**prep_cmd(char **cmd)
{
	int		len;
	int		idx;
	char	**result;
	char	*cmd_params;

	len = str_arr_len(cmd);
	if (len == 0)
		return (NULL);
	if (len <= 3)
		return (cmd);
	idx = 0;
	result = (char **)malloc(3 * sizeof(char *));
	result[idx] = ft_strdup(cmd[idx]);
	idx++;
	cmd_params = ft_strjoin_s1_free(ft_strdup(""), cmd[idx]);
	idx++;
	while (cmd[idx])
	{
		cmd_params = ft_strjoin_s1_free(
				ft_strjoin_s1_free(cmd_params, " "), cmd[idx]);
		idx++;
	}
	result[1] = cmd_params;
	result[2] = NULL;
	return (result);
}
