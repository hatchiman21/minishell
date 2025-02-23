/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 00:17:53 by yhamdan           #+#    #+#             */
/*   Updated: 2025/02/23 15:07:47 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	**my_cd2(char **argv, char **env, char **tmp, t_minishell *vars)
{
	*tmp = ft_merge("OLDPWD=", getcwd(NULL, 0), 0, 1);
	if (chdir(argv[1]) != 0)
	{
		free(*tmp);
		ft_dprintf(2, "minishell: cd: %s: %s\n", argv[1], strerror(errno));
		vars->exit_status = 1;
		return (env);
	}
	else
	{
		env = export(env, tmp, vars, -1);
		free(*tmp);
		*tmp = ft_merge("PWD=", getcwd(NULL, 0), 0, 1);
		env = export(env, tmp, vars, -1);
		free(*tmp);
	}
	return (env);
}

void	my_cd(char **argv, char **env, t_minishell *vars)
{
	char	**tmp;

	tmp = (char **)malloc(2 * sizeof(char *));
	if (!tmp)
		ft_exit((char *[]){"exit", "1", NULL}, vars);
	tmp[1] = NULL;
	if (array_size(argv) == 1)
	{
		chdir(getenv("HOME"));
		tmp[0] = ft_merge("OLDPWD=", getcwd(NULL, 0), 0, 1);
		env = export(env, tmp, vars, -1);
		free(tmp[0]);
		tmp[0] = ft_merge("PWD=", getcwd(NULL, 0), 0, 1);
		env = export(env, tmp, vars, -1);
		free(tmp[0]);
	}
	else if (array_size(argv) == 2)
		env = my_cd2(argv, env, tmp, vars);
	else
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		vars->exit_status = 1;
	}
	free(tmp);
}

char	**unset2(char **env, int i, t_minishell *vars)
{
	int		j;
	char	**tmp;

	j = 0;
	while (env[j])
		j++;
	tmp = (char **)malloc(sizeof(char *) * j);
	if (!tmp)
		ft_exit((char *[]){"exit", "1", NULL}, vars);
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

char	**unset(char **env, char **line, t_minishell *vars)
{
	int	i;
	int	t;

	t = 0;
	vars->exit_status = 0;
	while (line[t])
	{
		i = 0;
		while (env[i] && ft_strncmp(env[i], line[t], ft_strlen(line[t])) != 0)
			i++;
		if (env[i])
			env = unset2(env, i, vars);
		t++;
	}
	return (env);
}
