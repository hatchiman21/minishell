/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhamdan <yhamdan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 00:29:45 by yhamdan           #+#    #+#             */
/*   Updated: 2025/02/15 01:56:30 by yhamdan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

// void	print_redirections(t_redirect *redirections)
// {
// 	int			i;
// 	t_redirect	*red;

// 	i = 0;
// 	red = redirections;
// 	while (red)
// 	{
// 		printf("redirection[%d]: %s\n", i, red->redirection);
// 		printf("op: %d\n", red->op);
// 		i++;
// 		if (red->next && red->op != red->next->op)
// 			i = 0;
// 		red = red->next;
// 	}
// }

// void	print_vars(t_minishell vars)
// {
// 	int	i;

// 	i = 0;
// 	printf("argc: %d\n", vars.argc);
// 	while (i < vars.argc)
// 	{
// 		printf("argv[%d]: %s\n", i, vars.argv[i]);
// 		i++;
// 	}
// 	print_redirections(vars.redirections);
// }

int	redirections(char **line, t_minishell *vars)
{
	vars->redirections = get_redirections(*line);
	if (!vars->redirections && (ft_strchr_find(vars->final_line, '>')
			|| ft_strchr_find(vars->final_line, '<')))
	{
		add_history(vars->final_line);
		free(vars->final_line);
		free(*line);
		ft_putstr_fd("minishell: redirection malloc failed\n", 2);
		return (-1);
	}
	if (ambiguous_redirect_check(vars->redirections, vars->env) == 1)
	{
		add_history(vars->final_line);
		ft_free_red(vars->redirections);
		free(vars->final_line);
		free(*line);
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	vars;
	char		*line;
	int			error_check;

	(void)argc;
	(void)argv;
	inti_set_up(&vars, env);
	while (true)
	{
		error_check = first_step(&line, &vars);
		if (error_check == 0)
			error_check = redirections(&line, &vars);
		if (error_check == 0)
			error_check = here_doc_set(line, &vars);
		if (error_check == 0)
			error_check = final_step(&line, &vars);
		if (error_check == -1)
			break ;
	}
	free_split(vars.env, -1);
	printf("exit\n");
	return (0);
}
