/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhamdan <yhamdan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 00:19:25 by yhamdan           #+#    #+#             */
/*   Updated: 2025/02/22 01:48:08 by yhamdan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*get_exit_status(char *line, int *j, int exit_status, t_minishell *vars)
{
	char	*variable;
	char	*tmp;
	char	*tmp2;
	int		len;

	remove_from_line(line, *j - 1, 2);
	tmp = ft_itoa(exit_status);
	len = ft_strlen(tmp);
	tmp2 = ft_merge(tmp, line + *j - 1, 1, 0);
	tmp = rev_strdup(line, *j - 1, vars);
	line = tmp;
	(*j) += len - 2;
	variable = ft_merge(line, tmp2, 1, 1);
	return (variable);
}

int	get_var_helper(char *line, int var_len, int j)
{
	return (line[var_len + j] && line[var_len + j] != ' ' && line[var_len
			+ j] != '\'' && line[var_len + j] != '"' && line[var_len + j] != '|'
		&& line[var_len + j] != '$' && line[var_len + j] != '\t' && line[
			var_len + j] != '=');
}

char	*get_variable(t_minishell *vars, char *line, int *j)
{
	int		var_len;
	int		m;
	char	*variable;
	char	*tmp2;

	var_len = 0;
	(*j)++;
	if (line[*j] == '?')
		return (get_exit_status(line, j, vars->exit_status, vars));
	while (get_var_helper(line, var_len, *j))
		var_len++;
	m = 0;
	while (vars->env[m] && (ft_strncmp(vars->env[m], line + *j, var_len) != 0
			|| vars->env[m][var_len] != '='))
		m++;
	(*j) -= 2;
	remove_from_line(line, *j + 1, var_len + 1);
	if (!vars->env[m])
		return (line);
	tmp2 = ft_merge(ft_strdup(vars->env[m] + var_len + 1), line + *j + 1, 1, 0);
	line = rev_strdup(line, *j + 1, vars);
	variable = ft_merge(line, tmp2, 1, 1);
	while (vars->env[m][var_len++ + 1])
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
