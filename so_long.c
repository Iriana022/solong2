/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irazafim <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 15:13:06 by irazafim          #+#    #+#             */
/*   Updated: 2024/07/03 12:57:08 by irazafim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <X11/keysym.h>
#include <stdio.h>
#define BUFFER_SIZE 1024
typedef struct s_coord 
{
	int x;
	int y;
}	s_coord;
typedef struct s_data
{
	void *mlx;
	void *win;
	char **map;
	s_coord pos_pers;
	s_coord pos;
}	t_data;

void mtoa(char ***arr, int lines, int col, char *buf)
{
	int i;
	int j;
	int k;

	i = 0;
	j = 0;
	*arr = (char **)malloc(sizeof(char *) * (lines + 1));
	while (i < lines)
	{
		(*arr)[i] = (char *)malloc(sizeof(char) * (col + 1));
		k = 0;
		while (k < col + 1)
		{
			(*arr)[i][k] = buf[j];
			k++;
			j++;
		}
		(*arr)[i][k - 1] = '\0';
		i++;
	}
	(*arr)[lines] = NULL;
}

char **count_lines_map(int fd)
{
	int read_bytes;
	char buf[BUFFER_SIZE];
	char **arr;
	int i;
	int lines;

	i = 0;
	lines = 0;
	while((read_bytes = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		while (i < read_bytes)
		{
			if(buf[i] == '\n')
			{
				lines++;
			}
			i++;
		}
	}
	mtoa(&arr, lines, (i - 1) / lines, buf);
	return arr;
}

void free_double(char **arr, int size)
{
	int i;

	i = 0;
	while(i < size)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int ft_strlen(char *s)
{
	int i;
	i = -1;
	while(s[++i] != '\0');
	return (i);
}

int ft_len(char **s)
{
	int i;
	i = 0;
	while(s[i] != NULL)
		i++;
	return (i);
}



void *img_return(char *path, t_data *mlx)
{
	int width;
	int height;
	return (mlx_xpm_file_to_image(mlx->mlx, path, &width, &height));
}

int valid_move_left(char **map, int x, int y)
{
	return (map[y][x - 1] != '1');
}

int valid_move_right(char **map, int x, int y)
{
	return (map[y][x + 1] != '1');
}

int valid_move_top(char **map, int x, int y)
{
	return (map[y - 1][x] != '1');
}

int valid_move_bottom(char **map, int x, int y)
{
	return (map[y + 1][x] != '1');
}

void put_pos(char **matrix_map, t_data mlx, s_coord pos, int x, int y)
{
	mlx_clear_window(mlx.mlx, mlx.win);
	while (matrix_map[pos.y] != NULL)
	{
		pos.x = 0;
		while (matrix_map[pos.y][pos.x] != '\0')
		{ 
			if (matrix_map[pos.y][pos.x] == 'P')
				mlx_put_image_to_window(mlx.mlx, mlx.win, img_return("./assets/Terrain/characters.xpm", &mlx), pos.x * 48 + x, pos.y * 48 + y);
            if (matrix_map[pos.y][pos.x] == '1')
            	mlx_put_image_to_window(mlx.mlx, mlx.win, img_return("./assets/Terrain/wall.xpm", &mlx), pos.x * 48, pos.y * 48);
            if (matrix_map[pos.y][pos.x] == 'C')
            	mlx_put_image_to_window(mlx.mlx, mlx.win, img_return("./assets/Terrain/strawberry.xpm", &mlx), pos.x * 48, pos.x * 48);
            if (matrix_map[pos.y][pos.x] == 'E')
                mlx_put_image_to_window(mlx.mlx, mlx.win, img_return("./assets/Terrain/exit.xpm", &mlx), pos.x * 48, pos.y * 48);
            pos.x++;
		}
		pos.y++;
	}
}


int key_press(int keycode, void *param)
{
	t_data *data = (t_data *)param;
	static int  x = 0;
	static int y = 0;
	
	if(keycode == XK_Escape)
	{
		mlx_destroy_window(data->mlx, data->win);
		exit(0);
	}
	else if(keycode == XK_d)
		x += 48;
	else if(keycode == XK_a)
		x -= 48;
	else if(keycode == XK_w)
		y -= 48;
	else if(keycode == XK_s)
		y += 48;
	
	put_pos(data->map, *data, data->pos, x, y);
	
	return (0);
}

int main()
{
	int fd;
	t_data mlx;

	mlx.pos.x = 0;
	mlx.pos.y = 0;
	mlx.mlx = mlx_init();
	fd = open("carte2.ber", O_RDONLY);
	mlx.map = count_lines_map(fd);
	mlx.win = mlx_new_window(mlx.mlx, 48 * ft_strlen(mlx.map[0]), 48 * ft_len(mlx.map), "solong");
	put_pos(mlx.map,  mlx, mlx.pos, 0, 0);
	mlx_key_hook(mlx.win, key_press, &mlx);
	mlx_loop(mlx.mlx);
	free_double(mlx.map, ft_len(mlx.map));
}
