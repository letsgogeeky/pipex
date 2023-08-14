/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 22:16:30 by ramoussa          #+#    #+#             */
/*   Updated: 2023/08/14 02:42:32 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	return (str_arr_free(env_paths), path);
}

int main(int argc, char **argv, char **envp)
{
    pid_t pid;
    int fd;
    int ipc[2];
	char	**cmd;
	char	*path;

    if (argc < 4)
        return (1);
    fd = open(argv[1], O_RDONLY);
	cmd = ft_split(argv[2], ' ');
    dup2(fd, 0);
    close(fd);
    pipe(ipc);
    pid = fork();
	path = find_path(envp, cmd[0]);
	execve(path, cmd, envp);
    waitpid(pid, NULL, WNOHANG);
    return (0);
}
