/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhamdan <yhamdan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 04:41:37 by yhamdan           #+#    #+#             */
/*   Updated: 2025/01/15 21:07:51 by yhamdan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"


char	*rev_strdup(const char *s, int j)
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

	var_len = 0;
	(*j)++;
	while (line[var_len + *j] && line[var_len + *j] != ' ' && line[var_len + *j] != '\'' && line[var_len + *j] != '"' && line[var_len + *j] != '|')
		var_len++;
	m = 0;
	while (env[m] && (ft_strncmp(env[m], line + *j, var_len) != 0 || env[m][var_len] != '='))
		m++;
	if (!env[m])
		return (line);
	variable = ft_strdup(env[m] + var_len + 1);
	variable = ft_strjoin(variable, line + var_len + *j);
	line = rev_strdup(line, *j - 1);
	variable = ft_strjoin(line, variable);
	while (env[m][var_len++ + 1])
		(*j)++;
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
		//printf("%s\n", argv[i]);
		while (argv[i][j])
		{
			if (argv[i][j] == '\'')
			{
				q_flag *= -1;
				j++;
			}
			if (argv[i][j] == '$' && q_flag == -1)
				argv[i] = get_variable(vars.env, argv[i], &j);
			j++;
		}
		i++;
	}
}

// int	main(int argc, char **argv, char **env)
// {
// 	t_minishell	vars;

// 	vars.env = env;
// 	vars.argv = ++argv;
// 	expand(vars.argv, vars);
// 	int i = 0;
// 	while (vars.argv[i])
// 		printf("%s\n", vars.argv[i++]);
// 	return 0;
// }
