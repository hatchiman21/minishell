/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 00:29:45 by yhamdan           #+#    #+#             */
/*   Updated: 2025/02/10 17:06:13 by aatieh           ###   ########.fr       */
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

char	*expand_all(t_minishell *vars, char *line)
{
	t_redirect	*red;

	red = vars->redirections;
	line = expand(line, *vars);
	while (red)
	{
		red->redirection = expand(red->redirection, *vars);
		red = red->next;
	}
	return (line);
}

void	handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	remove_all_qoutes(t_minishell *vars)
{
	int			i;
	t_redirect	*red;
	char		*tmp;

	i = 0;
	red = vars->redirections;
	while (vars->argv[i])
	{
		tmp = rm_qoutes(vars->argv[i]);
		free(vars->argv[i]);
		vars->argv[i] = tmp;
		i++;
	}
	while (red)
	{
		tmp = rm_qoutes(red->redirection);
		free(red->redirection);
		red->redirection = tmp;
		red = red->next;
	}
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
	print_redirections(vars.redirections);
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	vars;
	char		*line;
	int			i;

	line = NULL;
	i = 0;
	(void)argc;
	(void)argv;
	if (!isatty(0))
		return (EXIT_FAILURE);
	while (env[i])
		i++;
	vars.env = (char **)malloc(sizeof(char *) * (i + 1));
	i = -1;
	while (env[++i])
		vars.env[i] = ft_strdup(env[i]);
	vars.env[i] = NULL;
	vars.exit_status = 0;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &handle_sigint);
	vars.exit_status = 0;
	vars.here_doc_fds = NULL;
	while (1)
	{
		line = readline("~/minishell$ ");
		if (!line)
			break ;
		vars.final_line = ft_strdup(line);
		if (first_input_check(line))
		{
			free(vars.final_line);
			free(line);
			continue ;
		}
		vars.op_num = 1;
		vars.tmp_fd = STDOUT_FILENO;
		vars.redirections = get_redirections(line);
		vars.here_doc_fds = prepare_here_doc(&vars);
		add_history(vars.final_line);
		free(vars.final_line);
		line = expand_all(&vars, line);
		vars.argc = words_count_sh(line);
		vars.argv = get_argv(line, &vars);
		free(line);
		// print_vars(vars);
		// exit1(line, vars);
		remove_all_qoutes(&vars);
		print_vars(vars);
		if (vars.argc > 0)
			process(&vars);
		close_free_here_doc(&vars.here_doc_fds);
		free_split(vars.argv, vars.argc);
		ft_free_red(vars.redirections);
		if (i == -2)
			exit(1);
	}
	free_split(vars.env, i);
	printf("exit\n");
	return (0);
}
