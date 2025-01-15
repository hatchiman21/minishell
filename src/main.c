/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhamdan <yhamdan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 00:29:45 by yhamdan           #+#    #+#             */
/*   Updated: 2025/01/15 21:08:44 by yhamdan          ###   ########.fr       */
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
	while (line[i + j] && line[i + j] != '\'' && line[i + j] != '"' && line[i + j] != ' ' && line[i + j] != '|')
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
		if (j != 0 || line[i] == '|')
			word++;
		if (line[i] == '|')
			i++;
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
	if (line[*j] == '|')
		return (ft_substr(line, (*j)++, 1));
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
		if (line[*j] == '|')
			break ;
	}
	return (ft_substr(line, i, *j - i));
}

char	**get_argv(char *line, int argc)
{
	int		i;
	int		j;
	char	**argv;

	argv = malloc(sizeof(char *) * (argc + 1));
	i = 0;
	j = 0;
	while (i < argc)
	{
		while (line[j] && line[j] == ' ')
			j++;
		argv[i] = get_token_sh(line, &j);
		i++;
	}
	argv[i] = NULL;
	return (argv);
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
	int		start;
	char	*tmp;
	char	op[3];

	j = 0;
	start = *i;
	while(line[start] && (line[start] == '<' || line[start] == '>'))
		op[j++] = line[start++];
	op[j] = '\0';
	while(line[start] && line[start] == ' ')
		start++;
	j = 0;
	while (line[start + j] && line[start + j] != ' ')
		j++;
	tmp = ft_substr(line, start, j);
	tmp = ft_merge(op, tmp, 0, 1);
	*redirections = ft_merge(*redirections, tmp, 1, 1);
	remove_from_line(&line, *i, start + j - *i);
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

char	**get_redirections(char *line, t_minishell *vars)
{
	int		i;
	int		k;
	char	**redirections;

	if (!line )
		return (NULL);
	vars->op_num = count_char(line, '|') + 1;
	redirections = malloc(sizeof(char *) * vars->op_num);
	i = 0;
	while (i < vars->op_num)
		redirections[i++] = ft_strdup("");
	k = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == '>' || line[i] == '<')
			add_redirection(line, &i, &redirections[k]);
		if (line[i] == '|')
			k++;
		i++;
	}
	for(int m = 0; m < k + 1; m++)
		ft_printf("redirection[%d] = %s\n", m, redirections[m]);
	return (redirections);
}

void	free_split(char **split, int num)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (i < num)
		free(split[i++]);
	free(split);
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
		if (!line)
			break ;
		vars.redirections = get_redirections(line, &vars);
		vars.argc = words_count_sh(line);
		vars.argv = get_argv(line, vars.argc);
		expand(vars.argv, vars);
		
		for (int i = 0; i < vars.argc; i++)
			ft_printf("argv %d is %s\n", i, vars.argv[i]);
		// my_echo(vars.argv);
		free_split(vars.argv, vars.argc);
		free_split(vars.redirections, vars.op_num);
		free(line);
	}
	return 0;
}
