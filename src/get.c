/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yousef <yousef@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 19:37:08 by yhamdan           #+#    #+#             */
/*   Updated: 2025/02/10 08:49:27 by yousef           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	exit1(char *line, t_minishell vars)
{
	if (vars.argv[0] && ft_strncmp(vars.argv[0], "exit", 5) == 0)
	{
		free_split(vars.argv, vars.argc);
		ft_free_red(vars.redirections);
		free(line);
		exit(1);
	}
}

char	*exit_status1(char *line, int *j, int exit_status)
{
	char	*variable;
	char	*tmp2;

	remove_from_line(line, *j - 1, 2);
	tmp2 = ft_merge(ft_itoa(exit_status), line + *j - 1, 1, 0);
	line = rev_strdup(line, *j - 1);
	variable = ft_merge(line, tmp2, 1, 1);
	return (variable);
}

char	*get_variable(char **env, char *line, int *j, int status)
{
	int		var_len;
	int		m;
	char	*variable;
	char	*tmp2;

	var_len = 0;
	(*j)++;
	while (line[var_len + *j] && line[var_len + *j] != ' ' && line[var_len
			+ *j] != '\'' && line[var_len + *j] != '"'
		&& line[var_len + *j] != '|' && line[var_len + *j] != '$')
		var_len++;
	if (var_len == 1 && line[*j] == '?')
		return (exit_status1(line, j, status));
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

char	**export(char **env, char *line)
{
	int		i;
	int		j;
	char	**tmp;

	i = 0;
	j = 0;
	while (line[j] && line[j] != '=')
		j++;
	while (line[j] && env[i] && ft_strncmp(env[i], line, j) != 0)
		i++;
	if (line[j] && env[i])
	{
		free(env[i]);
		env[i] = ft_strdup(line);
		return (env);
	}
	else if (line[j] && !env[i])
	{
		j = 0;
		while (env[j])
				j++;
		tmp = (char **)malloc(sizeof(char *) * (j + 2));
		if (!tmp)
			return (NULL);
		j = 0;
		while (env[j])
		{
 		   tmp[j] = ft_strdup(env[j]); 
 		   free(env[j]); 
 		   j++;
		}
		tmp[j] = ft_strdup(line); 
		tmp[j + 1] = NULL;
		free(env); 
		return (tmp);
	}
	return (env);
}
