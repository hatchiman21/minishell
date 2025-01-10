/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 00:29:45 by yhamdan           #+#    #+#             */
/*   Updated: 2025/01/10 20:25:03 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

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
	while (line[i + j] && line[i + j] != '\'' && line[i + j] != '"' && line[i + j] != ' ')
		j++;
	return (j);
}

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
		if (j != 0)
			word++;
		i += j;
	}
	printf("wod count %d\n", word);
	return (word);
}

int	word_len_sh(char *line, int *j)
{
	int	flag_q;
	int	i;


	if (!line)
		return (NULL);
	flag_q = 0;
	i = j;
	while (line[*j] && (line[*j] != ' ' || flag_q))
	{
		while ()
	}
}

char	*get_word_sh(char *line, int *j)
{
	int	flag_q;
	int	i;


	if (!line)
		return (NULL);
	flag_q = 0;
	i = j;
	while (line[*j] && (line[*j] != ' ' || flag_q))
	{
		
	}
}

char **get_argv(char *line, t_minishell *vars)
{
	int		i;
	int		j;

	vars->argc = words_count_sh(line);
	vars->argv = malloc(sizeof(char *) * (vars->argc + 1));
	i = 0;
	j = 0;
	while (i < vars->argc)
	{
		while (line[j] && line[j] == ' ')
			j++;
		vars->argv[i] = get_word_sh(line, &j);
		i++;
	}
	vars->argv[i] = NULL;
}

int	main(int argc, char **argv)
{
	t_minishell	vars;
	char		*line;

	line = NULL;
	(void)argv;
	printf("argc is %d\n", argc);
	while (1)
	{
		line = readline(NULL);
		vars.argv = get_argv(line, &vars);
		free(line);
	}
	return 0;
}
