/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 22:16:30 by ramoussa          #+#    #+#             */
/*   Updated: 2023/08/20 05:55:47 by ramoussa         ###   ########.fr       */
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
char	*validate_cmd(char *cmd)
{
	int	idx;

	if (ft_strncmp(cmd, "awk ", 4) != 0)
		return (ra_replace(cmd, '\"', ' '), ra_replace(cmd, '\'', ' '), cmd);
	idx = 4;
	while (ra_is_whiteshapce(cmd[idx]))
		idx++;
	// if ((cmd[idx] == '\'' || cmd[idx] == '\"') && (cmd[idx+1] == '\'' || cmd[idx+1] == '\"'))
	// 	abort_and_exit(ft_strdup("Syntax error"), NULL, 2);
	cmd[4] = ' ';
	// if (cmd[5] == '\'' || cmd[5] == '\"')
	// 	abort_and_exit(ft_strdup("Whatever for now"), NULL, 2);
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
		cmd_params = ft_strjoin_s1_free(ft_strjoin_s1_free(cmd_params, " "), cmd[idx]);
		idx++;
	}
	result[1] = cmd_params;
	result[2] = NULL;
	return (result);
}

char	*prepare_input(char **argv)
{
	int		arg_idx;
	char	*file_access;
	int		fd;
	
	arg_idx = 1;
	if (is_here_doc(argv))
	{
		read_here_doc(argv);
		return (NULL);
	}
	file_access = validate_infile(argv[arg_idx]);
	if (file_access)
		return (file_access);
    fd = open(argv[arg_idx], O_RDONLY);
	dup2(fd, 0);
	close(fd);
	return (NULL);
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

void	cmd_runner(char **argv, char **envp, int *ipc, int cmd_arg_idx)
{
	char	**cmd;
	char	*path;

	cmd = ft_split(validate_cmd(argv[cmd_arg_idx]), ' ');
	cmd = prep_cmd(cmd);
	dup2(ipc[1], 1);
	close(ipc[1]);
	close(ipc[0]);
	if (argv[cmd_arg_idx + 2] == NULL)
		prepare_outfile(cmd_arg_idx + 2, argv);
	path = find_path(envp, cmd[0]);
	if (execve(path, cmd, envp) == -1)
	{
		perror("execve");
		exit(1);
	}
		// abort_and_exit(ft_strjoin(cmd[0], ": command not found"), cmd, 127);
}
int	wait_children(pid_t last_pid)
{
	int	status;
	int	exit_code;
	int	pid;

	pid = 1;
	exit_code = 0;
	while (pid != -1)
	{
		pid = waitpid(-1, &status, WNOHANG);
		if (pid == last_pid)
			exit_code = WEXITSTATUS(status);
	}
	return (exit_code);
}

int main(int argc, char **argv, char **envp)
{
    pid_t	pid;
    int		ipc[2];
	int		arg_idx;
	char	*infile_msg;

    if (argc < 4 || (is_here_doc(argv) && argc < 5))
        return (1);
	infile_msg = prepare_input(argv);
	arg_idx = 2;
	if (is_here_doc(argv))
		arg_idx = 3;
	while (arg_idx < argc - 1)
	{
		if (pipe(ipc))
			abort_and_exit(ft_strdup("Failed to create a pipe."), NULL, 1);
		pid = fork();
		if (pid == -1)
			abort_and_exit(ft_strdup("Failed to create a subprocess!"), NULL, 1);
		if (pid == 0 && infile_msg != NULL)
			abort_and_exit(infile_msg, NULL, 0);
		if (pid == 0)
			cmd_runner(argv, envp, ipc, arg_idx);
		dup2(ipc[0], 0);
		close(ipc[0]);
		close(ipc[1]);
		if (infile_msg != NULL)
		{
			free(infile_msg);
			infile_msg = NULL;
		}
		arg_idx++;
	}
    return (wait_children(pid));
}
