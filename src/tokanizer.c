/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokanizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 03:18:42 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/22 20:43:17 by aatieh           ###   ########.fr       */
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
		while (line[i] == ' ' || line[i] == '\t')
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

char	*get_token_sh(char *line, int *j, t_minishell *vars)
{
	int	i;

	i = *j;
	if (line[*j] == '|')
	{
		vars->op_num++;
		(*j)++;
		return (NULL);
	}
	while (line[*j] && line[*j] != ' ' && line[*j] != '\t')
	{
		skip_qouted_line(line, j);
		(*j)++;
		if (line[*j] == '|')
			break ;
	}
	return (ft_substr(line, i, *j - i));
}

char	**get_argv(char *line, t_minishell *vars)
{
	int		i;
	int		j;
	char	**argv;

	argv = malloc(sizeof(char *) * (vars->argc + 1));
	if (!argv)
		return (NULL);
	i = 0;
	j = 0;
	while (i < vars->argc)
	{
		while (line[j] && (line[j] == ' ' || line[j] == '\t'))
			j++;
		argv[i] = get_token_sh(line, &j, vars);
		if (argv[i] == NULL && line[j - 1] != '|')
		{
			free_split(&argv, i);
			return (NULL);
		}
		i++;
	}
	argv[i] = NULL;
	return (argv);
}
