/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 16:43:10 by aatieh            #+#    #+#             */
/*   Updated: 2024/12/25 21:08:49 by aatieh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

int	ft_strchr_find(const char *s, int c)
{
	int	i;

	i = 1;
	while (s[i - 1])
	{
		if (s[i - 1] == (char)c)
			return (i);
		i++;
	}
	return (0);
}

char	*ft_lstclear_item(t_line_list **lst, int fd)
{
	t_line_list	*temp;
	t_line_list	*step;

	temp = *lst;
	while (temp)
	{
		if (temp->fd == fd)
			break ;
		step = temp;
		temp = temp->next;
	}
	if (!temp)
		return (NULL);
	if (temp->buff)
		free(temp->buff);
	if (temp == *lst)
		*lst = temp->next;
	else
		step->next = temp->next;
	free(temp);
	return (NULL);
}
