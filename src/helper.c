/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 01:01:38 by ramoussa          #+#    #+#             */
/*   Updated: 2023/08/15 01:54:31 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	abort_and_exit(char *message, char **on_cmd, int code)
{
	if (on_cmd != NULL)
		free(on_cmd);
	ft_putendl_fd(message, 2);
	free(message);
	exit(code);	
}
