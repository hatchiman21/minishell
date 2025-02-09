/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 19:29:46 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/09 06:49:59 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	redirectionadd_back(t_redirect **lst, t_redirect *new)
{
	t_redirect	*tmp;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_redirect	*assgine_redirection(char *line, int i, int op_num, int *space_num)
{
	int			j;
	t_redirect	*redirect;

	j = 0;
	*space_num = 0;
	redirect = malloc(sizeof(t_redirect));
	redirect->op = op_num;
	while (line[i + j] && (line[i + j] == '<' || line[i + j] == '>'))
		j++;
	while (line[i + j + *space_num] && line[i + j + *space_num] == ' ')
		(*space_num)++;
	while (line[i + *space_num + j] && line[i + *space_num + j] != ' ')
		j++;
	redirect->redirection = malloc(sizeof(char) * j + 1);
	return (redirect);
}

t_redirect	*get_redirection(char *line, int i, int op_num)
{
	int			j;
	int			space_num;
	t_redirect	*redirect;

	j = 0;
	redirect = assgine_redirection(line, i, op_num, &space_num);
	while (line[i + j] && (line[i + j] == '<' || line[i + j] == '>'))
	{
		redirect->redirection[j] = line[i + j];
		j++;
	}
	while (line[i + space_num + j] && line[i + space_num + j] != ' ')
	{
		redirect->redirection[j] = line[i + space_num + j];
		j++;
	}
	redirect->redirection[j] = '\0';
	redirect->next = NULL;
	remove_from_line(line, i, space_num + j);
	return (redirect);
}

t_redirect	*get_redirections(char *line)
{
	int			i;
	int			op_num;
	t_redirect	*head;

	if (!line)
		return (NULL);
	head = NULL;
	i = 0;
	op_num = 0;
	while (line[i])
	{
		if (line[i] == '>' || line[i] == '<')
			redirectionadd_back(&head, get_redirection(line, i, op_num));
		if (line[i] == '|')
			op_num++;
		i++;
	}
	return (head);
}
