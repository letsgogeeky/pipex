/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 22:16:30 by ramoussa          #+#    #+#             */
/*   Updated: 2023/08/15 02:00:45 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	is_here_doc(char **argv)
{
	if(ft_strncmp(argv[1], "here_doc", 9))
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
		// free(doc);
		ft_putstr_fd(doc, ipc[1]);		
		doc = get_next_line(0);
	}
	// free(limiter);
	// if (doc)
	// 	free(doc);
	dup2(ipc[0], 0);
	close(ipc[0]);
	close(ipc[1]);
}

char	*validate_infile(char *infile)
{
	if (access(infile, F_OK))
		return (ft_strjoin(infile, " :: file does not exist."));
	if (access(infile, R_OK))
		return (ft_strjoin(infile, " :: permission denied."));
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
		path = ft_strjoin(path, program);
		if(access(path, X_OK | F_OK) != -1)
			return (path);
		free(path);
		path = NULL;
		idx++;
	}
	return (path);
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

int main(int argc, char **argv, char **envp)
{
    pid_t pid;
    int ipc[2];
	char	**cmd;
	char	*path;
	int		arg_idx;

    if (argc < 4 || (is_here_doc(argv) && argc < 5))
        return (1);
	arg_idx = prepare_input(argv);
	while (arg_idx < argc - 1)
	{
		pipe(ipc);
		pid = fork();
		if (pid == 0)
		{
			cmd = ft_split(argv[arg_idx], ' ');
			dup2(ipc[1], 1);
			close(ipc[1]);
			close(ipc[0]);
			if (arg_idx == argc - 2)
			{
				prepare_outfile(argc, argv);
			}
			path = find_path(envp, cmd[0]);
			if (execve(path, cmd, envp) == -1)
				abort_and_exit(ft_strjoin(cmd[0], " failed to execute."), cmd, 127);
		}
		dup2(ipc[0], 0);
		close(ipc[0]);
		close(ipc[1]);
		waitpid(pid, NULL, WNOHANG);
		arg_idx++;
	}
    return (0);
}
