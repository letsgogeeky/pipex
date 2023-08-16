/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 01:01:38 by ramoussa          #+#    #+#             */
/*   Updated: 2023/08/16 19:45:37 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	abort_and_exit(char *message, char **on_cmd, int code)
{
	if (on_cmd != NULL)
		free(on_cmd);
	ft_putendl_fd(message, 2);
	ft_putnbr_fd(errno, 2);
	ft_putchar_fd('\n', 2);
	free(message);
	exit(code);	
}

int	is_here_doc(char **argv)
{
	if(ft_strncmp(argv[1], "here_doc", 9))
		return (0);
	return (1);
}
