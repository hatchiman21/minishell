/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_shlvl.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 01:45:45 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/22 20:42:40 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	get_shlvl(char *old_shlvl)
{
	int	shlvl;

	if (!old_shlvl)
		return (0);
	shlvl = ft_atoi(old_shlvl);
	if (shlvl >= 99)
	{
		ft_dprintf(2, "%s (%d) too high, resetting to 1\n",
			"minishell: warning: shell level", shlvl);
		shlvl = 0;
	}
	else if (shlvl < 0)
		shlvl = -1;
	return (shlvl);
}

void	increase_shlvl(char **env)
{
	int		shlvl;
	char	*new_shlvl;
	int		i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "SHLVL=", 6) != 0)
		i++;
	shlvl = get_shlvl(env[i] + 6);
	new_shlvl = ft_itoa(shlvl + 1);
	if (!new_shlvl)
	{
		ft_putstr_fd("minishell: SHLVL malloc failed\n", 2);
		free_split(&env, -1);
		exit (EXIT_FAILURE);
	}
	new_shlvl = ft_merge("SHLVL=", new_shlvl, 0, 1);
	if (!new_shlvl)
	{
		ft_putstr_fd("minishell: SHLVL malloc failed\n", 2);
		free_split(&env, -1);
		exit (EXIT_FAILURE);
	}
	free(env[i]);
	env[i] = new_shlvl;
}
