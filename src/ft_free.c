/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 19:32:27 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/22 20:47:13 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	*ft_free_red(t_redirect **lst)
{
	t_redirect	*tmp;

	while ((*lst))
	{
		tmp = (*lst);
		(*lst) = (*lst)->next;
		free(tmp->content);
		free(tmp);
	}
	(*lst) = NULL;
	return (NULL);
}

void	ft_free(t_minishell *vars)
{
	if (vars->line)
		free(vars->line);
	if (vars->argv)
		free_split(&vars->argv, vars->argc);
	if (vars->redirections)
		ft_free_red(&vars->redirections);
	close_fds((int [4]){vars->std_in, vars->std_out, vars->pipefd[0], -1});
	close_free_here_doc(&vars->here_doc_fds);
	if (vars->env)
		free_split(&vars->env, -1);
}

void	cmd_error_handling(t_minishell *vars)
{
	wait_for_all(vars);
	ft_putstr_fd("minishell: malloc failed inside a command\n", 2);
	ft_free(vars);
	exit(1);
}
