/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 01:01:38 by ramoussa          #+#    #+#             */
/*   Updated: 2023/08/21 19:41:31 by ramoussa         ###   ########.fr       */
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

int	is_here_doc(char **argv)
{
	if(ft_strncmp(argv[1], "here_doc", 9))
		return (0);
	return (1);
}
