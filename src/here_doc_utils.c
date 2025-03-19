/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 23:52:07 by aatieh            #+#    #+#             */
/*   Updated: 2025/03/19 06:24:08 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	here_doc_addback(t_here_doc **head, t_here_doc *new)
{
	t_here_doc	*tmp;

	if (!*head)
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

int	write_expanded_line(int fd, char *line, int i, t_minishell *vars)
{
	line = expand(line, *vars);
	ft_putstr_fd(line, fd);
	ft_putstr_fd("\n", fd);
	free(line);
	return (i + 1);
}

void	close_free_here_doc(t_here_doc **here_doc)
{
	t_here_doc	*tmp;

	while ((*here_doc))
	{
		tmp = (*here_doc);
		(*here_doc) = (*here_doc)->next;
		if (tmp->open)
			close(tmp->fd);
		free(tmp);
	}
	(*here_doc) = NULL;
}
