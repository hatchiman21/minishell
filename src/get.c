/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhamdan <yhamdan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 19:37:08 by yhamdan           #+#    #+#             */
/*   Updated: 2025/02/21 02:32:52 by yhamdan          ###   ########.fr       */
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
		return ;
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
	if (line[i[1]] && !env[i[0]])
	{
		tmp = malloc(sizeof(char *) * (array_size(env) + 2));
		if (!tmp)
			return (env);
		i[1] = 0;
		while (env[i[1]])
		{
			tmp[i[1]] = ft_strdup(env[i[1]]);
			free(env[i[1]]);
			i[1]++;
		}
		tmp[i[1]] = ft_strdup(line);
		tmp[i[1] + 1] = NULL;
		free(env);
		return (tmp);
	}
	ft_dprintf(2, "minishell: export: `%s': invalid identifier\n", line);
	vars->exit_status = 1;
	return (env);
}

char	**export(char **env, char **line, t_minishell *vars)
{
	int	i;
	int	j;
	int	t;

	t = -1;
	while (line[++t])
	{
		i = 0;
		j = 0;
		while (line[t] && line[t][j] && line[t][j] != '='
			&& (ft_isalpha(line[t][j]) || ft_isdigit(line[t][i])))
			j++;
		if (line[t][j] != '=')
			j = ft_strlen(line[t]);
		while (line[t] && line[t][j] && env[i] && ft_strncmp(env[i], line[t],
				j) != 0)
			i++;
		if (line[t][j] && env[i])
		{
			free(env[i]);
			env[i] = ft_strdup(line[t]);
		}
		env = export2(env, line[t], (int []){i, j}, vars);
	}
	return (env);
}
