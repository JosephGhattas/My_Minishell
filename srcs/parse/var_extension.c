/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_extension.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgh <jgh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 21:18:47 by jgh               #+#    #+#             */
/*   Updated: 2025/08/01 10:07:08 by jgh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char *get_special_var(char c, t_env_list *env)
{
    if (c == '?')
    {
        t_env_list *node = find_env_node(env, "?");
        return ft_strdup(node ? node->name : "0");
    }
    if (c == '$')
        return ft_itoa(getpid());
    return ft_strdup("");
}

// Given s points at first char AFTER the $, return the var name and set *len
char *get_var_name(const char *s, int *len)
{
    int i = 0;
    if (s[0] == '?' || s[0] == '$')
    {
        *len = 1;
        return ft_strndup((char*)s, 1);
    }
    while (s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
        i++;
    *len = i;
    return ft_strndup((char*)s, i);
}

char *expand_token_value(const char *input, t_env_list *env)
{
    char    *result = ft_strdup("");
    char     quote  = 0;
    int      i      = 0;

    while (input[i])
    {
        // 1) Handle opening quote
        if ((input[i] == '"' || input[i] == '\'') && quote == 0)
        {
            quote = input[i];
            i++;
            continue;
        }
        // 2) Handle closing quote
        if ((input[i] == quote))
        {
            quote = 0;
            i++;
            continue;
        }
        // 3) Variable expansion (only outside single quotes)
        if (input[i] == '$' && quote != '\'')
        {
            int   var_len;
            char *name = get_var_name(input + i + 1, &var_len);
            char *val  = (*name == '?' || *name == '$')
                           ? get_special_var(*name, env)
                           : get_env_value_exp(name, env);

            free(name);
            result = ft_strjoin_free(result, val);
            i += var_len + 1;
            continue;
        }
        // 4) Ordinary character
        result = ft_strjoin_free(result, ft_char_to_str(input[i]));
        i++;
    }

    return result;
}


char    *get_env_value_exp(const char *key, t_env_list *env)
{
    t_env_list  *node;

    node = find_env_node(env, (char *)key);
    if (node && node->equal)
        return (ft_strdup(node->name));
    return (ft_strdup(""));
}


void    update_exit_status(t_env_list **env, int status)
{
    char *s;

    s = ft_itoa(status);
    update_env_var(env, "?", s);
    free(s);
}

int     get_exit_status(t_env_list *env)
{
    t_env_list  *node;

    node = find_env_node(env, "?");
    if (!node)
        return (0);
    return (ft_atoi(node->name));
}