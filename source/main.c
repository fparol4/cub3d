/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 16:50:39 by fcardozo         #+#    #+#             */
/*   Updated: 2026/11/06 16:50:39 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/cub3d.h"

int	main(int argc, char **argv)
{
	t_game	g;

	if (runtime_args(argc, argv))
		return (1);
	if (runtime_setup(&g, argv[1]))
		return (1);
	return (runtime_run(&g));
}
