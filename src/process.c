/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 20:47:48 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/21 14:52:39 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_excute(char *path, char **cmd, t_minishell *vars)
{
	if (!cmd && cmd[0])
		return ;
	if (!ft_strncmp(cmd[0], "cd", 3))
		my_cd(cmd, vars->env, vars);
	else if (!ft_strncmp(cmd[0], "export", 7) && !cmd[1])
		export3(vars->env, vars, array_size(vars->env));
	else if (!ft_strncmp(cmd[0], "export", 7))
		vars->env = export(vars->env, cmd + 1, vars);
	else if (!ft_strncmp(cmd[0], "unset", 6))
		vars->env = unset(vars->env, cmd + 1, vars);
	else if (!ft_strncmp(cmd[0], "exit", 5))
		ft_exit(cmd, vars);
	else if (!ft_strncmp(cmd[0], "env", 4))
		vars->exit_status = env(vars->env, cmd);
	else if (!ft_strncmp(cmd[0], "pwd", 4))
		vars->exit_status = pwd();
	else if (!ft_strncmp(cmd[0], "echo", 5))
		vars->exit_status = ft_echo(cmd);
	else
		execve(path, cmd, vars->env);
}

int	child_process(char **cmd, t_minishell *vars)
{
	char	*path;

	path = NULL;
	if (!cmd || !cmd[0])
		exit(0);
	if (!cmd_built_in(cmd))
		path = get_path(cmd, vars->env);
	ft_excute(path, cmd, vars);
	ft_dprintf(2, "minishell: %s: is a directory\n", cmd[0]);
	free(path);
	exit(126);
}

void	process_step(t_minishell *vars, int *cur_op, int *i)
{
	int	red_success;

	red_success = apply_redirection(vars, *cur_op);
	if (red_success && not_child_process(vars->argv + *i, vars))
		ft_excute(NULL, vars->argv + *i, vars);
	else if (red_success)
		vars->last_id = fork();
	else
	{
		vars->exit_status = 1;
		vars->last_id = -2;
	}
}

void	process_operation(t_minishell *vars, int *i, int *cur_op)
{
	process_step(vars, cur_op, i);
	if (!vars->last_id)
	{
		close_fds((int [4]){vars->std_in, vars->std_out, vars->pipefd[0], -1});
		close_free_here_doc(&vars->here_doc_fds);
		child_process(vars->argv + *i, vars);
	}
	else
	{
		if (*cur_op != 0)
			close(vars->tmp_fd);
		close(vars->pipefd[1]);
		if (vars->last_id == -1)
			ft_dprintf(2, "minishell: fork failed in command %d\n", *cur_op);
		while (vars->argv[*i])
			(*i)++;
		if (*i < vars->argc)
			(*i)++;
		(*cur_op)++;
	}
}

int	process(t_minishell *vars)
{
	int	cur_op;
	int	i;

	cur_op = 0;
	i = 0;
	while (cur_op < vars->op_num && cur_op != -1)
	{
		if (intial_red(vars, &cur_op) == -1)
			break ;
		process_operation(vars, &i, &cur_op);
		if (dup2(vars->std_in, STDIN_FILENO) == -1
			|| dup2(vars->std_out, STDOUT_FILENO) == -1)
		{
			ft_putstr_fd("minishell: dup2 failed\n", 2);
			cur_op = -1;
			break ;
		}
	}
	if (vars->pipefd[0] != -1)
		close(vars->pipefd[0]);
	return (cur_op);
}
