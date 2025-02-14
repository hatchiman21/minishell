/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhamdan <yhamdan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 19:37:08 by yhamdan           #+#    #+#             */
/*   Updated: 2025/02/15 01:25:06 by yhamdan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"


char	**export2(char **env, char *line, int i, int j)
{
	char	**tmp;

	if (line[j] && !env[i])
	{
		j = 0;
		while (env[j])
			j++;
		tmp = (char **)malloc(sizeof(char *) * (j + 2));
		if (!tmp)
			return (env);
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
	if (!env[i])
		ft_dprintf(2, "minishell: export: `%s': invalid identifier\n", line);
	return (env);
}

char	**export(char **env, char **line)
{
	int	i;
	int	j;
	int	t;

	t = 0;
	while (line[t])
	{
		i = 0;
		j = 0;
		while (line[t] && line[t][j] && line[t][j] != '=' && ft_isalpha(line[t][j]))
			j++;
		if (line[t][j] != '=')
			j = ft_strlen(line[t]);
		while (line[t] && line[t][j] && env[i] && ft_strncmp(env[i], line[t], j) != 0)
			i++;
		if (line[t][j] && env[i])
		{
			free(env[i]);
			env[i] = ft_strdup(line[t]);
		}
		env = export2(env, line[t], i, j);
		t++;
	}
	return (env);
}

char	**unset2(char **env, int i)
{
	int		j;
	char	**tmp;

	j = 0;
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

char	**unset(char **env, char **line)
{
	int i;
	int t;

	t = 0;
	while (line[t])
	{
		i = 0;
		while (env[i] && ft_strncmp(env[i], line[t], ft_strlen(line[t])) != 0)
			i++;
		if (env[i])
			env = unset2(env, i);
		else
			ft_dprintf(2, "minishell: unset: `%s': not a valid identifier\n",
				line[t]);
		t++;
	}
	return (env);
}