/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yousef <yousef@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 00:19:25 by yhamdan           #+#    #+#             */
/*   Updated: 2025/02/21 16:48:23 by yousef           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*get_exit_status(char *line, int *j, int exit_status)
{
	char	*variable;
	char	*tmp;
	char	*tmp2;

	remove_from_line(line, *j - 1, 2);
	tmp = ft_itoa(exit_status);
	tmp2 = ft_merge(tmp, line + *j - 1, 1, 0);
	tmp = rev_strdup(line, *j - 1);
	line = tmp;
	variable = ft_merge(line, tmp2, 1, 1);
	return (variable);
}

int	get_var_helper(char *line, int *var_len, int j)
{
	if (line[j] == '?')
	{
		(*var_len)++;
		return (0);
	}
	return (line[*var_len + j] && line[*var_len + j] != ' ' && line[*var_len
			+ j] != '\'' && line[*var_len + j] != '"' && line[*var_len + j] != '|'
		&& line[*var_len + j] != '$');
}

char	*get_variable(char **env, char *line, int *j, int status)
{
	int		var_len;
	int		m;
	char	*variable;
	char	*tmp2;

	var_len = 0;
	(*j)++;
	while (get_var_helper(line, &var_len, *j))
		var_len++;
	if (var_len == 1 && line[*j] == '?')
		return (get_exit_status(line, j, status));
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

int	env(char **env, char **cmd)
{
	int	i;

	i = 0;
	if (cmd[1])
	{
		ft_putstr_fd("minishell: env: too many arguments\n", 2);
		exit(1);
	}
	while (env[i])
	{
		ft_putstr_fd(env[i], 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
	return (0);
}

int	pwd(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	ft_putstr_fd(path, 1);
	ft_putstr_fd("\n", 1);
	free(path);
	return (0);
}
