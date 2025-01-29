/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 19:32:27 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/29 20:12:26 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
