/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irazafim <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 15:13:30 by irazafim          #+#    #+#             */
/*   Updated: 2024/07/10 09:41:49 by irazafim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H
# include "minilibx-linux/mlx.h"
# include "minilibx-linux/mlx_int.h"
# include <fcntl.h>
# include <unistd.h>
# include <X11/keysym.h>
# define BUFFER_SIZE 1024
# define PLAYER "./assets/Terrain/characters.xpm"
# define WALL "./assets/Terrain/wall.xpm"
# define COLLECTIBLE "./assets/Terrain/strawberry.xpm"
# define EXIT "./assets/Terrain/exit.xpm"
# define BACKGROUND "./assets/Terrain/bg.xpm"

typedef struct s_coord
{
    int x;
    int y;
}   t_coord;
typedef struct s_data
{
    void *mlx;
    void *win;
    char **map;
	void *img[5];
    t_coord *pos_pers;
}   t_data;
#endif
