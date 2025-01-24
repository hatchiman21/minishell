/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhamdan <yhamdan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 19:37:08 by yhamdan           #+#    #+#             */
/*   Updated: 2025/01/24 19:37:51 by yhamdan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

void	get_pwd(char **env)
{
	int i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PWD=", 4) != 0)
		i++;
	if (env[i])
		printf("%s\n", env[i] + 4);
}

void	get_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		printf("%s\n", env[i]);
}