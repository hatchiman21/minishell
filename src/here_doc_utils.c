/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 23:52:07 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/15 06:29:05 by aatieh           ###   ########.fr       */
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

int	write_line(int fd, char *line, int i)
{
	ft_putstr_fd(line, fd);
	ft_putchar_fd('\n', fd);
	free(line);
	rl_replace_line("", 0);
	rl_on_new_line();
	i++;
	return (i);
}

int	add_line(char *line, char **final_line)
{
	*final_line = ft_merge(*final_line, "\n", 1, 0);
	if (!*final_line)
	{
		ft_putstr_fd("minishell: here_doc input malloc failed\n", 2);
		return (-1);
	}
	*final_line = ft_merge(*final_line, line, 1, 0);
	if (!*final_line)
	{
		ft_putstr_fd("minishell: here_doc input malloc failed\n", 2);
		return (-1);
	}
	return (0);
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
