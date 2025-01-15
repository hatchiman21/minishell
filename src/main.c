/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhamdan <yhamdan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 00:29:45 by yhamdan           #+#    #+#             */
/*   Updated: 2025/01/15 18:45:50 by yhamdan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// void	get_variable_len(char **env, char *line, int *j, int *i)
// {
// 	int		n;
// 	int		m;
// 	char	*var_value;

// 	n = *j;
// 	while (line[*j] != ' ' && line[*j] != '\'' && line[*j] != '"')
// 		*j++;
// 	n -= *j;
// 	m = 0;
// 	while (env[m] && ft_strncmp(env[m], line[*j], n))
// 		m++;
// 	*i += ft_strlen(env[m]) - n;
// }

// char	*get_variable(char **env, char *line, int *j, int *i)
// {
// 	int		n;
// 	int		m;
// 	char	*var_value;

// 	n = *j;
// 	while (line[*j] != ' ' && line[*j] != '\'' && line[*j] != '"')
// 		*j++;
// 	n -= *j;
// 	m = 0;
// 	while (env[m] && ft_strncmp(env[m], line[*j], n) && env[m][n + 1] != '=')
// 		m++;
// 	if (!env[m])
// 		return (NULL);
// 	var_value = ft_strdup(env[m] + n + 1);
// 	*i += n;
// 	return (var_value);
// }

// int	word_len_sh(char *line, int *j, char **env)
// {
// 	int	flag_q;
// 	int	i;

// 	if (!line)
// 		return (NULL);
// 	flag_q = 0;
// 	while (line[*j] && (line[*j] != ' ' || flag_q))
// 	{
// 		if (line[*j] == '\'' && !flag_q)
// 			flag_q = 1;
// 		else if (line[*j] == '"' && !flag_q)
// 			flag_q = 2;
// 		else if ((line[*j] == '\'' && flag_q == 1) || (line[*j] == '"' && flag_q == 2))
// 			flag_q = 0;
// 		else
// 		{
// 			i++;
// 			*j++;
// 		}
// 	}
// }

// char	*sh_strdup(const char *s, int j, int i)
// {
// 	int	i;
// 	char *res;

// 	res = (char *)malloc(sizeof(char) * (j + 1));
// 	if (res == NULL)
// 		return (NULL);
// 	i = 0;
// 	while (i < j)
// 	{
// 		res[i] = s[i];
// 		i++;
// 	}
// 	res[i] = '\0';
// 	return (res);
// }

// void	my_pwd(char **argv)
// {
	
// }

void	my_echo(char **argv)
{
	int	i;
	int	argc;

	argc = 0;
	if (!argv)
		exit(0);
	while (argv[argc] && argv[argc][0] != '|')
		argc++;
	if (argv[1] && !ft_strncmp(argv[1], "-n", 3))
		i = 2;
	else
		i = 1;
	while (argv[i] && i < argc)
		ft_putstr_fd(argv[i++], 1);
	if (argv[1] && ft_strncmp(argv[1], "-n", 3))
		ft_putchar_fd('\n', 1);
	exit(0);
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
	while (line[i + j] && line[i + j] != '\'' && line[i + j] != '"' && line[i + j] != ' ')
		j++;
	return (j);
}

int	words_count_sh(char *line)
{
	int	i;
	int	j;
	int	m;
	int	word;

	i = 0;
	word = 0;
	while (line[i])
	{
		j = 0;
		while (line[i] == ' ')
			i++;
		m = i + 1;
		while (m != i + j)
		{
			m = i + j;
			j += word_check(line, i + j);
		}
		if (j != 0)
			word++;
		i += j;
	}
	return (word);
}
char	*get_token_sh(char *line, int *j)
{
	int	flag_q;
	int	i;

	if (!line)
		return (NULL);
	flag_q = 0;
	i = *j;
	while (line[*j] && !(line[*j] == ' ' && !flag_q))
	{
		if ((line[*j] == '\'' && flag_q == 1)
			|| (line[*j] == '"' && flag_q == 2))
			flag_q = 0;
		else if (line[*j] == '\'' && !flag_q)
			flag_q = 1;
		else if (line[*j] == '"' && !flag_q)
			flag_q = 2;
		(*j)++;
	}
	return(ft_substr(line, i, *j - i));
}

void	get_argv(char *line, t_minishell *vars)
{
	int	i;
	int	j;

	vars->argv = malloc(sizeof(char *) * (vars->argc + 1));
	i = 0;
	j = 0;
	while (i < vars->argc)
	{
		while (line[j] && line[j] == ' ')
			j++;
		vars->argv[i] = get_token_sh(line, &j);
		i++;
	}
	vars->argv[i] = NULL;
}

char	*ft_merge(char *s1, char *s2, int free_s1, int free_s2)
{
	char	*tmp;

	tmp = ft_strjoin(s1, s2);
	if (free_s1)
		free(s1);
	if (free_s2)
		free(s2);
	return (tmp);
}

void	remove_from_line(char **line, int i, int j)
{
	int	m;

	m = 0;
	while (line[0][i + m] && line[0][i + m + j])
	{
		line[0][i + m] = line[0][i + m + j];
		m++;
	}
	line[0][i + m] = '\0';
}

void	add_redirection(char *line, int *i, char **redirections)
{
	int		j;
	char	*tmp;
	char	op[3];

	j = 0;
	while(line[*i] && (line[*i] == '<' || line[*i] == '>'))
		op[j++] = line[(*i)++];
	op[j] = '\0';
	while(line[*i] && line[*i] == ' ')
		(*i)++;
	j = 0;
	while (line[*i + j] && line[*i + j] != ' ')
		j++;
	if (!*redirections)
		*redirections = ft_strdup("");
	tmp = ft_substr(line, *i, j);
	tmp = ft_merge(op, tmp, 0, 1);
	*redirections = ft_merge(*redirections, tmp, 1, 1);
	remove_from_line(&line, *i, j);
	*i += j;
}

char	**get_redirections(char *line, int argc)
{
	int		i;
	int		k;
	char	**redirections;

	i = 0;
	k = 0;
	if (!line || argc == 0)
		return (NULL);
	redirections = malloc(sizeof(char *) * argc);
	while (i < argc)
		redirections[i++] = NULL;
	i = 0;
	while (line[i])
	{
		if (line[i] == '>' || line[i] == '<')
			add_redirection(line, &i, &redirections[k]);
		if (line[i] == '|')
			k++;
		i++;
	}
	return (redirections);
}


int	main(int argc, char **argv, char **env)
{
	t_minishell	vars;
	char		*line;

	line = NULL;
	(void)argc;
	(void)argv;
	vars.env = env;
	// (void)env;
	while (1)
	{
		line = readline(NULL);
		vars.argc = words_count_sh(line);
		get_redirections(line, vars.argc);
		get_argv(line, &vars);
		// expand(vars.argv + 1, vars);
		for (int i = 0; i < vars.argc; i++)
			ft_printf("argv %d is %s\n", i, vars.argv[i]);
		// my_echo(vars.argv);
		free(line);
	}
	return 0;
}
