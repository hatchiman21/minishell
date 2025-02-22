/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 00:29:45 by yhamdan           #+#    #+#             */
/*   Updated: 2025/02/22 02:47:56 by aatieh           ###   ########.fr       */
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
		g_ctrl_c = 0;
		if (error_check == 0)
			error_check = redirections(&line, &vars);
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
