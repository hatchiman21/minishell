/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 00:29:45 by yhamdan           #+#    #+#             */
/*   Updated: 2025/02/08 23:44:13 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int s_flag = 0;

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

void	handle_sigquit(int sig)
{
	sig++;
	sig--;
	s_flag = 1;
}

void	handle_sigint(int sig)
{
	write(1, "\n~/minishell$ ", 14);
	sig++;
	s_flag = 2;
}

void	s_flag_ch(char *line)
{
	char	*tmp;

	if (s_flag == 1)
	{
		rl_replace_line(line, 1);
		rl_redisplay();
	}
	if (s_flag == 2)
	{
		tmp = ft_merge("\n~/minishell$ ", line, 0, 0);
		rl_replace_line(tmp, 1);
		rl_redisplay();
		free(tmp);
	}
	s_flag = 0;
}

void	remove_all_qoutes(t_minishell *vars)
{
	int			i;
	t_redirect	*red;

	i = 0;
	red = vars->redirections;
	while (vars->argv[i])
	{
		vars->argv[i] = rm_qoutes(vars->argv[i]);
		i++;
	}
	while (red)
	{
		red->redirection = rm_qoutes(vars->redirections->redirection);
		red = red->next;
	}
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
	signal(SIGQUIT, &handle_sigquit);
	signal(SIGINT, &handle_sigint);
	while (1)
	{
		line = readline("~/minishell$ ");
		if (!line)
			break ;
		vars.op_num = 1;
		vars.tmp_fd = STDOUT_FILENO;
		vars.redirections = get_redirections(line);
		line = expand_all(&vars, line);
		vars.argc = words_count_sh(line);
		s_flag_ch(line);
		vars.argv = get_argv(line, &vars);
		remove_all_qoutes(&vars);
		// remove_all_qoutes(&vars);
		// gets(line, vars.env, vars);
		// if (vars.argv[0] && vars.argv[1] && ft_strncmp(vars.argv[0], "export", 6) == 0)
		// 	vars.env = export(vars.env, vars.argv[1]);
		// for (int i = 0; i < vars.argc; i++)
		// 	ft_printf("argv %d is %s\n", i, vars.argv[i]);
		// for (t_redirect *red = vars.redirections; red; red = red->next)
		// 	ft_printf("op num: %d, redirection: %s\n", red->op,
		// 		red->redirection);
		if (vars.argc > 0)
			process(&vars);
		else
			printf("\n");
		free_split(vars.argv, vars.argc);
		ft_free_lst(vars.redirections);
		free(line);
	}
	free_split(vars.env, i);
	printf("exit\n");
	return (0);
}
