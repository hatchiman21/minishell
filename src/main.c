/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 00:29:45 by yhamdan           #+#    #+#             */
/*   Updated: 2025/02/19 21:52:17 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	g_ctrl_c = 0;

void	handle_sigint(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', STDIN_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_ctrl_c = 1;
}

void	print_redirections(t_redirect *redirections)
{
	int			i;
	t_redirect	*red;

	i = 0;
	red = redirections;
	while (red)
	{
		printf("redirection[%d]: %s\n", i, red->redirection);
		printf("op: %d\n", red->op);
		i++;
		if (red->next && red->op != red->next->op)
			i = 0;
		red = red->next;
	}
}

void	print_vars(t_minishell vars)
{
	int	i;

	i = 0;
	printf("argc: %d\n", vars.argc);
	while (i < vars.argc)
	{
		printf("argv[%d]: %s\n", i, vars.argv[i]);
		i++;
	}
	// print_redirections(vars.redirections);
}

int	redirection_error(char *final_line, t_redirect *red)
{
	int	i;

	i = 0;
	if (red)
		return (0);
	while (final_line[i])
	{
		skip_qouted_line(final_line, &i);
		if (final_line[i] == '>' || final_line[i] == '<')
			return (1);
		i++;
	}
	return (0);
}

int	redirections(char **line, t_minishell *vars)
{
	vars->redirections = get_redirections(*line);
	if (redirection_error(vars->final_line, vars->redirections))
	{
		free(vars->final_line);
		free(*line);
		ft_putstr_fd("minishell: redirection malloc failed\n", 2);
		return (-1);
	}
	if (ambiguous_redirect_check(vars->redirections, vars->env) == 1)
	{
		ft_free_red(vars->redirections);
		free(vars->final_line);
		free(*line);
		vars->exit_status = 1;
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
	inti_set_up(&vars, env, &g_ctrl_c);
	while (true)
	{
		error_check = first_step(&line, &vars);
		g_ctrl_c = 0;
		if (error_check == 0)
			error_check = redirections(&line, &vars);
		// print_redirections(vars.redirections);
		if (error_check == 0)
			error_check = here_doc_set(line, &vars);
		if (error_check == 0)
			error_check = final_step(&line, &vars);
		if (error_check == -1)
			break ;
	}
	close(vars.std_in);
	close(vars.std_out);
	free_split(vars.env, -1);
	printf("exit\n");
	return (0);
}
