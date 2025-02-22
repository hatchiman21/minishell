/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_fds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 23:48:40 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/22 15:50:00 by aatieh           ###   ########.fr       */
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
	if (!ft_strncmp(red->content, ">>", 2))
		fd = open(red->content + 3, O_WRONLY | O_APPEND | O_CREAT, 0644);
	else if (red->content[0] == '>')
		fd = open(red->content + 2, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else if (!ft_strncmp(red->content, "<<", 2))
		fd = get_here_doc_fd(vars->here_doc_fds, red_order);
	else if (red->content[0] == '<')
		fd = open(red->content + 2, O_RDONLY);
	return (fd);
}

int	open_file(t_minishell *vars, t_redirect *red, int red_order)
{
	int	fd;
	int	out;

	red->content = rm_qoutes(red->content);
	if (red->content[0] == '>')
		out = 1;
	else
		out = 0;
	fd = get_fd(vars, red, red_order);
	if (fd == -1)
	{
		printf("error\n");
		if (red->content[1] == '<' || red->content[1] == '>')
			ft_dprintf(2, "minishell: %s: %s\n",
				red->content + 3, strerror(errno));
		else
			ft_dprintf(2, "minishell: %s: %s\n",
				red->content + 2, strerror(errno));
	}
	change_fds(vars, fd, red->op, out);
	if (fd != -1)
		close(fd);
	return (fd);
}
