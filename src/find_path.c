/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 18:06:10 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/14 21:18:20 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*check_status(char **cmd, char **paths, char *path)
{
	if (access(path, X_OK) == 0)
		return (path);
	if (access(path, F_OK) == 0)
	{
		ft_dprintf(2, "minishell: %s: Permission denied\n", cmd[0]);
		if (cmd[0] != path)
			free(path);
		free_split(paths, -1);
		exit(126);
	}
	return (path);
}

char	**get_all_paths(char **envp)
{
	int		i;
	char	**path;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		return (NULL);
	path = ft_split(envp[i] + 5, ':');
	return (path);
}

char	*get_final_path(char **paths, char *tmp, char **cmd)
{
	char	*path;
	char	**tmp_path;

	if (!paths || !tmp)
		return (NULL);
	tmp_path = paths;
	path = NULL;
	while (tmp_path && tmp_path[0])
	{
		path = ft_strjoin(*tmp_path, tmp);
		if (!path)
			break ;
		if (access(path, F_OK) == 0)
			return (check_status(cmd, paths, path));
		free(path);
		path = NULL;
		tmp_path++;
	}
	return (path);
}

char	*get_path(char **cmd, char **envp)
{
	char	**paths;
	char	*path;
	char	*tmp;

	if (!envp)
		return (NULL);
	if (ft_strchr(cmd[0], '/') && access(cmd[0], F_OK) == 0)
		return (ft_strdup(check_status(cmd, NULL, cmd[0])));
	tmp = ft_strjoin("/", cmd[0]);
	paths = get_all_paths(envp);
	if (!paths || !tmp)
	{
		if (tmp)
			free(tmp);
		return (NULL);
	}
	path = get_final_path(paths, tmp, cmd);
	free_all(tmp, paths);
	if (path == NULL)
	{
		ft_dprintf(2, "minishell: %s: command not found\n", cmd[0]);
		exit(127);
	}
	return (path);
}
