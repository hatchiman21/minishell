/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 20:47:48 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/27 05:59:44 by aatieh           ###   ########.fr       */
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
	if (cmd && cmd[0])
		path = get_path(cmd, envp);
	if (!cmd || !path)
	{
		free_all(path, NULL);
		ft_dprintf(2, "pipex: dup2 failed\n");
		exit(2);
	}
	execve(path, cmd, envp);
	ft_dprintf(2, "pipex : %s: Is a directory\n", cmd[0]);
	free_all(path, NULL);
	exit(126);
}

void	here_doc(int fd, char *stop_sign)
{
	char	*line;

	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		if (!ft_strncmp(line, stop_sign, ft_strlen(stop_sign))
			&& ft_strlen(line) - 1 == ft_strlen(stop_sign)
			&& line[ft_strlen(stop_sign)] == '\n')
			break ;
		write(fd, line, ft_strlen(line));
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	if (line)
		free(line);
}

int	open_file(t_redirect *red, int pipefd[2])
{
	int	tmp_fd;

	tmp_fd = 0;
	if (!ft_strncmp(red->redirection, ">>", 2))
		tmp_fd = open(red->redirection + 2,  O_WRONLY | O_APPEND | O_CREAT, 0644);
	else if (red->redirection[0] == '>')
		tmp_fd = open(red->redirection + 1,  O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else if (!ft_strncmp(red->redirection, "<<", 2))
		here_doc(pipefd[1], red->redirection + 2);
	else if (red->redirection[0] == '<')
		tmp_fd = open(red->redirection + 1, O_RDONLY);
	if (tmp_fd)
		dup2(tmp_fd, pipefd[0]);
	if (tmp_fd && tmp_fd != -1)
		close(tmp_fd);
	return (tmp_fd);
}

void	apply_redirection(t_minishell *vars, int cur_op)
{
	t_redirect *red;

	if (vars->op_num == cur_op)
		vars->redir = 0;
	else
		vars->redir = 1;
	red = vars->redirections;
	pipe(vars->pipefd);
	while (red && red->op < cur_op)
		red = red->next;
	// printf("1\n");
	while(red && red->op == cur_op)
	{
		open_file(red, vars->pipefd);
		red = red->next;
	}
	// printf("2\n");
	dup2(vars->pipefd[0], STDIN_FILENO);
	// printf("3\n");
	// if (vars->redir == 1)
	// 	dup2(vars->pipefd[1], STDOUT_FILENO);
	// printf("4\n");
	// close(vars->pipefd[0]);
	close(vars->pipefd[0]);
	close(vars->pipefd[1]);
}

void	process(t_minishell *vars)
{
	int	cur_op;
	int	i;

	cur_op = 0;
	i = 0;
	while (cur_op < vars->op_num)
	{
		vars->last_id = fork();
		apply_redirection(vars, cur_op);
		if (!vars->last_id)
			child_process(vars->argv + i, vars->env);
		else
		{
			while (vars->argv[i])
				i++;
			if (i < vars->argc)
				i++;
			cur_op++;
		}
	}
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	dup2(vars->std_in, STDIN_FILENO);
	dup2(vars->std_out, STDOUT_FILENO);
	close(vars->tmp_fd);
}

void	wait_for_all(void)
{
	int	id;

	id = 1;
	while (id != -1)
		id = waitpid(-1, NULL, 0);
}
