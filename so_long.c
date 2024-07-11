/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irazafim <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 15:13:06 by irazafim          #+#    #+#             */
/*   Updated: 2024/07/10 11:42:06 by irazafim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <X11/keysym.h>
#include <stdio.h>

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

t_coord	*catch_pos(char **map, int i, int j)
{
	t_coord	*coord;

	coord = malloc(sizeof(t_coord));
	if (NULL == coord)
		return (NULL);
	coord->x = 0;
	coord->y = 0;
	while (map[i] != NULL)
	{
		j = 0;
		while (map[i][j])
		{
			if (map[i][j] == 'P')
			{
				coord->x =j;
				coord->y = i;
				return (coord);
			}
			++j;
		}
		++i;
	}
	return (coord);
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

void free_array_image(int size, t_data *mlx)
{
	int i = 0;
	while (i < size)
	{
		mlx_destroy_image(mlx->mlx, mlx->img[i]);
		i++;
	}
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



void put_pos(char **matrix_map, t_data mlx)
{
	int y;
	int x;

	y = 0;
	while (matrix_map[y] != NULL)
	{
		x = 0;
		while (matrix_map[y][x] != '\0')
		{ 
			if (matrix_map[y][x] == 'P')
				mlx_put_image_to_window(mlx.mlx, mlx.win, mlx.img[0], x * 48, y * 48);
	       	else if (matrix_map[y][x] == '1')
            	mlx_put_image_to_window(mlx.mlx, mlx.win, mlx.img[1], x * 48, y * 48);
        	else if (matrix_map[y][x] == 'C')
        		mlx_put_image_to_window(mlx.mlx, mlx.win, mlx.img[2], x * 48, y * 48);
        	else if (matrix_map[y][x] == 'E')
				mlx_put_image_to_window(mlx.mlx, mlx.win, mlx.img[3], x * 48, y * 48);
			else if (matrix_map[y][x] == '0')
				mlx_put_image_to_window(mlx.mlx, mlx.win, mlx.img[4], x * 48, y * 48);
        	x++;
		}
		y++;
	}
}

int invalid_move(int x, int y, char **map)
{
	return (x < 0 || y < 0 || x > ft_strlen(map[0]) - 1 || y > ft_len(map) - 1);
}

void	print_map(char **map)
{
	for(int y = 0; map[y] != NULL; ++y)
	{
		for (int x = 0; map[y][x] != '\0'; ++x)
			printf("%c", map[y][x]);
		printf("\n");
	}
}

void move(char **map, char direction, t_coord pos)
{
	if (direction == 'l')
	{
		map[pos.y][pos.x] = '0';
		map[pos.y][pos.x - 1] = 'P';
	}
	else if (direction == 'r')
	{
		map[pos.y][pos.x] = '0';
		map[pos.y][pos.x + 1] = 'P';
	}
	else if (direction == 'u')
	{
		map[pos.y][pos.x] = '0';
		map[pos.y - 1][pos.x] = 'P';
	}
	else if (direction == 'd')
	{
		map[pos.y][pos.x] = '0';
		map[pos.y + 1][pos.x] = 'P';
	}
}

int on_destroy(t_data *data)
{
	free(data->pos_pers);
	free_array_image(5, data);
	mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_display(data->mlx);
	free(data->mlx);
	free_double(data->map, ft_len(data->map));
	exit(0);
	return (0);
}

void right(t_data *data)
{
	if (invalid_move(data->pos_pers->x + 1, data->pos_pers->y, data->map) || (data->map[data->pos_pers->y][data->pos_pers->x + 1] == '1'))
		return;
	move(data->map, 'r', *(data->pos_pers));
	(data->pos_pers->x)++;
}

void left(t_data *data)
{
	if (invalid_move(data->pos_pers->x - 1, data->pos_pers->y, data->map) ||  (data->map[data->pos_pers->y][data->pos_pers->x - 1] == '1'))
		return;
	move(data->map, 'l', *(data->pos_pers));
	(data->pos_pers->x)--;
}

void top(t_data *data)
{
	if (invalid_move(data->pos_pers->x, data->pos_pers->y - 1, data->map) || (data->map[data->pos_pers->y - 1][data->pos_pers->x] == '1'))
		return;
	move(data->map, 'u', *(data->pos_pers));
	(data->pos_pers->y)--;
}

void bottom(t_data *data)
{
	if (invalid_move(data->pos_pers->x, data->pos_pers->y + 1, data->map) || (data->map[data->pos_pers->y + 1][data->pos_pers->x] == '1'))
		return;
	move(data->map, 'd', *(data->pos_pers));
	(data->pos_pers->y)++;
}

int key_press(int keycode, void *param)
{
	t_data *data = (t_data *)param;
		
	if(keycode == XK_Escape)
		on_destroy(data);
	else if(keycode == XK_d)
		right(data);
	else if(keycode == XK_a)
		left(data);
	else if(keycode == XK_w)
		top(data);
	else if(keycode == XK_s)
		bottom(data);
	printf("\n----------\n");
	print_map(data->map);
	put_pos(data->map, *data);
	return (0);
}

int close_window(t_data *data)
{
	on_destroy(data);
	return (0);
}

int main()
{
	int fd;
	t_data mlx;

	mlx.mlx = mlx_init();
	fd = open("carte3.ber", O_RDONLY);
	mlx.img[0] = img_return(PLAYER, &mlx);
	mlx.img[1] = img_return(WALL, &mlx);
	mlx.img[2] = img_return(COLLECTIBLE, &mlx);
	mlx.img[3] = img_return(EXIT, &mlx);
	mlx.img[4] = img_return(BACKGROUND, &mlx);
	mlx.map = count_lines_map(fd);
	mlx.win = mlx_new_window(mlx.mlx, 48 * ft_strlen(mlx.map[0]), 48 * ft_len(mlx.map), "solong");
	put_pos(mlx.map,  mlx);
	mlx.pos_pers = catch_pos(mlx.map, 0, 0);
	print_map(mlx.map);
	mlx_hook(mlx.win, DestroyNotify, StructureNotifyMask, &close_window, &mlx);
	mlx_key_hook(mlx.win, key_press, &mlx);
	mlx_loop(mlx.mlx);
}
