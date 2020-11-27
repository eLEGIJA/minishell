/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafflow <msafflow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 22:41:17 by msafflow          #+#    #+#             */
/*   Updated: 2020/11/27 22:41:17 by msafflow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "shell.h"
#include "scanner.h"
#include "source.h"

char *g_tok_buf = NULL;
int   g_tok_bufsize  = 0;
int   g_tok_bufindex = -1;

/* special token to indicate end of input */
t_token g_eof_token = 
{
    .text_len = 0,
};


void add_to_buf(char c)
{
    g_tok_buf[g_tok_bufindex++] = c;

    if(g_tok_bufindex >= g_tok_bufsize)
    {
        char *tmp = realloc(g_tok_buf, g_tok_bufsize*2);

        if(!tmp)
        {
            errno = ENOMEM;
            return;
        }

        g_tok_buf = tmp;
        g_tok_bufsize *= 2;
    }
}


t_token *create_token(char *str)
{
    t_token *tok = malloc(sizeof(t_token));
    
    if(!tok)
    {
        return NULL;
    }

    memset(tok, 0, sizeof(t_token));
    tok->text_len = strlen(str);
    
    char *nstr = malloc(tok->text_len+1);
    
    if(!nstr)
    {
        free(tok);
        return NULL;
    }
    
    strcpy(nstr, str);
    tok->text = nstr;
    
    return tok;
}


void free_token(t_token *tok)
{
    if(tok->text)
    {
        free(tok->text);
    }
    free(tok);
}


t_token *tokenize(t_source *src)
{
    int  endloop = 0;

    if(!src || !src->buffer || !src->bufsize)
    {
        errno = ENODATA;
        return &g_eof_token;
    }
    
    if(!g_tok_buf)
    {
        g_tok_bufsize = 1024;
        g_tok_buf = malloc(g_tok_bufsize);
        if(!g_tok_buf)
        {
            errno = ENOMEM;
            return &g_eof_token;
        }
    }

    g_tok_bufindex     = 0;
    g_tok_buf[0]       = '\0';

    char nc = next_char(src);
    char nc2;
    int i;

    if(nc == ERRCHAR || nc == EOF)
    {
        return &g_eof_token;
    }

    do
    {
        switch(nc)
        {
            case  '"':
            case '\'':
            case  '`':
                /*
                 * for quote chars, add the quote, as well as everything between this
                 * quote and the matching closing quote, to the token buffer.
                 */
                add_to_buf(nc);
                i = find_closing_quote(src->buffer+src->curpos);

		if(!i)
                {
                    /* failed to find matching quote. return error token */
                    src->curpos = src->bufsize;
                    fprintf(stderr, "error: missing closing quote '%c'\n", nc);
                    return &g_eof_token;
                }

		while(i--)
                {
                    add_to_buf(next_char(src));
                }
                break;

            case '\\':
                /* get the next char after the backslah */
                nc2 = next_char(src);

		/*
                 * discard backslash+newline '\\n' combination.. in an interactive shell, this
                 * case shouldn't happen as the read_cmd() function discards the '\\n' sequence
                 * automatically.. however, if the input comes from a command string or script,
                 * we might encounter this sequence.
                 */
                if(nc2 == '\n')
                {
                    break;
                }

		/* add the backslah to the token buffer */
                add_to_buf(nc);

		/* add the escaped char to the token buffer */
                if(nc2 > 0)
                {
                    add_to_buf(nc2);
                }
                break;
                
            case '$':
                /* add the '$' to buffer and check the char after it */
                add_to_buf(nc);
                nc = peek_char(src);

		/* we have a '${' or '$(' sequence */
                if(nc == '{' || nc == '(')
                {
                    /* find the matching closing brace */
                    i = find_closing_brace(src->buffer+src->curpos+1);

		    if(!i)
                    {
                        /* failed to find matching brace. return error token */
                        src->curpos = src->bufsize;
                        fprintf(stderr, "error: missing closing brace '%c'\n", nc);
                        return &g_eof_token;
                    }

		    while(i--)
                    {
                        add_to_buf(next_char(src));
                    }
                }
		/*
                 * we have a special parameter name, such as $0, $*, $@, $#,
                 * or a positional parameter name, such as $1, $2, ...
                 */
                else if(isalnum(nc) || nc == '*' || nc == '@' || nc == '#' ||
                                       nc == '!' || nc == '?' || nc == '$')
                {
                    add_to_buf(next_char(src));
                }
                break;

            case ' ':
            case '\t':
                if(g_tok_bufindex > 0)
                {
                    endloop = 1;
                }
                break;
                
            case '\n':
                if(g_tok_bufindex > 0)
                {
                    unget_char(src);
                }
                else
                {
                    add_to_buf(nc);
                }
                endloop = 1;
                break;
                
            default:
                add_to_buf(nc);
                break;
        }

        if(endloop)
        {
            break;
        }

    } while((nc = next_char(src)) != EOF);

    if(g_tok_bufindex == 0)
    {
        return &g_eof_token;
    }
    
    if(g_tok_bufindex >= g_tok_bufsize)
    {
        g_tok_bufindex--;
    }
    g_tok_buf[g_tok_bufindex] = '\0';

    t_token *tok = create_token(g_tok_buf);
    if(!tok)
    {
        fprintf(stderr, "error: failed to alloc buffer: %s\n", strerror(errno));
        return &g_eof_token;
    }

    tok->src = src;
    return tok;
}
