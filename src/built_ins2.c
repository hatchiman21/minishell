/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 00:19:25 by yhamdan           #+#    #+#             */
/*   Updated: 2025/03/06 10:54:12 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*get_special_charecter(char *line, int *j, int exit_status, t_minishell *vars)
{
	char	*variable;
	char	*tmp;
	char	*tmp2;
	int		len;

	tmp = NULL;
	if (ft_isdigit(line[*j]))
	{
		if (line[*j] == '0')
			tmp = ft_strdup("minishell");
	}
	else
		tmp = ft_itoa(exit_status);
	remove_from_line(line, *j - 1, 2);
	if (!tmp)
		return (line);
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
	if (line[*j] == '?' || ft_isdigit(line[*j]))
		return (get_special_charecter(line, j, vars->exit_status, vars));
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
