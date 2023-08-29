/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 22:16:30 by ramoussa          #+#    #+#             */
/*   Updated: 2023/08/30 01:04:39 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	cmd_runner(t_program *env, int cmd_arg_idx)
{
	char	**cmd;
	char	*path;

	path = env->argv[cmd_arg_idx];
	if (ft_strncmp(path, "./", 2))
	{
		cmd = ft_split(sanitize_cmd(env->argv[cmd_arg_idx]), ' ');
		cmd = prep_cmd(cmd);
	}
	else
		cmd = make_sh_cmd(sanitize_cmd(env->argv[cmd_arg_idx]));
	dup2(env->ipc[1], 1);
	close(env->ipc[1]);
	close(env->ipc[0]);
	if (env->argv[cmd_arg_idx + 2] == NULL)
		prepare_outfile(cmd_arg_idx + 2, env->argv);
	if (access(path, X_OK | F_OK) < 0)
		path = find_path(env->envp, cmd[0]);
	if (execve(path, cmd, env->envp) == -1)
		abort_and_exit(ft_strjoin(cmd[0], ": command not found"), cmd, 127);
}

int	wait_children(t_program *env)
{
	int	status;
	int	exit_code;
	int	pid;

	pid = 1;
	exit_code = 0;
	while (pid != -1)
	{
		pid = waitpid(-1, &status, WNOHANG);
		if (pid == env->pid)
			exit_code = WEXITSTATUS(status);
	}
	free(env);
	return (exit_code);
}

void	pype(t_program *env, int arg_idx, char *infile_msg)
{
	if (pipe(env->ipc))
		abort_and_exit(ft_strdup("Failed to create a pipe."), NULL, 1);
	env->pid = fork();
	if (env->pid == -1)
		abort_and_exit(ft_strdup("Failed to create a subprocess!"), NULL, 1);
	if (env->pid == 0 && infile_msg != NULL)
		abort_and_exit(infile_msg, NULL, 0);
	if (env->pid == 0)
		cmd_runner(env, arg_idx);
	dup2(env->ipc[0], 0);
	close(env->ipc[0]);
	close(env->ipc[1]);
}

int	main(int argc, char **argv, char **envp)
{
	t_program	*env;
	int			arg_idx;
	char		*infile_msg;

	if (argc < 5 || (is_here_doc(argv) && argc < 5))
		exit(1);
	env = (t_program *)malloc(sizeof(t_program));
	env->argc = argc;
	env->argv = argv;
	env->envp = envp;
	infile_msg = prepare_input(argv);
	arg_idx = 2;
	if (is_here_doc(argv))
		arg_idx = 3;
	while (arg_idx < argc - 1)
	{
		pype(env, arg_idx, infile_msg);
		if (infile_msg != NULL)
		{
			free(infile_msg);
			infile_msg = NULL;
		}
		arg_idx++;
	}
	return (wait_children(env));
}
