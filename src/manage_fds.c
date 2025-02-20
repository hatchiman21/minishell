/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_fds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 23:48:40 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/20 22:18:52 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	change_fds(t_minishell *vars, int fd, int cur_op, int out)
{
	int	error;

	if (fd == -1)
		return ;
	error = 0;
	if (out == 1)
	{
		if (cur_op != vars->op_num - 1)
			error += dup2(fd, vars->pipefd[1]);
		else
			error += dup2(fd, STDOUT_FILENO);
	}
	else
	{
		if (cur_op != 0)
			error += dup2(fd, vars->tmp_fd);
		else
			error += dup2(fd, STDIN_FILENO);
	}
	if (error < 0)
		ft_putstr_fd("minishell: dup2 failed\n", 2);
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

int	open_file(t_minishell *vars, t_redirect *red, int red_order)
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
	}
	change_fds(vars, fd, red->op, out);
	if (fd != -1)
		close(fd);
	return (fd);
}
