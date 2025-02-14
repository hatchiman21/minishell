/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 00:29:45 by yhamdan           #+#    #+#             */
/*   Updated: 2025/02/13 12:36:36 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	word_end(char c)
{
	if (c == '"' || c == ' ' || c == '\'' || c == '|' || c == '\0' || c == '$')
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

char	**ft_array_dup(char **array)
{
	int		i;
	char	**new_array;

	if (!array)
		return (NULL);
	i = 0;
	while (array[i])
		i++;
	new_array = (char **)malloc(sizeof(char *) * (i + 1));
	if (!new_array)
		return (NULL);
	i = 0;
	while (array[i])
	{
		new_array[i] = ft_strdup(array[i]);
		if (!new_array[i])
		{
			free_split(new_array, i);
			return (NULL);
		}
		i++;
	}
	new_array[i] = NULL;
	return (new_array);
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	vars;
	char		*line;

	line = NULL;
	(void)argc;
	(void)argv;
	if (!isatty(0))
		return (EXIT_FAILURE);
	vars.env = ft_array_dup(env);
	if (!vars.env && env)
	{
		ft_putstr_fd("minishell: enviorment malloc failed\n", 2);
		return (EXIT_FAILURE);
	}
	vars.exit_status = 0;
	vars.here_doc_fds = NULL;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &handle_sigint);
	while (1)
	{
		vars.pipefd[0] = -1;
		vars.last_id = 0;
		line = readline("~/minishell$ ");
		if (!line)
			break ;
		if (first_input_check(line))
		{
			add_history(line);
			free(line);
			continue ;
		}
		vars.final_line = ft_strdup(line);
		if (!vars.final_line)
		{
			ft_putstr_fd("minishell: final line malloc failed\n", 2);
			free(line);
			break ;
		}
		vars.op_num = 1;
		vars.tmp_fd = STDOUT_FILENO;
		vars.redirections = get_redirections(line);
		if (!vars.redirections && (ft_strchr_find(vars.final_line, '>') || ft_strchr_find(vars.final_line, '<')))
		{
			add_history(vars.final_line);
			free(vars.final_line);
			free(line);
			ft_putstr_fd("minishell: redirection malloc failed\n", 2);
			break ;
		}
		if (ambiguous_redirect_check(vars.redirections, vars.env) == 1)
		{
			add_history(vars.final_line);
			ft_free_red(vars.redirections);
			free(vars.final_line);
			free(line);
			continue ;
		}
		line = expand_all(&vars, line);
		prepare_here_doc(&vars, vars.redirections);
		if (!vars.here_doc_fds && ft_strnstr(vars.final_line, "<<", ft_strlen(vars.final_line)))
		{
			if (vars.final_line)
				free(vars.final_line);
			free(line);
			ft_free_red(vars.redirections);
			ft_putstr_fd("minishell: here doc malloc failed\n", 2);
			break ;
		}
		add_history(vars.final_line);
		free(vars.final_line);
		vars.argc = words_count_sh(line);
		vars.argv = get_argv(line, &vars);
		free(line);
		// if (vars.argv)
		// 	ft_excute(NULL, vars.argv, &vars, 0);
		if (!vars.argv)
		{
			ft_free_red(vars.redirections);
			ft_putstr_fd("minishell: argv malloc failed\n", 2);
			break ;
		}
		remove_all_qoutes(&vars);
		if (vars.argc > 0)
			process(&vars);
		close_free_here_doc(&vars.here_doc_fds);
		free_split(vars.argv, vars.argc);
		ft_free_red(vars.redirections);
	}
	free_split(vars.env, -1);
	printf("exit\n");
	return (0);
}
