/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 01:02:20 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/15 05:41:19 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	word_end(char c)
{
	if (c == '"' || c == ' ' || c == '\'' || c == '|' || c == '\0' || c == '$')
		return (1);
	return (0);
}

int	word_check(char *line, int i)
{
	int	j;

	j = 0;
	if (line[i] && line[i] == '"')
	{
		while (line[i + j + 1] && line[i + j + 1] != '"')
			j++;
		if (!line[i + j + 1])
			exit(1);
		j += 2;
	}
	if (line[i + j] && line[i + j] == '\'')
	{
		while (line[i + j + 1] && line[i + j + 1] != '\'')
			j++;
		if (!line[i + j + 1])
			exit(1);
		j += 2;
	}
	while (line[i + j] && line[i + j] != '\'' && line[i + j] != '"' && line[i
			+ j] != ' ' && line[i + j] != '|')
		j++;
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
