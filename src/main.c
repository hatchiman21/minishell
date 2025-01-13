/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 00:29:45 by yhamdan           #+#    #+#             */
/*   Updated: 2025/01/13 03:12:18 by aatieh           ###   ########.fr       */
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
	return (word);
}

// void	get_variable_len(char **env, char *line, int *j, int *i)
// {
// 	int		n;
// 	int		m;
// 	char	*var_value;

// 	n = *j;
// 	while (line[*j] != ' ' && line[*j] != '\'' && line[*j] != '"')
// 		*j++;
// 	n -= *j;
// 	m = 0;
// 	while (env[m] && ft_strncmp(env[m], line[*j], n))
// 		m++;
// 	*i += ft_strlen(env[m]) - n;
// }

char	*get_variable(char **env, char *line, int *j, int *i)
{
	int		n;
	int		m;
	char	*var_value;

	n = *j;
	while (line[*j] != ' ' && line[*j] != '\'' && line[*j] != '"')
		*j++;
	n -= *j;
	m = 0;
	while (env[m] && ft_strncmp(env[m], line[*j], n) && env[m][n + 1] != '=')
		m++;
	if (!env[m])
		return (NULL);
	var_value = ft_strdup(env[m] + n + 1);
	*i += n;
	return (var_value);
}

int	word_len_sh(char *line, int *j, char **env)
{
	int	flag_q;
	int	i;

	if (!line)
		return (NULL);
	flag_q = 0;
	while (line[*j] && (line[*j] != ' ' || flag_q))
	{
		if (line[*j] == '\'' && !flag_q)
			flag_q = 1;
		else if (line[*j] == '"' && !flag_q)
			flag_q = 2;
		else if ((line[*j] == '\'' && flag_q == 1) || (line[*j] == '"' && flag_q == 2))
			flag_q = 0;
		if (line[*j] == '$' && flag_q != 1)
			get_variable();
		else
		{
			i++;
			*j++;
		}
	}
}

char	*sh_strdup(const char *s, int j, int i)
{
	int	i;
	char *res;
	
	res = (char *)malloc(sizeof(char) * (j + 1));
	if (res == NULL)
		return (NULL);
	i = 0;
	while (i < j)
	{
		res[i] = s[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

char	*get_token_sh(char *line, int *j, char **env)
{
	int	flag_q;
	int	i;

	if (!line)
		return (NULL);
	flag_q = 0;
	i = *j;
	while (line[*j] && (line[*j] != ' ' || flag_q))
	{
		if (line[*j] == '\'' && !flag_q)
			flag_q = 1;
		if (line[*j] == '"' && !flag_q)
			flag_q = 2;
		if (line[*j] == '$' && flag_q != 1)
			get_variable(env, line, j, &i);
		i++;
		j++;
		if (line && ((line[*j] == '\'' && flag_q == 1) || (line && line[*j] == '"' && flag_q == 2)))
		{
			flag_q = 0;
			j++;
		}
	}
	return(sh_strdup(line + i, *j - i));
}

char **get_argv(char *line, t_minishell *vars, char **env)
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
		vars->argv[i] = get_token_sh(line, &j, env);
		i++;
	}
	vars->argv[i] = NULL;
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	vars;
	char		*line;

	line = NULL;
	(void)argv;
	vars.env = env;
	printf("argc is %d\n", argc);
	while (1)
	{
		line = readline(NULL);
		vars.argv = get_argv(line, &vars, env);
		free(line);
	}
	return 0;
}
