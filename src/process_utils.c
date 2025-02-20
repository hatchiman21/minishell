/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 23:47:30 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/20 22:23:15 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	wait_for_all(t_minishell *vars)
{
	int	id;
	int	status;

	id = 1;
	while (id != -1)
	{
		id = waitpid(-1, &status, 0);
		if (id == vars->last_id)
		{
			if (WIFEXITED(status))
				vars->exit_status = WEXITSTATUS(status);
			else
				vars->exit_status = 128 + WTERMSIG(status);
		}
	}
}

int	not_child_process(char **cmd, t_minishell *vars)
{
	if (!cmd || !cmd[0])
		return (0);
	if (!ft_strncmp(cmd[0], "cd", 3) || !ft_strncmp(cmd[0], "export", 7)
		|| !ft_strncmp(cmd[0], "unset", 6) || !ft_strncmp(cmd[0], "exit", 5)
		|| !ft_strncmp(cmd[0], "env", 4) || !ft_strncmp(cmd[0], "pwd", 4)
		|| !ft_strncmp(cmd[0], "echo", 5))
	{
		vars->last_id = -2;
		return (1);
	}
	return (0);
}
