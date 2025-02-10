/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:32:13 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/10 16:25:51 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	here_doc_input(char *stop_sign, int fd, char **final_line)
{
	char	*line;
	int		i;

	line = readline("> ");
	i = 0;
	while (1)
	{
		if (!line)
		{
			ft_dprintf(2,"%s %d delimited by end-of-file (wanted `%s')\n",
				"minishell: warning: here-document at line", i, stop_sign);
			break ;
		}
		*final_line = ft_merge(*final_line, "\n", 1, 0);
		*final_line = ft_merge(*final_line, line, 1, 0);
		if (!ft_strncmp(line, stop_sign, ft_strlen(stop_sign))
			&& ft_strlen(line) == ft_strlen(stop_sign))
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		rl_replace_line("", 0);
		rl_on_new_line();
		line = readline("> ");
		i++;
	}
	if (line)
		free(line);
}

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
	new->next = NULL;
}

t_here_doc	*prepare_here_doc(t_minishell *vars)
{
	int			fd[2];
	int			i;
	t_here_doc	*here_doc_node;
	t_here_doc	*head;
	t_redirect	*red;

	i = 0;
	red = vars->redirections;
	head = NULL;
	while(red)
	{
		if (!ft_strncmp(red->redirection, "<<", 2))
		{
			here_doc_node = malloc(sizeof(t_here_doc));
			pipe(fd);
			here_doc_node->fd = fd[0];
			here_doc_node->red_order = i;
			here_doc_node->open = true;
			here_doc_node->next = NULL;
			here_doc_input(red->redirection + 2, fd[1], &vars->final_line);
			close(fd[1]);
			here_doc_addback(&head, here_doc_node);
		}
		red = red->next;
		i++;
	}
	return (head);
}

int	get_here_doc_fd(t_here_doc *here_doc, int red_order)
{
	t_here_doc	*tmp;

	tmp = here_doc;
	while (tmp)
	{
		if (tmp->red_order == red_order)
		{
			tmp->open = false;
			return (tmp->fd);
		}
		tmp = tmp->next;
	}
	return (-1);
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
