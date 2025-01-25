/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 19:32:27 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/24 19:32:48 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_split(char **split, int num)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (i < num)
		free(split[i++]);
	free(split);
}

void	ft_free_lst(t_redirect *lst)
{
	t_redirect	*tmp;

	while (lst)
	{
		tmp = lst;
		lst = lst->next;
		free(tmp->redirection);
		free(tmp);
	}
}
