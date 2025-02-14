/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhamdan <yhamdan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 04:41:37 by yhamdan           #+#    #+#             */
/*   Updated: 2025/02/15 01:57:04 by yhamdan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
	int	q2_flag;

	j = 0;
	q_flag = -1;
	q2_flag = 1;
	while (argv[j])
	{
		if (argv[j] == '"' && q_flag == -1)
		{
			q2_flag *= -1;
			j++;
		}
		if (argv[j] == '\'' && q2_flag == 1)
		{
			q_flag *= -1;
			j++;
		}
		if (argv[j] == '$' && q_flag == -1)
			argv = get_variable(vars.env, argv, &j, vars.exit_status);
		if (j >= (int)ft_strlen(argv))
			break ;
		j++;
	}
	return (argv);
}
