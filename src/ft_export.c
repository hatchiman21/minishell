/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 19:37:08 by yhamdan           #+#    #+#             */
/*   Updated: 2025/03/07 18:18:21 by aatieh           ###   ########.fr       */
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
		cmd_error_handling(vars);
	i = -1;
	while (++i < len)
		sorted[i] = env[i];
	sorted[len] = NULL;
	print_sorted(sorted, len);
}

char	**export2(char **env, char *line, int *i, t_minishell *vars)
{
	char	**tmp;

	if (i[1] > -1 && i[0] != -1 && !env[i[0]])
	{
		tmp = malloc(sizeof(char *) * (array_size(env) + 2));
		if (!tmp)
			cmd_error_handling(vars);
		i[1] = -1;
		while (env[++i[1]])
			tmp[i[1]] = env[i[1]];
		tmp[i[1]] = ft_strdup(line);
		if (!tmp[i[1]])
			free(tmp);
		if (!tmp)
			cmd_error_handling(vars);
		tmp[i[1] + 1] = NULL;
		free(env);
		return (tmp);
	}
	if (i[1] == -1)
		ft_dprintf(2, "minishell: export: `%s': invalid identifier\n", line);
	if (i[1] == -1)
		vars->exit_status = 1;
	return (env);
}

void	name_check(char *line, int *j)
{
	if (line[0] == '=' || ft_isdigit(line[0]))
	{
		*j = -1;
		return ;
	}
	while (line[*j] && line[*j] != '=')
	{
		if (!ft_isalnum(line[*j]) && line[*j] != '_')
		{
			*j = -1;
			return ;
		}
		(*j)++;
	}
	if (!line[*j])
		*j = -2;
}

char	**export(char **env, char **line, t_minishell *vars, int t)
{
	int	i;
	int	j;

	while (line[++t])
	{
		i = 0;
		j = 0;
		name_check(line[t], &j);
		while (j > -1 && line[t] && env[i]
			&& (ft_strncmp(env[i], line[t], j + 1) != 0))
			i++;
		if (j > -1 && env[i])
		{
			free(env[i]);
			env[i] = ft_strdup(line[t]);
			i = -1;
		}
		env = export2(env, line[t], (int []){i, j}, vars);
	}
	return (env);
}
