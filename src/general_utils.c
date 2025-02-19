/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 01:02:20 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/19 22:05:25 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	close_fds(int *fd)
{
	int	i;

	i = 0;
	while (fd[i] != -1)
		close(fd[i++]);
}

int	word_end(char c)
{
	if (c == '"' || c == ' ' || c == '\'' || c == '|' || c == '\0' || c == '$')
		return (1);
	return (0);
}

int	word_check(char *line, int i)
{
	int	j;
	int	i_init;

	j = 0;
	while (line[i + j] && line[i + j] != ' ' && line[i + j] != '|')
	{
		if (line[i + j] == '\'' || line[i + j] == '"')
		{
			i_init = i;
			skip_qouted_line(line, &i_init);
			j += i_init - i;
		}
		j++;
	}
	return (j);
}

char	**ft_array_dup(char **array)
{
	int		i;
	char	**new_array;

	if (!array)
		return (NULL);
	i = 0;
	while (array[i])
		i++;
	new_array = (char **)malloc(sizeof(char *) * (i + 1));
	if (!new_array)
		return (NULL);
	i = 0;
	while (array[i])
	{
		new_array[i] = ft_strdup(array[i]);
		if (!new_array[i])
		{
			free_split(new_array, i);
			return (NULL);
		}
		i++;
	}
	new_array[i] = NULL;
	return (new_array);
}

int	cmd_built_in(char **cmd)
{
	if (!ft_strncmp(cmd[0], "cd", 3))
		return (1);
	if (!ft_strncmp(cmd[0], "export", 7))
		return (1);
	if (!ft_strncmp(cmd[0], "unset", 6))
		return (1);
	if (!ft_strncmp(cmd[0], "exit", 5))
		return (1);
	if (!ft_strncmp(cmd[0], "env", 4))
		return (1);
	if (!ft_strncmp(cmd[0], "pwd", 4))
		return (1);
	if (!ft_strncmp(cmd[0], "echo", 5))
		return (1);
	return (0);
}
