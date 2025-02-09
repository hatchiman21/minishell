/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhamdan <yhamdan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 00:29:45 by yhamdan           #+#    #+#             */
/*   Updated: 2025/02/09 03:42:10 by yhamdan          ###   ########.fr       */
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

void handle_sigint(int sig)
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
	vars.exit_status = 0;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &handle_sigint);
	while (1)
	{	
		line = readline("~/minishell$ ");
		add_history(line);
		if (!line)
			break ;
		vars.op_num = 1;
		vars.tmp_fd = STDOUT_FILENO;
		vars.redirections = get_redirections(line);
		line = expand_all(&vars, line);
		vars.argc = words_count_sh(line);
		vars.argv = get_argv(line, &vars);
		exit1(line, vars);
		remove_all_qoutes(&vars);
		if (vars.argc > 0)
			process(&vars);
		free_split(vars.argv, vars.argc);
		ft_free_lst(vars.redirections);
		free(line);
		if (i == -2)
			exit(1);
	}
	free_split(vars.env, i);
	printf("exit\n");
	return (0);
}
