/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 00:19:25 by yhamdan           #+#    #+#             */
/*   Updated: 2025/02/15 05:41:08 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char *exit_status1(char *line, int *j, int exit_status)
{
	char *variable;
	char *tmp2;

	remove_from_line(line, *j - 1, 2);
	tmp2 = ft_merge(ft_itoa(exit_status), line + *j - 1, 1, 0);
	line = rev_strdup(line, *j - 1);
	variable = ft_merge(line, tmp2, 1, 1);
	return (variable);
}

int get_var_helper(char *line, int var_len, int j)
{
	return (line[var_len + j] && line[var_len + j] != ' ' && line[var_len + j] != '\'' && line[var_len + j] != '"' && line[var_len + j] != '|' && line[var_len + j] != '$');
}

char *get_variable(char **env, char *line, int *j, int status)
{
	int var_len;
	int m;
	char *variable;
	char *tmp2;

	var_len = 0;
	(*j)++;
	while (get_var_helper(line, var_len, *j))
		var_len++;
	if (var_len == 1 && line[*j] == '?')
		return (exit_status1(line, j, status));
	m = 0;
	while (env[m] && (ft_strncmp(env[m], line + *j, var_len) != 0 || env[m][var_len] != '='))
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
