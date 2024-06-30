/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irazafim <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 15:13:06 by irazafim          #+#    #+#             */
/*   Updated: 2024/06/13 15:13:09 by irazafim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <X11/keysym.h>
#include <stdio.h>
#define BUFFER_SIZE 1024

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

int key_press(int keycode, void *param)
{
	if(keycode == 53)
	{
		printf("Escape pressed");
		mlx_destroy_window(param, param);
		exit(0);
	}
	return (0);
}

int main()
{
	char **arr;
	int fd;
	void *mlx;
	void *mlx_win;
	void *img_wall;
	void *img_collectible;
	void *img_characters;
	void *img_exit;
	int img_width;
	int img_height;
	int i;
	int k;

	mlx = mlx_init();
	img_wall = mlx_xpm_file_to_image(mlx, "./assets/Terrain/wall.xpm", &img_width, &img_height);
	img_collectible = mlx_xpm_file_to_image(mlx, "./assets/Terrain/strawberry.xpm", &img_width, &img_height);
	img_characters = mlx_xpm_file_to_image(mlx, "./assets/Terrain/characters.xpm", &img_width, &img_height);
	img_exit = mlx_xpm_file_to_image(mlx, "./assets/Terrain/exit.xpm", &img_width, &img_height);

	i = 0;
	fd = open("carte2.ber", O_RDONLY);
	arr = count_lines_map(fd);
	mlx_win = mlx_new_window(mlx, 48 * ft_strlen(arr[0]), 48 * ft_len(arr), "solong");
	k = 0;
	while (arr[i] != NULL)
	{
		k = 0;
		while(arr[i][k] != '\0')
		{
			if (arr[i][k] == '1')
				mlx_put_image_to_window(mlx, mlx_win, img_wall, k * 48, i * 48);
			if (arr[i][k] == 'C')
				mlx_put_image_to_window(mlx, mlx_win, img_collectible, k * 48, i * 48);
			if (arr[i][k] == 'P')
				mlx_put_image_to_window(mlx, mlx_win, img_characters, k * 48, i * 48);
			if (arr[i][k] == 'E')
				mlx_put_image_to_window(mlx, mlx_win, img_exit, k * 48, i * 48);
				

			k++;
		}
		i++;
	}
	mlx_key_hook(mlx_win, key_press, mlx);
	mlx_loop(mlx);
	free_double(arr, ft_len(arr));
}
