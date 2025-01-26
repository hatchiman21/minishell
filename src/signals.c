/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhamdan <yhamdan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 01:01:29 by yhamdan           #+#    #+#             */
/*   Updated: 2025/01/27 01:32:23 by yhamdan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

void	sig_stay(void)
{
	struct sigaction sa;

	sa.sa_handler = &handle_sigint;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
}

void	sig_