/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 23:36:37 by ramoussa          #+#    #+#             */
/*   Updated: 2023/08/29 23:06:57 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	is_here_doc(char **argv)
{
	if (ft_strncmp(argv[1], "here_doc", 9))
		return (0);
	return (1);
}

void	read_here_doc(char **argv)
{
	char	*doc;
	char	*limiter;
	int		ipc[2];

	if (pipe(ipc))
		abort_and_exit(ft_strdup("here_doc :: error while piping."), NULL, 1);
	limiter = ft_strjoin(argv[2], "\n");
	doc = get_next_line(0);
	while (doc && ft_strncmp(limiter, doc, ft_strlen(doc) - 1))
	{
		ft_putstr_fd(doc, ipc[1]);
		free(doc);
		doc = get_next_line(0);
	}
	free(limiter);
	if (doc)
		free(doc);
	dup2(ipc[0], 0);
	close(ipc[0]);
	close(ipc[1]);
}
