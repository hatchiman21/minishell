/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:32:13 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/25 23:51:01 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	here_doc_input(char *stop_sign, int fd, t_minishell *vars)
{
	int		i;

	i = 0;
	while (1)
	{
		ft_putstr_fd("> ", 1);
		vars->line = get_next_line(STDIN_FILENO);
		if (g_ctrl_c)
			break ;
		if (!vars->line)
		{
			ft_dprintf(2, "\n%s %d delimited by end-of-file (wanted `%s')\n",
				"minishell: warning: here-document at line", i, stop_sign);
			return (0);
		}
		if (!ft_strncmp(vars->line, stop_sign, ft_strlen(stop_sign) - 1)
			&& ft_strlen(vars->line) == ft_strlen(stop_sign) + 1)
			break ;
		i = write_line(fd, vars->line, i);
	}
	if (!g_ctrl_c)
		free(vars->line);
	else if (vars->line)
		remove_from_line(vars->line, ft_strlen(vars->line) - 1, 1);
	return (0);
}

t_here_doc	*get_here_doc_node(int fd[2], int i,
				t_minishell *vars, t_redirect *red)
{
	t_here_doc	*here_doc_node;

	here_doc_node = malloc(sizeof(t_here_doc));
	if (!here_doc_node)
	{
		close(fd[1]);
		close(fd[0]);
		return (NULL);
	}
	here_doc_node->fd = fd[0];
	here_doc_node->red_order = i;
	here_doc_node->open = true;
	here_doc_node->next = NULL;
	if (here_doc_input(red->content + 3, fd[1], vars) == -1)
	{
		close(fd[1]);
		close(fd[0]);
		free(here_doc_node);
		return (NULL);
	}
	close(fd[1]);
	return (here_doc_node);
}

void	prepare_here_doc(t_minishell *vars, t_redirect *red)
{
	int			fd[2];
	int			i;
	t_here_doc	*here_doc_node;

	i = 0;
	while (red && ++i)
	{
		if (!ft_strncmp(red->content, "<<", 2) && !g_ctrl_c)
		{
			if (pipe(fd) == -1)
			{
				close_free_here_doc(&vars->here_doc_fds);
				ft_putstr_fd("minishell: here_doc pipe failed\n", 2);
				return ;
			}
			here_doc_node = get_here_doc_node(fd, i - 1, vars, red);
			if (!here_doc_node)
			{
				close_free_here_doc(&vars->here_doc_fds);
				return ;
			}
			here_doc_addback(&vars->here_doc_fds, here_doc_node);
		}
		red = red->next;
	}
}

int	get_here_doc_fd(t_here_doc *here_doc, int red_order)
{
	t_here_doc	*tmp;

	tmp = here_doc;
	while (tmp)
	{
		if (tmp->red_order == red_order)
			return (tmp->fd);
		tmp = tmp->next;
	}
	return (-1);
}
