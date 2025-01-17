/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yousef <yousef@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 04:41:37 by yhamdan           #+#    #+#             */
/*   Updated: 2025/01/17 23:54:05 by yousef           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*	int	flag_q;
	int	i;

	if (!line)
		return (NULL);
	flag_q = 0;
	i = *j;
	if (line[*j] == '|')
		return (ft_substr(line, (*j)++, 1));
	while (line[*j] && !(line[*j] == ' ' && !flag_q))
	{
		if ((line[*j] == '\'' && flag_q == 1)
			|| (line[*j] == '"' && flag_q == 2))
			flag_q = 0;
		else if (line[*j] == '\'' && !flag_q)
			flag_q = 1;
		else if (line[*j] == '"' && !flag_q)
			flag_q = 2;
		(*j)++;
		if (line[*j] == '|')
			break ;
	}*/

char	*dup_without_qoutes(char *s, int counter)
{
	int		i;
	int		j;
	char	*res;
	int q_flag;

	res = (char *)malloc(sizeof(char) * (counter + 1));
	if (res == NULL)
		return (NULL);
	i = 0;
	j = 0;
	q_flag = 0;
	while (j < counter)
	{
		if ((s[i] == '\'' && q_flag == 1)
			|| (s[i] == '"' && q_flag == 2))
			q_flag = 0;
		else if (s[i] == '\'' && q_flag != 2)
			q_flag = 1;
		else if (s[i] == '"' && !q_flag)
			q_flag = 2;
		if (!((s[i] == '\'' && q_flag != 2)
			|| (s[i] == '"' && q_flag != 1)))
			res[j++] = s[i];
		i++;
	}
	res[j] = '\0';
	return (res);
}

char *rm_qoutes(char *line)
{
	int i;
	int q_flag;
	int counter;

	i = 0;
	q_flag = 0;
	counter = 0;
	while (line[i])
	{
		if ((line[i] == '\'' && q_flag == 1)
			|| (line[i] == '"' && q_flag == 2))
			q_flag = 0;
		else if (line[i] == '\'' && q_flag != 2)
			q_flag = 1;
		else if (line[i] == '"' && !q_flag)
			q_flag = 2;
		if ((line[i] == '\'' && q_flag != 2)
			|| (line[i] == '"' && q_flag != 1))
			counter--;
		counter++;
		i++;
	}
	return (dup_without_qoutes(line, counter));
}

char	*rev_strdup(char const *s, int j)
{
	char	*res;
	int		i;

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

char	*get_variable(char **env, char *line, int *j)
{
	int		var_len;
	int		m;
	char	*variable;
	char 	*tmp;
	char 	*tmp2;

	var_len = 0;
	(*j)++;
	while (line[var_len + *j] && line[var_len + *j] != ' ' && line[var_len + *j] != '\'' && line[var_len + *j] != '"' && line[var_len + *j] != '|')
		var_len++;
	m = 0;
	while (env[m] && (ft_strncmp(env[m], line + *j, var_len) != 0 || env[m][var_len] != '='))
		m++;
	if (!env[m])
		return (line);
	tmp = ft_strdup(env[m] + var_len + 1);
	tmp2 = ft_strjoin(tmp, line + var_len + *j);
	line = rev_strdup(line, *j - 1);
	variable = ft_strjoin(line, tmp2);
	while (env[m][var_len++ + 1])
		(*j)++;
	free(line);
	free(tmp);
	free(tmp2);
	return (variable);
}

void	expand(char **argv, t_minishell vars)
{
	int	i;
	int	j;
	int	q_flag;

	i = 0;
	while (argv[i])
	{
		j = 0;
		q_flag = -1;
		while (argv[i][j])
		{
			if (argv[i][j] == '\'')
			{
				q_flag *= -1;
				j++;
			}
			if (argv[i][j] == '$' && q_flag == -1)
				argv[i] = get_variable(vars.env, argv[i], &j);
			if (!argv[i][j])
				break;
			j++;
		}
		printf("%s\n", argv[i]);
		argv[i] = rm_qoutes(argv[i]);
		printf("%s\n", argv[i]);
		i++;
	}
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	vars;

	vars.env = env;
	vars.argv = ++argv;
	expand(vars.argv, vars);
	int i = 0;
	while (vars.argv[i])
	i++;
	while (i >= 0)
	free(vars.argv[i--]);
	return 0;
}
