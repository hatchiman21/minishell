/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 23:04:41 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/10 00:40:00 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	error_msg(char *msg)
{
	ft_dprintf(2, "minishell: %s\n", msg);
	
	exit(1);
}

void	errors(int	error, t_minishell *vars)
{
	(void)error;
	(void)vars;
	free_split(vars->env, -1);
}
