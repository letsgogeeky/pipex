/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 22:16:30 by ramoussa          #+#    #+#             */
/*   Updated: 2023/08/15 02:40:14 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
		free(doc);
		ft_putstr_fd(doc, ipc[1]);		
		doc = get_next_line(0);
	}
	free(limiter);
	if (doc)
		free(doc);
	dup2(ipc[0], 0);
	close(ipc[0]);
	close(ipc[1]);
}

int	prepare_input(char **argv)
{
	int		arg_idx;
	char	*file_access;
	int		fd;
	
	arg_idx = 1;
	if (is_here_doc(argv))
	{
		read_here_doc(argv);
		arg_idx += 2;
		return (arg_idx);
	}
	file_access = validate_infile(argv[arg_idx]);
	if (file_access)
		abort_and_exit(file_access, NULL, 1);
    fd = open(argv[arg_idx], O_RDONLY);
	dup2(fd, 0);
	close(fd);
	arg_idx++;
	return (arg_idx);
}

void	prepare_outfile(int argc, char **argv)
{
	char	*file_access;
	int		fd;
	file_access = validate_outfile(argv[argc - 1]);
	if (file_access)
		abort_and_exit(file_access, NULL, 1);
	if (is_here_doc(argv))
		fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(fd, 1);
	close(fd);
}

void	cmd_runner(char **argv, char **envp, int *ipc, int cmd_arg_idx)
{
	char	**cmd;
	char	*path;
	
	cmd = ft_split(argv[cmd_arg_idx], ' ');
	dup2(ipc[1], 1);
	close(ipc[1]);
	close(ipc[0]);
	if (argv[cmd_arg_idx + 2] == NULL)
		prepare_outfile(cmd_arg_idx + 2, argv);
	path = find_path(envp, cmd[0]);
	if (execve(path, cmd, envp) == -1)
		abort_and_exit(ft_strjoin(cmd[0], " failed to execute."), cmd, 127);
}

int main(int argc, char **argv, char **envp)
{
    pid_t pid;
    int ipc[2];
	
	int		arg_idx;

    if (argc < 4 || (is_here_doc(argv) && argc < 5))
        return (1);
	arg_idx = prepare_input(argv);
	while (arg_idx < argc - 1)
	{
		if (pipe(ipc))
			abort_and_exit(ft_strdup("Failed to create a pipe."), NULL, 1);
		pid = fork();
		if (pid == -1)
			abort_and_exit(ft_strdup("Failed to create a subprocess!"), NULL, 1);
		if (pid == 0)
			cmd_runner(argv, envp, ipc, arg_idx);
		dup2(ipc[0], 0);
		close(ipc[0]);
		close(ipc[1]);
		waitpid(pid, NULL, WNOHANG);
		arg_idx++;
	}
    return (0);
}
