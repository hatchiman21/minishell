/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhamdan <yhamdan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 00:17:53 by yhamdan           #+#    #+#             */
/*   Updated: 2025/02/16 01:33:38 by yhamdan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	**my_cd2(char **argv, char **env, char **tmp)
{
	char		*path;
	struct stat	sb;

	path = getcwd(NULL, 0);
	if (chdir(argv[1]) == -1)
	{
		if (stat(argv[1], &sb) == -1)
			ft_dprintf(2, "minishell: cd: %s: No such file or directory\n",
				argv[1]);
		else
			ft_dprintf(2, "minishell: cd: %s: Permission denied\n", argv[1]);
		free(path);
		return (env);
	}
	else
	{
		tmp[0] = ft_merge("OLDPWD=", path, 0, 0);
		env = export(env, tmp);
		free(*tmp);
		tmp[0] = ft_merge("PWD=", getcwd(NULL, 0), 0, 1);
		env = export(env, tmp);
		free(tmp[0]);
	}
	free(path);
	return (env);
}

void	my_cd(char **argv, char **env)
{
	char	*path;
	char	**tmp;

	tmp = (char **)malloc(2 * sizeof(char *));
	tmp[0] = NULL;
	tmp[1] = NULL;
	if (array_size(argv) == 1)
	{
		path = getcwd(NULL, 0);
		chdir(getenv("HOME"));
		tmp[0] = ft_merge("OLDPWD=", path, 0, 1);
		env = export(env, tmp);
		free(tmp[0]);
		tmp[0] = ft_merge("PWD=", getcwd(NULL, 0), 0, 1);
		env = export(env, tmp);
		free(tmp[0]);
	}
	else if (array_size(argv) == 2)
		env = my_cd2(argv, env, tmp);
	else
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
	free(tmp);
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
	int	i;
	int	t;

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
