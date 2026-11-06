/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runtime.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 17:02:50 by fcardozo         #+#    #+#             */
/*   Updated: 2026/11/06 17:02:50 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RUNTIME_H
# define RUNTIME_H
# include "core/struct.h"

int		runtime_run(t_game *g);
int		runtime_close(t_game *g);
void	runtime_cleanup(t_game *g);
int		runtime_args(int argc, char **argv);
int		runtime_setup(t_game *g, char *map_path);
#endif
