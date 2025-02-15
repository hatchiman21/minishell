/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 19:32:27 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/15 21:04:36 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	*ft_free_red(t_redirect *lst)
{
	t_redirect	*tmp;

	while (lst)
	{
		tmp = lst;
		lst = lst->next;
		free(tmp->redirection);
		free(tmp);
	}
	return (NULL);
}

void	ft_free(t_minishell *vars)
{
	free_split(vars->argv, vars->argc);
	ft_free_red(vars->redirections);
	close_fds((int [4]){vars->std_in, vars->std_out, vars->pipefd[0], -1});
	close_free_here_doc(&vars->here_doc_fds);
	free_split(vars->env, -1);
}
