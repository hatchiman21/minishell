/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 19:29:46 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/12 07:13:58 by aatieh           ###   ########.fr       */
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

int	redirectiont_text_end(char *line, int i, int j)
{
	int	q_flag;

	q_flag = 0;
	while (line[i + j])
	{
		if (!q_flag && (line[i + j] == ' ' || line[i + j] == '|'
				|| line[i + j] == '<' || line[i + j] == '>'))
			break ;
		else if (line[i + j] == '\'' && !q_flag)
			q_flag = 1;
		else if (line[i + j] == '"' && !q_flag)
			q_flag = 2;
		else if ((line[i + j] == '"' && q_flag == 2)
			|| (line[i + j] == '\'' && q_flag == 1))
			q_flag = 0;
		j++;
	}
	return (j);
}

t_redirect	*assgine_redirection(char *line, int i, int op_num, int *space_num)
{
	int			j;
	t_redirect	*redirect;

	j = 0;
	*space_num = 0;
	redirect = malloc(sizeof(t_redirect));
	if (!redirect)
		return (NULL);
	redirect->op = op_num;
	while (line[i + j] && (line[i + j] == '<' || line[i + j] == '>'))
		j++;
	while (line[i + j + *space_num] && line[i + j + *space_num] == ' ')
		(*space_num)++;
	j = redirectiont_text_end(line, i + *space_num, j);
	redirect->redirection = malloc(sizeof(char) * j + 1);
	if (!redirect->redirection)
	{
		free(redirect);
		return (NULL);
	}
	return (redirect);
}

t_redirect	*get_redirection(char *line, int i, int op_num)
{
	int			j;
	int			end;
	int			space_num;
	t_redirect	*redirect;

	j = 0;
	redirect = assgine_redirection(line, i, op_num, &space_num);
	if (!redirect)
		return (NULL);
	while (line[i + j] && (line[i + j] == '<' || line[i + j] == '>'))
	{
		redirect->redirection[j] = line[i + j];
		j++;
	}
	end = redirectiont_text_end(line, i + space_num, j);
	while (j < end)
	{
		redirect->redirection[j] = line[i + j + space_num];
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
	t_redirect	*tmp;

	if (!line)
		return (NULL);
	head = NULL;
	i = 0;
	op_num = 0;
	while (line[i])
	{
		if (line[i] == '>' || line[i] == '<')
		{
			tmp = get_redirection(line, i, op_num);
			if (!tmp)
				return (ft_free_red(head));
			redirectionadd_back(&head, tmp);
			i--;
		}
		else if (line[i] == '|')
			op_num++;
		i++;
	}
	return (head);
}
