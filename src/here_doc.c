/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:32:13 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/11 18:11:19 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
			ft_dprintf(2,"%s %d delimited by end-of-file (wanted `%s')\n",
				"minishell: warning: here-document at line", i, stop_sign);
			break ;
		}
		if (add_line(line, final_line) == -1)
		{
			free(line);
			return (-1);
		}
		if (!ft_strncmp(line, stop_sign, ft_strlen(stop_sign))
			&& ft_strlen(line) == ft_strlen(stop_sign))
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		rl_replace_line("", 0);
		rl_on_new_line();
		i++;
	}
	if (line)
		free(line);
	return (0);
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
			if (!here_doc_node)
			{
				close_free_here_doc(&head);
				ft_putstr_fd("minishell: here_doc malloc failed\n", 2);
				return (NULL);
			}
			here_doc_addback(&head, here_doc_node);
			if (pipe(fd) == -1)
			{
				close_free_here_doc(&head);
				ft_putstr_fd("minishell: here_doc pipe failed\n", 2);
				return (NULL);
			}
			here_doc_node->fd = fd[0];
			here_doc_node->red_order = i;
			here_doc_node->open = true;
			here_doc_node->next = NULL;
			if (here_doc_input(red->redirection + 2, fd[1], &vars->final_line) == -1)
			{
				close(fd[1]);
				close_free_here_doc(&head);
				return (NULL);
			}
			close(fd[1]);
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
