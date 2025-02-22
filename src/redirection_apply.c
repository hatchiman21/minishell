/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_apply.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 14:52:02 by aatieh            #+#    #+#             */
/*   Updated: 2025/02/22 03:19:24 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*remove_edge_spaces(char *line)
{
	int	i;
	int	j;

	i = ft_strlen(line) - 1;
	while (i >= 0
		&& (line[i] == ' ' || line[i] == '\t'))
	{
		line[i] = '\0';
		i--;
	}
	i = skip_red_sign(line);
	j = i;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	while (line[i])
	{
		line[j] = line[i];
		i++;
		j++;
	}
	line[j] = '\0';
	return (line);
}

// int	ft_calculate_flag(t_redirect *red, int i, int q_flag)
// {
// 	if (red->redirection[i] == '"' && !q_flag)
// 		q_flag = 1;
// 	else if (red->redirection[i] == '\'' && !q_flag)
// 		q_flag = 2;
// 	else if ((red->redirection[i] == '"' && q_flag == 1)
// 		|| (red->redirection[i] == '\'' && q_flag == 2))
// 		q_flag = 0;
// 	return (q_flag);
// }

t_redirect	*get_op_red(t_redirect *red, int op)
{
	while (red && red->op < op)
		red = red->next;
	return (red);
}

int	skip_red_sign(char *line)
{
	int	i;

	i = 0;
	while (line[i] == '>' || line[i] == '<')
		i++;
	return (i + 1);
}

int	apply_redirection(t_minishell *vars, int cur_op)
{
	t_redirect	*red;
	int			red_order;

	red_order = 0;
	red = get_op_red(vars->redirections, cur_op);
	while (red && red->op == cur_op)
	{
		red->redirection = remove_edge_spaces(red->redirection);
		if (ambiguous_check(red->redirection) || !skip_red_sign(red->redirection) || open_file(vars, red, red_order) == -1)
			return (0);
		red_order++;
		red = red->next;
	}
	if (cur_op != vars->op_num - 1)
		if (dup2(vars->pipefd[1], STDOUT_FILENO) == -1)
			ft_dprintf(2, "minishell: pipe dup2 failed\n");
	if (cur_op != 0)
	{
		if (dup2(vars->tmp_fd, STDIN_FILENO) == -1)
			ft_dprintf(2, "minishell: pipe dup2 failed\n");
		close(vars->tmp_fd);
	}
	close(vars->pipefd[1]);
	return (1);
}

int	intial_red(t_minishell *vars, int *cur_op)
{
	if (*cur_op != 0)
		vars->tmp_fd = vars->pipefd[0];
	if (pipe(vars->pipefd) == -1)
	{
		ft_dprintf(2, "minishell: child pipe failed\n");
		*cur_op = -1;
		return (-1);
	}
	return (0);
}
