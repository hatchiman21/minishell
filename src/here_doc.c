/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:32:13 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/16 03:09:27 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	here_doc_input(char *stop_sign, int fd, char **final_line)
{
	char	*line;
	int		i;

	i = 0;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_dprintf(2, "%s %d delimited by end-of-file (wanted `%s')\n",
				"minishell: warning: here-document at line", i, stop_sign);
			return (0);
		}
		if (add_line(line, final_line) == -1)
		{
			free(line);
			return (-1);
		}
		if (!ft_strncmp(line, stop_sign, ft_strlen(stop_sign))
			&& ft_strlen(line) == ft_strlen(stop_sign))
			break ;
		i = write_line(fd, line, i);
	}
	free(line);
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
		ft_putstr_fd("minishell: here_doc malloc failed\n", 2);
		return (NULL);
	}
	here_doc_node->fd = fd[0];
	here_doc_node->red_order = i;
	here_doc_node->open = true;
	here_doc_node->next = NULL;
	if (here_doc_input(red->redirection + 2, fd[1], &vars->final_line) == -1)
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
		if (!ft_strncmp(red->redirection, "<<", 2))
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
