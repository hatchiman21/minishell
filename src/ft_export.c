/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 19:37:08 by yhamdan           #+#    #+#             */
/*   Updated: 2025/02/22 19:14:15 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	print_sorted(char **sorted, int len)
{
	int		i;
	int		j;
	char	*tmp;

	i = -1;
	while (++i < len - 1)
	{
		j = -1;
		while (++j < len - i - 1)
		{
			if (ft_strncmp(sorted[j], sorted[j + 1], ft_strlen(sorted[j])) > 0)
			{
				tmp = sorted[j];
				sorted[j] = sorted[j + 1];
				sorted[j + 1] = tmp;
			}
		}
	}
	i = 0;
	while (i < len)
		printf("declare -x %s\n", sorted[i++]);
	free(sorted);
}

void	export3(char **env, t_minishell *vars, int len)
{
	int		i;
	char	**sorted;

	vars->exit_status = 0;
	sorted = (char **)malloc(sizeof(char *) * (len + 1));
	if (!sorted)
		ft_exit((char *[]){"exit", "1", NULL}, vars);
	i = -1;
	while (++i < len)
		sorted[i] = env[i];
	sorted[len] = NULL;
	print_sorted(sorted, len);
}

char	**export2(char **env, char *line, int *i, t_minishell *vars)
{
	char	**tmp;

	vars->exit_status = 0;
	if (i[1] != -1 && i[0] != -1 && !env[i[0]])
	{
		tmp = malloc(sizeof(char *) * (array_size(env) + 2));
		if (!tmp)
			ft_exit((char *[]){"exit", "1", NULL}, vars);
		i[1] = -1;
		while (env[++i[1]])
			tmp[i[1]] = env[i[1]];
		tmp[i[1]] = ft_strdup(line);
		tmp[i[1] + 1] = NULL;
		free(env);
		return (tmp);
	}
	if (i[0] != -1 || i[1] == -1)
		ft_dprintf(2, "minishell: export: `%s': invalid identifier\n", line);
	if (i[0] != -1 || i[1] == -1)
		vars->exit_status = 1;
	return (env);
}

char	**export(char **env, char **line, t_minishell *vars, int t)
{
	int	i;
	int	j;

	while (line[++t])
	{
		i = 0;
		j = 0;
		while (line[t] && line[t][j] && line[t][j] != '='
			&& ft_isalnum(line[t][j]))
			j++;
		if (line[t][0] == '=' || ft_isdigit(line[t][0])
			|| (line[t][j] != '=' && line[t][j]))
			j = -1;
		while (j != -1 && line[t] && env[i]
				&& (ft_strncmp(env[i], line[t], j) != 0))
			i++;
		if (j != -1 && env[i])
		{
			free(env[i]);
			env[i] = ft_strdup(line[t]);
			i = -1;
		}
		env = export2(env, line[t], (int []){i, j}, vars);
	}
	return (env);
}
