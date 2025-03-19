/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 04:41:37 by yhamdan           #+#    #+#             */
/*   Updated: 2025/03/19 06:01:14 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*rev_strdup(char *s, int j, t_minishell *vars)
{
	char	*res;
	int		i;

	res = (char *)malloc(sizeof(char) * (j + 1));
	if (!res)
		ft_exit((char *[]){"exit", "1", NULL}, vars);
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

int	expander_helper(char *line, int q_flag, int j)
{
	return (line[j] == '$' && line[j + 1] != ' ' && q_flag == -1
		&& line[j + 1] != '\'' && line[j + 1] != '|'
		&& line[j + 1] != '\t');
}

char	*expand(char *line, t_minishell vars)
{
	int	j;
	int	q_flag;

	j = 0;
	q_flag = -1;
	while (line[j])
	{
		if (line[j] == '\'' && q_flag == -1)
			skip_qouted_line(line, &j);
		if (line[j] == '"')
			q_flag *= -1;
		if (line[j] == '$' && line[j + 1] && line[j + 1] != '|'
			&& line[j + 1] != ' ' && line[j + 1] != '\t'
			&& !(q_flag == 1 && (line[j + 1] == '\'' || line[j + 1] == '"')))
			line = get_variable(&vars, line, &j);
		if (j++ >= (int)ft_strlen(line))
			break ;
	}
	return (line);
}
