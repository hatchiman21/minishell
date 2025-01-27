/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhamdan <yhamdan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 19:37:08 by yhamdan           #+#    #+#             */
/*   Updated: 2025/01/27 04:13:05 by yhamdan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	gets(char *line, char **env, t_minishell vars)
{
	int i;

	i = 0;
	if (vars.argv[0] && ft_strncmp(vars.argv[0], "env\0", 4) == 0)
		while (env[i])
			printf("%s\n", env[i++]);
	i = 0;
	while (env[i] && ft_strncmp(env[i], "PWD=", 4) != 0)
		i++;
	if (env[i] && vars.argv[0] && ft_strncmp(vars.argv[0], "pwd\0", 4) == 0)
		printf("%s\n", env[i] + 4);
	if (vars.argv[0] && ft_strncmp(vars.argv[0], "exit", 5) == 0)
	{
		free_split(vars.argv, vars.argc);
		ft_free_lst(vars.redirections);
		free(line);
		exit(1);
	}
}

char	*get_variable(char **env, char *line, int *j)
{
	int		var_len;
	int		m;
	char	*variable;
	char	*tmp2;

	var_len = 0;
	(*j)++;
	while (line[var_len + *j] && line[var_len + *j] != ' ' && line[var_len
		+ *j] != '\'' && line[var_len + *j] != '"' && line[var_len + *j] != '|'
		&& line[var_len + *j] != '$')
		var_len++;
	m = 0;
	while (env[m] && (ft_strncmp(env[m], line + *j, var_len) != 0
			|| env[m][var_len] != '='))
		m++;
	(*j) -= 2;
	remove_from_line(line, *j + 1, var_len + 1);
	if (!env[m])
		return (line);
	tmp2 = ft_merge(ft_strdup(env[m] + var_len + 1), line + *j + 1, 1, 0);
	line = rev_strdup(line, *j + 1);
	variable = ft_merge(line, tmp2, 1, 1);
	while (env[m][var_len++ + 1])
		(*j)++;
	return (variable);
}

void	export(char **env, char *line)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (line[j] != '=')
		j++;
	while (ft_strncmp(env[i], line, j) != 0)
		i++;
	if (env[i])
	{
		free(env[i]);
		env[i] = ft_strdup(line);
	}
}