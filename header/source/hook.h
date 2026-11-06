/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 17:02:50 by fcardozo         #+#    #+#             */
/*   Updated: 2026/11/06 17:02:50 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HOOK_H
# define HOOK_H
# include "core/struct.h"

int	hook_focus_out(t_game *g);
int	hook_expose(t_game *g);
int	hook_press(int keycode, t_game *g);
int	hook_release(int keycode, t_game *g);
#endif
