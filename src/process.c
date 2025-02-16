/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 20:47:48 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/16 03:11:17 by aatieh           ###   ########.fr       */
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
		vars->env = export(vars->env, cmd + 1);
	else if (!ft_strncmp(cmd[0], "unset", 6))
		vars->env = unset(vars->env, cmd + 1, vars);
	else if (!ft_strncmp(cmd[0], "exit", 5))
		ft_exit(cmd, vars);
	else if (!ft_strncmp(cmd[0], "env", 4))
		env(vars->env, cmd);
	else if (!ft_strncmp(cmd[0], "pwd", 4))
		pwd();
	else if (!ft_strncmp(cmd[0], "echo", 5))
		ft_echo(cmd);
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
	{
		path = get_path(cmd, vars->env);
		if (!path)
		{
			free(path);
			ft_dprintf(2, "minishell: dup2 failed\n");
			exit(1);
		}
	}
	ft_excute(path, cmd, vars);
	ft_dprintf(2, "minishell: %s: is a directory\n", cmd[0]);
	free(path);
	exit(126);
}

void	apply_redirection(t_minishell *vars, int cur_op)
{
	t_redirect	*red;
	int			red_order;

	red = vars->redirections;
	red_order = 0;
	while (red && red->op < cur_op)
	{
		red_order++;
		red = red->next;
	}
	while (red && red->op == cur_op)
	{
		open_file(vars, red, red_order);
		red_order++;
		red = red->next;
	}
	if (cur_op != vars->op_num - 1)
		dup2(vars->pipefd[1], STDOUT_FILENO);
	if (cur_op != 0)
	{
		if (dup2(vars->tmp_fd, STDIN_FILENO) == -1)
			ft_dprintf(2, "minishell: pipe dup2 failed\n");
		close(vars->tmp_fd);
	}
	close(vars->pipefd[1]);
}

void	process_operation(t_minishell *vars, int *i, int *cur_op)
{
	apply_redirection(vars, *cur_op);
	if (not_child_process(vars->argv + *i, vars))
		ft_excute(NULL, vars->argv + *i, vars);
	else
		vars->last_id = fork();
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

void	process(t_minishell *vars)
{
	int	cur_op;
	int	i;

	cur_op = 0;
	i = 0;
	while (cur_op < vars->op_num)
	{
		if (cur_op != 0)
			vars->tmp_fd = vars->pipefd[0];
		if (pipe(vars->pipefd) == -1)
		{
			ft_dprintf(2, "minishell: child pipe failed\n");
			break ;
		}
		process_operation(vars, &i, &cur_op);
		if (dup2(vars->std_in, STDIN_FILENO) == -1)
			ft_dprintf(2, "minishell: stdin dup2 failed\n");
		if (dup2(vars->std_out, STDOUT_FILENO) == -1)
			ft_dprintf(2, "minishell: stdout dup2 failed\n");
	}
	if (vars->pipefd[0] != -1)
		close(vars->pipefd[0]);
	wait_for_all(vars);
}
