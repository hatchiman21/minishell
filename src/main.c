/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 00:29:45 by yhamdan           #+#    #+#             */
/*   Updated: 2025/01/24 04:49:16 by aatieh           ###   ########.fr       */
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

int	new_word_start(char c)
{
	if (c == '"' || c == ' ' || c == '\'' || c == '|')
		return (1);
	return (0);
}

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

void	remove_from_line(char *line, int i, int j)
{
	int	m;

	m = 0;
	if (!line || !line[i])
		return ;
	while (line[i + m + j])
	{
		line[i + m] = line[i + m + j];
		m++;
	}
	while(line[i + m])
	{
		line[i + m] = '\0';
		m++;
	}
}

t_redirect	*assgine_redirection(char *line, int i, int op_num, int *space_num)
{
	int			j;
	t_redirect	*redirect;

	j = 0;
	*space_num = 0;
	redirect = malloc(sizeof(t_redirect));
	redirect->op = op_num;
	while(line[i + j] && (line[i + j] == '<' || line[i + j] == '>'))
		j++;
	while(line[i + j + *space_num] && line[i + j + *space_num] == ' ')
		(*space_num)++;
	while (line[i + *space_num + j] && line[i + *space_num + j] != ' ')
		j++;
	redirect->redirection = malloc(sizeof(char) * j + 1);
	return (redirect);
}

t_redirect	*get_redirection(char *line, int i, int op_num)
{
	int	j;
	int	space_num;
	t_redirect	*redirect;

	j = 0;
	redirect = assgine_redirection(line, i, op_num, &space_num);
	while(line[i + j] && (line[i + j] == '<' || line[i + j] == '>'))
	{
		redirect->redirection[j] = line[i + j];
		j++;
	}
	while (line[i + space_num + j] && line[i + space_num + j] != ' ')
	{
		redirect->redirection[j] = line[i + space_num + j];
		j++;
	}
	redirect->redirection[j] = '\0';
	redirect->next = NULL;
	remove_from_line(line, i, space_num + j);
	return (redirect);
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

void	ft_printlist(t_redirect *lst)
{
	t_redirect	*tmp;

	tmp = lst;
	printf("printing list\n");
	while (tmp)
	{
		printf("redirect for %d is %s\n", tmp->op, tmp->redirection);
		tmp = tmp->next;
	}
	printf("end of list\n");
}

void	redirectionadd_back(t_redirect **lst, t_redirect *new)
{
	t_redirect	*tmp;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_redirect	*get_redirections(char *line)
{
	int			i;
	int			op_num;
	t_redirect	*head;

	if (!line )
		return (NULL);
	head = NULL;
	i = 0;
	op_num = 0;
	while (line[i])
	{
		if (line[i] == '>' || line[i] == '<')
			redirectionadd_back(&head, get_redirection(line, i, op_num));
		if (line[i] == '|')
			op_num++;
		i++;
	}
	return (head);
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

void	ft_free_lst(t_redirect *lst)
{
	t_redirect	*tmp;

	while (lst)
	{
		tmp = lst;
		lst = lst->next;
		free(tmp->redirection);
		free(tmp);
	}
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
		line = readline(NULL);
		if (!line)
			break ;
		vars.op_num = count_char(line, '|') + 1;
		vars.redirections = get_redirections(line);
		vars.argc = words_count_sh(line);
		vars.argv = get_argv(line, vars.argc);
		expand(vars.argv, vars);
		for (int i = 0; i < vars.argc; i++)
			ft_printf("argv %d is %s\n", i, vars.argv[i]);
		ft_printlist(vars.redirections);
		free_split(vars.argv, vars.argc);
		ft_free_lst(vars.redirections);
		free(line);
	}
	return 0;
}
