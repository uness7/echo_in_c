#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Structs  & Enums Used */

/* Functions Prototypes */

/* Implementation */

bool	is_quotes(char c)
{
	return (c == '"' || c == '\'');
}

void	remove_last_quote(char *str)
{
	int		len;
	bool	escaped;

	len = strlen(str);
	if (len == 0)
		return ;
	if (str[len - 1] == '"' || str[len - 1] == '\'')
	{
		escaped = false;
		int i = len - 2;
		while (i >= 0 && str[i] == '\\')
		{
			escaped = !escaped;
			i--;
		}
		if (!escaped)
			str[len - 1] = '\0';
	}
}

t_echo_arr	**split(char *input)
{
	t_echo_arr	**res;
	int			i;
	int			k;
	bool		inside_quotes;
	int			start_index;
	int			token_length;

	inside_quotes = false;
	res = arena_alloc(arena, (strlen(input)) * (sizeof(t_echo_arr *)) + 1);
	k = 0;
	i = 0;
	input+=5;
	while (input[i] != '\0')
	{
		if (is_quotes(input[i]))
		{
			inside_quotes = !inside_quotes;
			i++;
		}
		else if (!inside_quotes && is_space(input[i]))
		{
			res[k] = arena_alloc(arena, sizeof(t_echo_arr));
			res[k]->data = ft_strdup(arena, "SPACE");
			res[k]->type = _SPACE;
			k++;
			while (input[i] && input[i] == ' ')
				i++;
		}
		else
		{
			start_index = i;
			while (input[i] && (inside_quotes || input[i] != ' '))
			{
				if (input[i] == '"' || input[i] == '\'')
				{
					i++;
					inside_quotes = !inside_quotes;
					break ;
				}
				i++;
			}
			//printf("i : %d\n", i);
			if (start_index <= (int)strlen(input))
			{
				//printf("current : %c\n", input[i]);
				token_length = i - start_index;
				res[k] = arena_alloc(arena, sizeof(t_echo_arr));
				res[k]->data = arena_alloc(arena, (token_length + 1)
						* sizeof(char));
				res[k]->type = WORD;
				strncpy(res[k]->data, input + start_index, token_length);
				remove_last_quote(res[k]->data);
				res[k]->data[token_length] = '\0';
				k++;
			}
		}
	}
	k++;
	res[k] = NULL;
	return (res);
}

bool	check_str_for_n_char(char *str)
{
	int	i;

	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

void	jump_spaces(t_echo_arr ***arr)
{
	int	i;

	i = 0;
	while ((*arr)[i] != NULL)
	{
		if ((*arr)[i]->type != _SPACE)
			break ;
		(*arr)++;
		i++;
	}
}

int	ft_echo(char *input)
{
	int			i;
	int			flag;
	t_echo_arr	**echo_args;

	i = 0;
	flag = 0;
	echo_args = split(arena, input);
	jump_spaces(&echo_args);
	while (echo_args[i] != NULL)
	{
		if (ft_strncmp(echo_args[i]->data, "-", 1) == 0
			&& check_str_for_n_char(echo_args[i]->data))
		{
			i++;
			flag = 1;
		}
		if (echo_args[i] && echo_args[i]->type == WORD)
			printf("%s", echo_args[i]->data);
		else if (echo_args[i] && echo_args[i - 1] && echo_args[i + 1] &&
				echo_args[i-1]->type == WORD &&
				echo_args[i+1]->type == WORD &&
			       echo_args[i]->type == _SPACE &&
			       !(strncmp(echo_args[i - 1]->data, "-", 1) == 0 \
				       && check_str_for_n_char(echo_args[i - 1]->data)))
		{
			printf(" ");
		}

		i++;
	}
	if (flag == 0)
		printf("\n");
	return (0);
}

/* Main Function  */
int	main(void)
{
	char	*input = "echo -n hello world";
	echo(input);
	return (0);
}

