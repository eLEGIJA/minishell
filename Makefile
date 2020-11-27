# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msafflow <msafflow@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/27 20:03:38 by msafflow          #+#    #+#              #
#    Updated: 2020/11/27 22:54:59 by msafflow         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# directory definitions
SRCDIR=.
BUILTINS_SRCDIR=$(SRCDIR)/builtins
SYMTAB_SRCDIR=$(SRCDIR)/symtab
BUILD_DIR=$(SRCDIR)/build

# compiler name and flags
CC=gcc
LIBS=
CFLAGS=-Wall -Wextra -g -I$(SRCDIR)
LDFLAGS=-g

# generate the lists of source and object files
SRCS_BUILTINS=$(shell find $(SRCDIR)/builtins -name "*.c")

SRCS_SYMTAB=$(SRCDIR)/symtab/symtab.c

SRCS=main.c prompt.c node.c parser.c scanner.c source.c executor.c initsh.c  \
     pattern.c strings.c wordexp.c shunt.c get_next_line.c            \
     $(SRCS_BUILTINS) $(SRCS_SYMTAB)

OBJS=$(SRCS:%.c=$(BUILD_DIR)/%.o)

# output file name
TARGET=minishell

# default target (when we call make with no arguments)
.PHONY: all
all: prep-build $(TARGET)

prep-build:
	mkdir -p $(BUILD_DIR)/builtins
	mkdir -p $(BUILD_DIR)/symtab

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

# target to auto-generate header file dependencies for source files
depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CC) $(CFLAGS) -MM $^ > ./.depend;

include .depend

#compile C source files
$(BUILD_DIR)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

# clean target
.PHONY: clean
clean:
	$(RM) $(OBJS) core .depend
	$(RM) -r $(BUILD_DIR)

fclean:
	$(RM) $(OBJS) $(TARGET) core .depend
	$(RM) -r $(BUILD_DIR)	