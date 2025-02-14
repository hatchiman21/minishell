/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhamdan <yhamdan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 00:19:25 by yhamdan           #+#    #+#             */
/*   Updated: 2025/02/15 00:21:20 by yhamdan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	mod_strncmp(const char *s2, const char *s1, int y, int t)
{
	int		j;

	j = 0;
	if (s2[0] == '-')
	{
		s2++;
		y++;
	}
	while (s1[j])
		j++;
	while (s2[t])
		t++;
	if (t > j)
		return (1);
	if (j > t)
		return (0);
	t = -1;
	while (s2[++t])
	{
		if (((unsigned char)s2[t] - (unsigned char)s1[t]) > y)
			return (1);
		else if (((unsigned char)s2[t] - (unsigned char)s1[t]) < y)
			return (0);
	}
	return (0);
}


void	exit1(char *state, t_minishell *vars)
{
	if (state && mod_strncmp(state, "9223372036854775807", 0, 0))
		ft_dprintf(2, "minishell: exit: %s: numeric argument required\n", vars->argv[1]);
	else if (vars->argc <= 2)
	{
		if(state)
			vars->exit_status = ft_atoi(state);
		free_split(vars->argv, vars->argc);
		ft_free_red(vars->redirections);
		exit(1);
	}
	else
		ft_dprintf(2, "minishell: exit: too many arguments\n");
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
int	get_var_helper(char *line, int var_len, int j)
{
	return (line[var_len + j] && line[var_len + j] != ' ' && line[var_len
			+ j] != '\'' && line[var_len + j] != '"'
		&& line[var_len + j] != '|' && line[var_len + j] != '$');
}

char	*get_variable(char **env, char *line, int *j, int status)
{
	int		var_len;
	int		m;
	char	*variable;
	char	*tmp2;

	var_len = 0;
	(*j)++;
	while (get_var_helper(line, var_len, *j))
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
