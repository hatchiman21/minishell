/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 05:40:24 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/15 06:28:23 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int mod_strncmp(const char *s2, const char *s1, int y, int t)
{
	int j;

	j = 0;
	if (s2[0] == '-')
	{
		s2++;
		y++;
	}
	while (s1[j])
		j++;
	while (s2[t])
		t++;
	if (t > j)
		return (1);
	if (j > t)
		return (0);
	t = -1;
	while (s2[++t])
	{
		if (((unsigned char)s2[t] - (unsigned char)s1[t]) > y)
			return (1);
		else if (((unsigned char)s2[t] - (unsigned char)s1[t]) < y)
			return (0);
	}
	return (0);
}

int	array_size(char **array)
{
	int	size;

	size = 0;
	if (!array)
		return (0);
	while (array[size])
		size++;
	return (size);
}

int	all_digits(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_exit(char **cmd, t_minishell *vars)
{
	if (cmd[1] && mod_strncmp(cmd[1], "9223372036854775807", 0, 0))
		ft_dprintf(2, "minishell: exit: %s: numeric argument required\n",
				   vars->argv[1]);
	else if (array_size(cmd) <= 2)
	{
		vars->exit_status = 0;
		if (cmd[1])
			vars->exit_status = ft_atoi(cmd[1]);
		if (vars->argc <= 2 || !all_digits(cmd[1]))
		{
			free_split(vars->argv, vars->argc);
			close_free_here_doc(&vars->here_doc_fds);
			ft_free_red(vars->redirections);
			free_split(vars->env, -1);
			close(vars->pipefd[1]);
			close(vars->pipefd[0]);
			printf("exit\n");
			exit(vars->exit_status);
		}
	}
	else
		ft_dprintf(2, "minishell: exit: too many arguments\n");
}
