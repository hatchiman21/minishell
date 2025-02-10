/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 20:47:48 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/10 16:13:26 by aatieh           ###   ########.fr       */
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

int	child_process(char **cmd, char **envp)
{
	char	*path;

	path = NULL;
	if (!cmd || !cmd[0])
		exit(0);
	if (cmd && cmd[0])
		path = get_path(cmd, envp);
	if (!cmd || !path)
	{
		free_all(path, NULL);
		ft_dprintf(2, "minishell: dup2 failed\n");
		exit(2);
	}
	execve(path, cmd, envp);
	ft_dprintf(2, "minishell: %s: premission denied\n", cmd[0]);
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

void	open_file(t_minishell *vars, t_redirect *red, int red_order)
{
	int	fd;
	int	out;

	fd = -1;
	if (red->redirection[0] == '>')
		out = 1;
	else
		out = 0;
	if (!ft_strncmp(red->redirection, ">>", 2))
		fd = open(red->redirection + 2, O_WRONLY | O_APPEND | O_CREAT, 0644);
	else if (red->redirection[0] == '>')
		fd = open(red->redirection + 1, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else if (!ft_strncmp(red->redirection, "<<", 2))
		fd = get_here_doc_fd(vars->here_doc_fds, red_order);
	else if (red->redirection[0] == '<')
		fd = open(red->redirection + 1, O_RDONLY);
	change_fds(vars, fd, red->op, out);
	if (fd != -1)
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
		pipe(vars->pipefd);
		vars->last_id = fork();
		if (!vars->last_id)
		{
			apply_redirection(vars, cur_op);
			close_free_here_doc(&vars->here_doc_fds);
			child_process(vars->argv + i, vars->env);
		}
		else
		{
			if (cur_op != 0)
				close(vars->tmp_fd);
			close(vars->pipefd[1]);
			while (vars->argv[i])
				i++;
			if (i < vars->argc)
				i++;
			cur_op++;
		}
	}
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
