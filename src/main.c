/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 00:29:45 by yhamdan           #+#    #+#             */
/*   Updated: 2025/01/27 04:35:21 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	new_word_start(char c)
{
	if (c == '"' || c == ' ' || c == '\'' || c == '|')
		return (1);
	return (0);
}

int	word_check(char *line, int i)
{
	int	j;

	j = 0;
	if (line[i] && line[i] == '"')
	{
		while (line[i + j + 1] && line[i + j + 1] != '"')
			j++;
		if (!line[i + j + 1])
			exit(1);
		j += 2;
	}
	if (line[i + j] && line[i + j] == '\'')
	{
		while (line[i + j + 1] && line[i + j + 1] != '\'')
			j++;
		if (!line[i + j + 1])
			exit(1);
		j += 2;
	}
	while (line[i + j] && line[i + j] != '\'' && line[i + j] != '"' && line[i
		+ j] != ' ' && line[i + j] != '|')
		j++;
	return (j);
}

int	count_char(char *line, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (line[i])
	{
		if (line[i] == c)
			count++;
		i++;
	}
	return (count);
}

void	expand_all(t_minishell *vars)
{
	t_redirect	*red;
	int			i;

	i = 0;
	red = vars->redirections;
	while (vars->argv[i])
	{
		vars->argv[i] = expand(vars->argv[i], *vars);
		i++;
	}
	while (red)
	{
		red->redirection = expand(red->redirection, *vars);
		red = red->next;
	}
}

void	handle_sigquit(int sig)
{
	sig++;
	sig--;
}

void	handle_sigint(int sig)
{
	printf("\n~/minishell$ ");
	sig++;
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	vars;
	char		*line;

	line = NULL;
	(void)argc;
	(void)argv;
	vars.env = env;
	while (1)
	{
		signal(SIGQUIT, &handle_sigquit);
		signal(SIGINT, &handle_sigint);
		line = readline(NULL);
		if (!line)
			break ;
		vars.op_num = 1;
		vars.std_in = dup(STDIN_FILENO);
		vars.std_out = dup(STDOUT_FILENO);
		vars.redirections = get_redirections(line);
		vars.argc = words_count_sh(line);
		vars.argv = get_argv(line, &vars);
		expand_all(&vars);
		// gets(line, vars.env, vars);
		if (vars.argc > 0)
			process(&vars);
		else
			printf("\n");
		wait_for_all();
		//if (ft_strncmp(vars.argv[0], "export", 7) == 0)
		//	export(vars.env, vars.argv[1]);
		// for (int i = 0; i < vars.argc; i++)
		// 	ft_printf("argv %d is %s\n" , i, vars.argv[i]);
		// for (t_redirect *red = vars.redirections; red; red = red->next)
		// 	ft_printf("op num: %d, redirection: %s\n", red->op,
		// 		red->redirection);
		free_split(vars.argv, vars.argc);
		ft_free_lst(vars.redirections);
		free(line);
	}
	printf("exit\n");
	return (0);
}
