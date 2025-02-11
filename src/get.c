/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 19:37:08 by yhamdan           #+#    #+#             */
/*   Updated: 2025/02/11 07:51:12 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	exit1(char *line, t_minishell *vars)
{
	if (vars->argc > 2 )
	{
		free_split(vars->argv, vars->argc);
		ft_free_red(vars->redirections);
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

char	**unset(char **env, char *line)
{
	int		i;
	int		j;
	char	**tmp;

	i = 0;
	j = 0;
	while (env[i] && ft_strncmp(env[i], line, ft_strlen(line)) != 0)
		i++;
	if (env[i])
	{
		while (env[j])
			j++;
		tmp = (char **)malloc(sizeof(char *) * j);
		if (!tmp)
			return (NULL);
		j = 0;
		while (env[j + 1])
		{
			if (j < i)
				tmp[j] = ft_strdup(env[j]);
			if (j >= i)
				tmp[j] = ft_strdup(env[j + 1]);
			free(env[j]);
			j++;
		}
		free(env[j]);
		tmp[j] = NULL;
		free(env);
		return (tmp);
	}
	ft_dprintf(2, "minishell: unset: `%s': not a valid identifier\n", line);
	return (env);
}

void	my_cd(t_minishell *vars)
{
	char	*path;
	char	*tmp;

	if (vars->argc == 1)
	{
		path = getcwd(NULL, 0);
		chdir(getenv("HOME"));
		tmp = ft_merge("OLDPWD=", path, 0, 1);
		vars->env = export(vars->env, tmp);
		path = ft_merge("PWD=", getcwd(NULL, 0), 0, 1);
		vars->env = export(vars->env, path);
		free(path);
		free(tmp);
	}
	else if (vars->argc == 2)
	{
		path = getcwd(NULL, 0);
		if (chdir(vars->argv[1]) == -1)
			ft_dprintf(2, "minishell: cd: %s: No such file or directory\n", vars->argv[1]);
		else
		{
			tmp = ft_merge("OLDPWD=", path, 0, 0);
			vars->env = export(vars->env, tmp);
			free(tmp);
			tmp = ft_merge("PWD=", getcwd(NULL, 0), 0, 1);
			vars->env = export(vars->env, tmp);
			free(tmp);
		}
		free(path);
	}
	else
		write(2, "minishell: cd: too many arguments\n", 34);
}