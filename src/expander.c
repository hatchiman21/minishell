/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 04:41:37 by yhamdan           #+#    #+#             */
/*   Updated: 2025/01/24 20:51:34 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*dup_without_qoutes(char *s, int counter)
{
	int		i;
	int		j;
	char	*res;
	int q_flag;

	res = (char *)malloc(sizeof(char) * (counter + 1));
	if (res == NULL)
		return (NULL);
	i = 0;
	j = 0;
	q_flag = 0;
	while (j < counter)
	{
		if ((s[i] == '\'' && q_flag == 1)
			|| (s[i] == '"' && q_flag == 2))
			q_flag = 0;
		else if (s[i] == '\'' && q_flag != 2)
			q_flag = 1;
		else if (s[i] == '"' && !q_flag)
			q_flag = 2;
		if (!((s[i] == '\'' && q_flag != 2)
			|| (s[i] == '"' && q_flag != 1)))
			res[j++] = s[i];
		i++;
	}
	res[j] = '\0';
	free(s);
	return (res);
}

char *rm_qoutes(char *line)
{
	int i;
	int q_flag;
	int counter;

	i = 0;
	q_flag = 0;
	counter = 0;
	while (line[i])
	{
		if ((line[i] == '\'' && q_flag == 1)
			|| (line[i] == '"' && q_flag == 2))
			q_flag = 0;
		else if (line[i] == '\'' && q_flag != 2)
			q_flag = 1;
		else if (line[i] == '"' && !q_flag)
			q_flag = 2;
		if ((line[i] == '\'' && q_flag != 2)
			|| (line[i] == '"' && q_flag != 1))
			counter--;
		counter++;
		i++;
	}
	return (dup_without_qoutes(line, counter));
}

char	*rev_strdup(char *s, int j)
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
	free(s);
	return (res);
}

char	*expand(char *argv, t_minishell vars)
{
	int	j;
	int	q_flag;

	j = 0;
	q_flag = -1;
	while (argv[j])
	{
		if (argv[j] == '\'')
		{
			q_flag *= -1;
			j++;
		}
		if (argv[j] == '$' && q_flag == -1)
			argv = get_variable(vars.env, argv, &j);
		if (j >= (int)ft_strlen(argv))
			break;
		j++;
	}
	// printf("%s\n", argv);
	argv = rm_qoutes(argv);
	// printf("%s\n", argv);
	return (argv);
}

// int	test_expander(int argc, char **argv, char **env)
// {
// 	get_pwd(env);
// 	t_minishell	vars;

// 	vars.env = env;
// 	vars.argv = ++argv;
// 	expand(vars.argv, vars);
// 	int i = 0;
// 	while (vars.argv[i])
// 	i++;
// 	while (i >= 0)
// 	free(vars.argv[i--]);
// 	return 0;
// }
