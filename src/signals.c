/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aatieh <aatieh@student.42amman.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 01:01:29 by yhamdan           #+#    #+#             */
/*   Updated: 2025/02/09 06:56:48 by aatieh           ###   ########.fr       */
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
	// struct sigaction sa;
	// sa.sa_handler = &handle_sigquit;
	// sa.sa_flags = SA_RESTART;
	// sigaction(SIGQUIT, &sa, NULL);
	signal(SIGQUIT, &handle_sigquit);
}

void	sig_nothing(void)
{
	// struct sigaction sa;
	// sa.sa_handler = &handle_sigint;
	// sa.sa_flags = SA_RESTART;
	// sigaction(SIGINT, &sa, NULL);
	signal(SIGINT, &handle_sigint);
}
