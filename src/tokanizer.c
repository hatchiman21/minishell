/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokanizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 03:18:42 by aatieh            #+#    #+#             */
/*   Updated: 2025/01/27 01:10:26 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	words_count_sh(char *line)
{
	int	i;
	int	j;
	int	m;
	int	word;

	i = 0;
	word = 0;
	while (line[i])
	{
		j = 0;
		while (line[i] == ' ')
			i++;
		m = i + 1;
		while (m != i + j)
		{
			m = i + j;
			j += word_check(line, i + j);
		}
		if (j != 0 || line[i] == '|')
			word++;
		if (line[i] == '|')
			i++;
		i += j;
	}
	return (word);
}

char	*get_token_sh(char *line, int *j)
{
	int	flag_q;
	int	i;

	flag_q = 0;
	i = *j;
	if (line[*j] == '|')
	{
		(*j)++;
		return (NULL);
	}
	while (line[*j] && !(line[*j] == ' ' && !flag_q))
	{
		if ((line[*j] == '\'' && flag_q == 1) || (line[*j] == '"'
				&& flag_q == 2))
			flag_q = 0;
		else if (line[*j] == '\'' && !flag_q)
			flag_q = 1;
		else if (line[*j] == '"' && !flag_q)
			flag_q = 2;
		(*j)++;
		if (line[*j] == '|')
			break ;
	}
	return (ft_substr(line, i, *j - i));
}

char	**get_argv(char *line, int argc)
{
	int		i;
	int		j;
	char	**argv;

	argv = malloc(sizeof(char *) * (argc + 1));
	i = 0;
	j = 0;
	while (i < argc)
	{
		while (line[j] && line[j] == ' ')
			j++;
		argv[i] = get_token_sh(line, &j);
		i++;
	}
	argv[i] = NULL;
	return (argv);
}
