/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 20:47:48 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/11 17:49:01 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_all(char *str, char **split)
{
	int	i;

	if (str)
		free(str);
	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	ft_excute(char *path, char **cmd, t_minishell *vars)
{
	if (!ft_strncmp(cmd[0], "cd", 3))
		my_cd(vars);
	// else if (!ft_strncmp(cmd[0], "export", 7))
	// 	export(vars->env, vars->env);
	// else if (!ft_strncmp(cmd[0], "unset", 6))
	// 	unset(cmd, vars->env);
	// else if (!ft_strncmp(cmd[0], "exit", 5))
	// 	exit1(cmd, vars);
	// else if (!ft_strncmp(cmd[0], "env", 4))
	// 	env(vars->env);
	// else if (!ft_strncmp(cmd[0], "pwd", 4))
	// 	pwd();
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
	if (cmd && cmd[0])
		path = get_path(cmd, vars->env);
	if (!cmd || !path)
	{
		free_all(path, NULL);
		ft_dprintf(2, "minishell: dup2 failed\n");
		exit(1);
	}
	ft_excute(path, cmd, vars);
	ft_dprintf(2, "minishell: %s: is a directory\n", cmd[0]);
	free_all(path, NULL);
	exit(126);
}

void	change_fds(t_minishell *vars, int fd, int cur_op, int out)
{
	if (out == 1)
	{
		if (cur_op != vars->op_num - 1)
			dup2(fd, vars->pipefd[1]);
		else
			dup2(fd, STDOUT_FILENO);
	}
	else
	{
		if (cur_op != 0)
			dup2(fd, vars->tmp_fd);
		else
			dup2(fd, STDIN_FILENO);
	}
}

int	get_fd(t_minishell *vars, t_redirect *red, int red_order)
{
	int	fd;

	fd = -1;
	if (!ft_strncmp(red->redirection, ">>", 2))
		fd = open(red->redirection + 2, O_WRONLY | O_APPEND | O_CREAT, 0644);
	else if (red->redirection[0] == '>')
		fd = open(red->redirection + 1, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else if (!ft_strncmp(red->redirection, "<<", 2))
		fd = get_here_doc_fd(vars->here_doc_fds, red_order);
	else if (red->redirection[0] == '<')
		fd = open(red->redirection + 1, O_RDONLY);
	return (fd);
}

void	open_file(t_minishell *vars, t_redirect *red, int red_order)
{
	int	fd;
	int	out;

	if (red->redirection[0] == '>')
		out = 1;
	else
		out = 0;
	fd = get_fd(vars, red, red_order);
	if (fd == -1)
	{
		if (red->redirection[1] == '<' || red->redirection[1] == '>')
			ft_dprintf(2, "minishell: %s: %s\n",
				red->redirection + 2, strerror(errno));
		else
			ft_dprintf(2, "minishell: %s: %s\n",
				red->redirection + 1, strerror(errno));
		close_free_here_doc(&vars->here_doc_fds);
		close(vars->pipefd[0]);
		close(vars->pipefd[1]);
		exit(1);
	}
	change_fds(vars, fd, red->op, out);
	close(fd);
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
		dup2(vars->tmp_fd, STDIN_FILENO);
		close(vars->tmp_fd);
	}
	close(vars->pipefd[1]);
	close(vars->pipefd[0]);
}

void	process_operation(t_minishell *vars, int *i, int *cur_op)
{
	vars->last_id = fork();
	if (!vars->last_id)
	{
		apply_redirection(vars, *cur_op);
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
	}
	if (vars->pipefd[0] != -1)
		close(vars->pipefd[0]);
	wait_for_all(vars);
}

void	wait_for_all(t_minishell *vars)
{
	int	id;
	int	status;

	id = 1;
	while (id != -1)
	{
		id = waitpid(-1, &status, 0);
		if (id == vars->last_id)
		{
			if (WIFEXITED(status))
				vars->exit_status = WEXITSTATUS(status);
			else
				vars->exit_status = 128 + WTERMSIG(status);
		}
	}
}
