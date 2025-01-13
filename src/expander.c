/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhamdan <yhamdan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 04:41:37 by yhamdan           #+#    #+#             */
/*   Updated: 2025/01/13 05:50:46 by yhamdan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../libft/inc/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_vars
{
	int		argc;
	char	**argv;
	char	**env;
}			t_vars;


char	*rev_strdup(const char *s, int j)
{
	char	*res;
	int		i;

	res = (char *)malloc(sizeof(char) * (j + 1));
	if (res == NULL)
		return (NULL);
	i = 0;
	while (i < j)
	{
		res[i] = s[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

char	*get_variable(char **env, char *line, int *j, char *full_line)
{
	int		var_len;
	int		m;
	char	*var_value;

	var_len = 0;
	while (line[var_len] && line[var_len] != ' ' && line[var_len] != '\'' && line[var_len] != '"')
		var_len++;
	m = 0;
	while (env[m] && ft_strncmp(env[m], line + *j, var_len) && env[m][var_len + 1] != '=')
		m++;
	if (!env[m])
		return (NULL);
	var_value = ft_strdup(env[m] + var_len + 1);
	var_value = ft_strjoin(var_value, line + var_len);
	full_line = rev_strdup(full_line, *j);
	var_value = ft_strjoin(full_line, var_value);
	while (env[m][var_len++ + 1])
		*j++;
	return (var_value);
}

void	expand(char **argv, t_vars vars)
{
	int	i;
	int	j;
	int	q_flag;

	i = 0;
	while (argv[i])
	{
		j = 0;
		q_flag = 1;
		while (argv[i][j])
		{
			if (argv[i][j] == '"')
			{
				q_flag *= -1;
				j++;
			}
			if (argv[i][j] == '$' && (argv[i][j] == '\'' && !q_flag))
				argv[i] = get_variable(vars.env, argv[i] + j, &j, argv[i]);
			j++;
		}
		i++;
	}
}

int	main(int argc, char **argv, char **env)
{
	t_vars	vars;

	vars.env = env;
	vars.argv = argv;
	expand(vars.argv + 1, vars);
	int i = 0;
	while (vars.argv[i])
		printf("%s\n", vars.argv[i++]);
	return 0;
}
