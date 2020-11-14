
      ███╗░░░███╗ ██╗ ███╗░░██╗ ██╗ ░██████╗ ██╗░░██╗ ███████╗ ██╗░░░░░ ██╗░░░░░
      ████╗░████║ ██║ ████╗░██║ ██║ ██╔════╝ ██║░░██║ ██╔════╝ ██║░░░░░ ██║░░░░░
      ██╔████╔██║ ██║ ██╔██╗██║ ██║ ╚█████╗░ ███████║ █████╗░░ ██║░░░░░ ██║░░░░░
      ██║╚██╔╝██║ ██║ ██║╚████║ ██║ ░╚═══██╗ ██╔══██║ ██╔══╝░░ ██║░░░░░ ██║░░░░░
      ██║░╚═╝░██║ ██║ ██║░╚███║ ██║ ██████╔╝ ██║░░██║ ███████╗ ███████╗ ███████╗
      ╚═╝░░░░░╚═╝ ╚═╝ ╚═╝░░╚══╝ ╚═╝ ╚═════╝░ ╚═╝░░╚═╝ ╚══════╝ ╚══════╝ ╚══════╝

   # Компоненты оболочки Linux
   
   Оболочка-это сложная часть программного обеспечения, содержащая множество различных частей.
   
   Основной частью любой оболочки Linux является интерпретатор командной строки, или CLI. Эта часть служит двум целям: она читает и анализирует пользовательские команды, а затем выполняет проанализированные команды. Вы можете думать о самом CLI как о двух частях: синтаксическом анализаторе (или front-end) и исполнителе (или back-end).
   
   Анализатор сканирует входные данные и разбивает их на маркеры. Токен состоит из одного или нескольких символов (букв, цифр, символов) и представляет собой единую единицу ввода. Например, токен может быть именем переменной, ключевым словом, числом или арифметическим оператором.
   
   Синтаксический анализатор берет эти маркеры, группирует их вместе и создает специальную структуру, которую мы называем абстрактным синтаксическим деревом, или AST.
   
   Вы можете думать о AST как о высокоуровневом представлении командной строки, которую вы дали оболочке. Синтаксический анализатор принимает AST и передает его исполнителю, который считывает AST и выполняет проанализированную команду.
   
   Другой частью оболочки является пользовательский интерфейс, который обычно работает, когда оболочка находится в интерактивном режиме, например, когда вы вводите команды в командной строке оболочки. Здесь оболочка работает в цикле, который мы знаем как цикл Read-Eval-Print, или REPL. Как следует из названия цикла, оболочка считывает входные данные, анализирует и выполняет их, затем выполняет цикл для чтения следующей команды и так далее, пока вы не введете команду, такую как выход, завершение работы или перезагрузка.
   
   Большинство оболочек реализуют структуру, известную как таблица символов, которая используется оболочкой для хранения информации о переменных, а также их значений и атрибутов. Мы реализуем таблицу символов в Части II этого руководства.
   
   Оболочки Linux также имеют средство истории, которое позволяет пользователю получить доступ к самым последним введенным командам, а затем редактировать и повторно выполнять команды без особого набора текста. Оболочка также может содержать встроенные утилиты, которые представляют собой специальный набор команд, реализуемых как часть самой программы оболочки. Встроенные утилиты включают обычно используемые команды, такие как cd, fg и bg. Мы будем реализовывать многие из встроенных утилит по мере продвижения этого урока.
   
   Теперь, когда мы знаем основные компоненты типичной оболочки Linux, давайте начнем строить нашу собственную оболочку.
   
   # Наш Первый Shell
   
   Наша первая версия оболочки не будет делать ничего необычного; она просто напечатает строку запроса, прочитает строку ввода, а затем эхом вернет ввод обратно на экран. В последующих частях этого урока мы добавим возможность анализировать и выполнять команды, циклы, условные выражения и многое другое.
   
   Давайте начнем с создания каталога для этого проекта. Обычно я использую ~/projects/ для своих проектов, но не стесняйтесь использовать любой путь, который вам удобен.
   
   Первое, что мы сделаем, это напишем наш базовый цикл REPL. Создайте файл с именем main.c (используя touch main.c), затем откройте его с помощью вашего любимого текстового редактора. Введите следующий код в свой main.c:

      #include <stdio.h>
      #include <stdlib.h>
      #include <errno.h>
      #include <string.h>
      #include "shell.h"
      
      int main(int argc, char **argv)
      {
         char *cmd;
         
         do
         {
            print_prompt1();
            cmd = read_cmd();
            if(!cmd)
            {
               exit(EXIT_SUCCESS);
            }
            if(cmd[0] == '\0' || strcmp(cmd, "\n") == 0)
            {
               free(cmd);
               continue;
            }
            if(strcmp(cmd, "exit\n") == 0)
            {
               free(cmd);
               break;
            }
            printf("%s\n", cmd);
            free(cmd);
         } while(1);
         exit(EXIT_SUCCESS);
      }

   Наша функция main() довольно проста (я выделил наиболее важные строки, которые мы обсудим далее), так как ей нужно только реализовать цикл REPL. Сначала мы печатаем приглашение оболочки, а затем читаем команду (пока давайте определим команду как строку ввода, заканчивающуюся на \n). Если есть ошибка чтения команды, мы выходим из оболочки. Если команда пуста (то есть пользователь нажал ENTER, ничего не записывая, мы пропускаем этот ввод и продолжаем цикл. Если команда exit, мы выходим из оболочки. В противном случае мы эхом возвращаем команду, освобождаем память, которую мы использовали для хранения команды, и продолжаем цикл.
   
   Довольно просто, не так ли?
   
   Наша функция main() вызывает две пользовательские функции print_prompt1() и read_cmd(). Первая функция выводит строку приглашения, а вторая считывает следующую строку ввода. Давайте более подробно рассмотрим эти две функции.
   
   # Строки Запроса На Печать
   
   Мы сказали, что оболочка выводит строку запроса перед чтением каждой команды. На самом деле существует пять различных типов строки приглашения: PS0 , PS1, PS2, PS3 и PS4. Нулевая строка, PS0, используется только bash, поэтому мы не будем рассматривать ее здесь. Остальные четыре строки печатаются в определенное время, когда оболочка хочет передать определенные сообщения пользователю. В этом разделе мы поговорим о PS1 и PS2. Остальное придет позже, когда мы обсудим более продвинутые темы оболочки.
   
   Теперь создайте исходный файл prompt.c и введите следующий код:

      #include <stdio.h>
      #include "shell.h"
      
      void print_prompt1(void)
      {
         fprintf(stderr, "$ ");
      }
      
      void print_prompt2(void)
      {
         fprintf(stderr, "> ");
      } 

Первая функция выводит первую строку приглашения, или PS1, которую вы обычно видите, когда оболочка ожидает ввода команды. Вторая функция печатает вторую строку приглашения, или PS2, которая печатается оболочкой при вводе многострочной команды (Подробнее об этом ниже).

Далее, Давайте прочитаем некоторые пользовательские данные.

# Чтение Пользовательского Ввода

Откройте файл main.c и введите следующий код в конце, сразу после функции main() :

      char *read_cmd(void)
      {
         char buf[1024];
         char *ptr = NULL;
         char ptrlen = 0;

         while(fgets(buf, 1024, stdin))
         {
            int buflen = strlen(buf);

            if(!ptr)
            {
               ptr = malloc(buflen+1);
            }
            else
            {
               char *ptr2 = realloc(ptr, ptrlen+buflen+1);

               if(ptr2)
               {
                  ptr = ptr2;
               }
               else
               {
                  free(ptr);
                  ptr = NULL;
               }
            }

            if(!ptr)
            {
               fprintf(stderr, "error: failed to alloc buffer: %s\n", strerror(errno));
               return NULL;
            }

            strcpy(ptr+ptrlen, buf);

            if(buf[buflen-1] == '\n')
            {
               if(buflen == 1 || buf[buflen-2] != '\\')
               {
                  return ptr;
               }

               ptr[ptrlen+buflen-2] = '\0';
               buflen -= 2;
               print_prompt2();
            }

            ptrlen += buflen;
         }

         return ptr;
      }

Здесь мы считываем входные данные из stdin в 1024-байтовых блоках и храним их в буфере. В первый раз, когда мы читаем входные данные (первый фрагмент для текущей команды), мы создаем наш буфер с помощью malloc(). Для последующих фрагментов мы расширяем буфер с помощью функции realloc(). Здесь мы не должны сталкиваться с какими-либо проблемами памяти, но если что-то происходит неправильно, мы печатаем сообщение об ошибке и возвращаем NULL. Если все идет хорошо, мы копируем фрагмент ввода, который только что прочитали от пользователя, в наш буфер и соответствующим образом корректируем наши указатели.

Последний блок кода интересен. Чтобы понять, зачем нам нужен этот блок кода, рассмотрим следующий пример. Допустим, вы хотите ввести очень-очень длинную строку ввода:

      echo "This is a very long line of input, one that needs to span two, three, or perhaps even more lines of input, so that we can feed it to the shell"
      
 Это глупый пример, но он прекрасно демонстрирует то, о чем мы говорим. Чтобы ввести такую длинную команду, мы можем записать все это в одной строке (как мы сделали здесь), что является громоздким и уродливым процессом. Или мы можем нарезать леску на более мелкие кусочки и скормить эти кусочки Shell, по одному кусочку за раз:
 
      echo "This is a very long line of input, \
         one that needs to span two, three, \
         or perhaps even more lines of input, \
         so that we can feed it to the shell"
         
После ввода первой строки и чтобы оболочка знала, что мы не закончили ввод, мы заканчиваем каждую строку символом обратной косой черты \\, за которым следует новая строка (я также сделал отступы в строках, чтобы сделать их более читабельными). Мы называем это экранированием символа новой строки. Когда оболочка видит экранированную новую строку, она знает, что ей нужно отбросить два символа и продолжить чтение ввода.

Теперь давайте вернемся к нашей функции read_cmd (). Мы обсуждали последний блок кода, который гласит:

        if(buf[buflen-1] == '\n')
        {
            if(buflen == 1 || buf[buflen-2] != '\\')
            {
                return ptr;
            }

            ptr[ptrlen+buflen-2] = '\0';
            buflen -= 2;
            print_prompt2();
        }
        
 Здесь мы проверяем, заканчивается ли входной сигнал, который мы получили в буфере, на \n, и если да, то экранируется ли \n символом обратной косой черты \\. Если последний \n не экранирован, то входная строка завершена, и мы возвращаем ее в функцию main (). В противном случае мы удаляем два символа (\\ и \n), распечатываем PS2 и продолжаем чтение ввода.
 
# Компиляция Shell

С приведенным выше кодом наша нишевая оболочка почти готова к компиляции. Мы просто добавим заголовочный файл с нашими прототипами функций, прежде чем приступим к компиляции оболочки. Этот шаг необязателен, но он значительно улучшает читаемость нашего кода и предотвращает появление нескольких предупреждений компилятора.
Создайте исходный файл shell.h и введите следующий код:

      #ifndef SHELL_H
      #define SHELL_H
      
      void print_prompt1(void);
      void print_prompt2(void);
      
      char *read_cmd(void);
      
      #endif 
      
 # Что такое простая команда?
 
Простая команда состоит из списка слов, разделенных пробелами (пробел, табуляция, новая строка). Первое слово-это имя команды, и оно обязательно (в противном случае оболочка не будет иметь команды для разбора и выполнения!). Второе и последующие слова являются необязательными. Если они присутствуют, они формируют аргументы, которые мы хотим, чтобы оболочка передала выполняемой команде.

Например, следующая команда: ls-l состоит из двух слов: ls (имя команды) и-l (первый и единственный аргумент). Аналогично команда: gcc-o shell main.c prompt.c (которую мы использовали в части I для компиляции нашей оболочки) состоит из 5 слов: имени команды и списка из 4 аргументов.

Теперь, чтобы иметь возможность выполнять простые команды, наша оболочка должна выполнить следующие шаги:

Сканируйте ввод, по одному символу за раз, чтобы найти следующий маркер. Мы называем этот процесс лексическим сканированием, и часть оболочки, которая выполняет эту задачу, известна как лексический сканер, или просто сканер.

Извлеките входные токены. Мы называем эти входные разбора.

Проанализируйте маркеры и создайте абстрактное синтаксическое дерево, или AST. Часть оболочки, ответственная за это, называется синтаксическим анализатором.

Выполните АСТ. Это работа исполнителя.

На рисунке ниже показаны шаги, выполняемые оболочкой для анализа и выполнения команд. Вы можете видеть, что рисунок содержит больше шагов, чем показано в приведенном выше списке, и это нормально. По мере того как наша оболочка растет и становится более сложной, мы будем добавлять другие шаги по мере необходимости.

Теперь давайте подробно рассмотрим каждый из вышеперечисленных четырех шагов и рассмотрим код, который нам понадобится для реализации этих функций в нашей оболочке.

# Сканирование входных данных

Чтобы получить следующий маркер, мы должны иметь возможность сканировать наши входные данные, по одному символу за раз, чтобы мы могли идентифицировать символы, которые могут быть частью маркера, и те, которые являются символами-разделителями. Символ-разделитель-это символ, который обозначает конец маркера (и, возможно, начало другого маркера). Как правило, разделителями являются символы пробела (пробел, табуляция, новая строка), но также могут быть и другие символы, такие как ; и &.

В общем, сканирование входных данных означает, что мы должны быть в состоянии:

Извлечь следующий символ из входных данных.

Вернуть последний символ, который мы прочитали, обратно на вход.

Lookahead (или peek), чтобы проверить следующий символ, фактически не извлекая его.

Пропустить пробелы.

Мы определим функции для выполнения всех этих задач в течение минуты. Но сначала давайте поговорим об абстрагировании входных данных.
Помните функцию read_cmd (), которую мы определили в части I этого урока? Это была функция, которую мы использовали, чтобы прочитать ввод пользователя и вернуть его в виде строки malloc'D. Мы могли бы передать эту строку непосредственно на наш сканер, но это сделало бы процесс сканирования немного громоздким. В частности, сканеру было бы очень трудно запомнить последний символ, который он дал нам, чтобы он мог пройти мимо этого символа и дать нам следующий символ.

Чтобы облегчить работу сканера, мы абстрагируем наши входные данные, передавая входную строку как часть структуры struct source_s, которую мы определим в исходном файле source.h. Создайте этот файл в исходном каталоге, затем откройте его в своем любимом текстовом редакторе и добавьте следующий код:

Проанализируйте маркеры и создайте абстрактное синтаксическое дерево, или AST. Часть оболочки, ответственная за это, называется синтаксическим анализатором.

      #ifndef SOURCE_H
      #define SOURCE_H
      
      #define EOF             (-1)
      #define ERRCHAR         ( 0)
      #define INIT_SRC_POS    (-2)
      
      struct source_s
      {   
            char *buffer;       /* the input text */
            long bufsize;       /* size of the input text */
            long  curpos;       /* absolute char position in source */
      };
      
      char next_char(struct source_s *src);
      void unget_char(struct source_s *src);
      char peek_char(struct source_s *src);
      void skip_white_spaces(struct source_s *src);
      
      #endif
      
Сосредоточившись на определении структуры, вы можете увидеть, что наша структура source_s содержит указатель на входную строку, в дополнение к двум длинным полям, которые содержат информацию о длине строки и нашей текущей позиции в строке (откуда мы получим следующий символ).

Теперь создайте еще один файл с именем source.c, в который вы должны добавить следующий код:

      #include <errno.h>
      #include "shell.h"
      #include "source.h"
      
      void unget_char(struct source_s *src)
      {
            if(src->curpos < 0)
            {
                  return;
            }
            src->curpos--;
      }
      
      char next_char(struct source_s *src)
      {
            if(!src || !src->buffer)
            {
                  errno = ENODATA;
                  return ERRCHAR;
            }
            char c1 = 0;
            if(src->curpos == INIT_SRC_POS)
            {
                  src->curpos  = -1;
            }
            else
            {
                  c1 = src->buffer[src->curpos];
            }
            if(++src->curpos >= src->bufsize)
            {
                  src->curpos = src->bufsize;
                  return EOF;
            }
            return src->buffer[src->curpos];
      }
      
      char peek_char(struct source_s *src)
      {
            if(!src || !src->buffer)
            {
                  errno = ENODATA;
                  return ERRCHAR;
            }
            long pos = src->curpos;
            if(pos == INIT_SRC_POS)
            {
                  pos++;
            }
            pos++;
            if(pos >= src->bufsize)
            {
                  return EOF;
            }
            return src->buffer[pos];
      }
      
      void skip_white_spaces(struct source_s *src)
      {
            char c;
            
            if(!src || !src->buffer)
            {
                  return;
            }
            while(((c = peek_char(src)) != EOF) && (c == ' ' || c == '\t'))
            {
                  next_char(src);
            }
      }

Функция unget_char() возвращает (или разгружает) последний символ, который мы извлекли из входных данных, обратно к источнику входных данных. Он делает это, просто манипулируя указателями исходной структуры. Вы увидите преимущества этой функции позже в этой серии.

Функция next_char() возвращает следующий символ ввода и обновляет указатель источника, так что следующий вызов функции next_char() возвращает следующий входной символ. Когда мы достигаем последнего символа во входных данных, функция возвращает специальный символ EOF, который мы определили как -1 в source.h выше.

Функция peek_char() аналогична функции next_char() в том, что она возвращает следующий символ ввода. Единственное отличие состоит в том, что функция peek_char() не обновляет исходный указатель, так что следующий вызов функции next_char() возвращает тот же входной символ, который мы только что просмотрели. Вы увидите преимущество подглядывания ввода позже в этой серии.

Наконец, функция skip_white_spaces() пропускает все символы пробела. Это поможет нам, когда мы закончим чтение маркера, и мы хотим пропустить пробелы разделителей, прежде чем читать следующий маркер.

# Токенизация Входных Данных

Теперь, когда у нас есть функции нашего сканера, мы будем использовать эти функции для извлечения входных маркеров. Мы начнем с определения новой структуры, которую будем использовать для представления наших токенов.
Создайте файл scanner.h в исходном каталоге, откройте его и добавьте следующий код:

      #ifndef SCANNER_H
      #define SCANNER_Hl
      
      struct token_s
      {
            struct source_s *src;       /* source of input */
            int    text_len;            /* length of token text */
            char   *text;               /* token text */
      };
      
      /* the special EOF token, which indicates the end of input */
      extern struct token_s eof_token;
      struct token_s *tokenize(struct source_s *src);
      void free_token(struct token_s *tok);
      
      #endif 
      
Сосредоточившись на определении структуры, наша структура token_s содержит указатель на структуру source_s, которая содержит ваши входные данные. Структура также содержит указатель на текст токена и поле, которое сообщает нам длину этого текста (так что нам не нужно повторно вызывать strlen() для текста токена).

Далее мы напишем функцию tokenize (), которая будет извлекать следующий токен из входных данных. Мы также напишем некоторые вспомогательные функции, которые помогут нам работать с входными токенами.

В исходном каталоге, создайте файл под названием сканер.C и введите следующий код:

      #include <stdlib.h>
      #include <stdio.h>
      #include <string.h>
      #include <errno.h>
      #include "shell.h"
      #include "scanner.h"
      #include "source.h"
      
      char *tok_buf = NULL;
      int   tok_bufsize  = 0;
      int   tok_bufindex = -1;
      
      /* special token to indicate end of input */
      struct token_s eof_token = 
      {
            .text_len = 0,
      };
 
      void add_to_buf(char c)
      {
            tok_buf[tok_bufindex++] = c;
            if(tok_bufindex >= tok_bufsize)
            {
                  char *tmp = realloc(tok_buf, tok_bufsize*2);
                  if(!tmp)
                  {
                        errno = ENOMEM;
                        return;
                  }
                  tok_buf = tmp;
                  tok_bufsize *= 2;
            }
      }
      
      struct token_s *create_token(char *str)
      {
            struct token_s *tok = malloc(sizeof(struct token_s));
    
            if(!tok)
            {
                  return NULL;
            }
            memset(tok, 0, sizeof(struct token_s));
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
      
      void free_token(struct token_s *tok)
      {
            if(tok->text)
            {
                  free(tok->text);
            }
            free(tok);
      }
      
      struct token_s *tokenize(struct source_s *src)
      {
            int  endloop = 0;
            if(!src || !src->buffer || !src->bufsize)
            {
                  errno = ENODATA;
                  return &eof_token;
            }
    
            if(!tok_buf)
            {
                  tok_bufsize = 1024;
                  tok_buf = malloc(tok_bufsize);
                  if(!tok_buf)
                  {
                        errno = ENOMEM;
                        return &eof_token;
                  }
            }
            tok_bufindex     = 0;
            tok_buf[0]       = '\0';
            char nc = next_char(src);
            if(nc == ERRCHAR || nc == EOF)
            {
                  return &eof_token;
            }
            do
            {
                  switch(nc)
                  {
                        case ' ':
                        
                        case '\t':
                              if(tok_bufindex > 0)
                              {
                                    endloop = 1;
                              }
                              break;
                
                        case '\n':
                              if(tok_bufindex > 0)
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
            if(tok_bufindex == 0)
            {
                  return &eof_token;
            }
    
            if(tok_bufindex >= tok_bufsize)
            {
                  tok_bufindex--;
            }
            tok_buf[tok_bufindex] = '\0';
            struct token_s *tok = create_token(tok_buf);
            if(!tok)
            {
                  fprintf(stderr, "error: failed to alloc buffer: %s\n",
                        strerror(errno));
                  return &eof_token;
            } 
            tok->src = src;
            return tok;
      }

Глобальные переменные, которые мы определили в этом файле, служат следующим целям:

tok_buf-это указатель на буфер, в котором мы будем хранить текущий токен.

tok_bufsize-это количество байтов, которые мы выделяем в буфер.

tok_bufindex-это текущий индекс буфера (т. е. он говорит нам, где добавить следующий входной символ в буфер).

eof_token-это специальный токен, который мы будем использовать для сигнализации конца файла/ввода (EOF).

Теперь давайте посмотрим на функции, которые мы определили в этом файле.
Функция add_to_buf() добавляет один символ в буфер токенов. Если буфер заполнен, функция позаботится о его расширении.

Функция create_token() принимает строку и преобразует ее в структуру struct token_s. Он заботится о выделении памяти для структуры и текста токена и заполняет поля-члены структуры.

Функция free_token() освобождает память, используемую структурой токена, а также память, используемую для хранения текста токена.

Функция tokenize () - это сердце и душа нашего лексического сканера, и код этой функции довольно прямолинеен. Он начинается с выделения памяти для нашего буфера токенов (если это еще не сделано), а затем инициализирует наш буфер токенов и исходные указатели. Затем он вызывает функцию next_char() для извлечения следующего входного символа. Когда мы достигаем конца ввода, tokenize() возвращает специальный eof_token, который отмечает конец ввода.

Затем функция выполняет циклическое считывание входных символов по одному за раз. Если он встречает символ пробела, он проверяет буфер токенов, чтобы увидеть, пуст он или нет. Если буфер не пуст, мы разделяем текущий токен и выходим из цикла. В противном случае мы пропускаем пробелы и переходим к началу следующего маркера.

После того как мы получили наш токен, tokenize() вызывает create_token(), передавая ему текст токена (который мы сохранили в буфере). Текст токена преобразуется 
Мы, конечно, добились большого прогресса в этой части, но наша оболочка все еще не готова анализировать и выполнять команды. Поэтому сейчас мы не будем компилировать оболочку. Наша следующая компиляция будет в конце части III, после того, как мы реализуем наш синтаксический анализатор и наш исполнитель.

# Разбор Простых Команд
В предыдущей части этого урока мы реализовали наш лексический сканер. Теперь давайте обратимся к синтаксическому анализатору.

Напомним, что синтаксический анализатор-это часть интерпретатора командной строки, которая вызывает лексический сканер для извлечения маркеров, а затем строит абстрактное синтаксическое дерево, или AST, из этих маркеров. АСТ-это то, что мы передадим исполнителю, чтобы он был, ну, казнен.

Наш парсер будет содержать только одну функцию-parse_simple_command(). В следующих частях этого урока мы добавим дополнительные функции, позволяющие нашей оболочке анализировать циклы и условные выражения.

Итак, давайте начнем кодировать наш парсер. Вы можете начать с создания файла parser.h в исходном каталоге, в который вы добавите следующий код:

      #ifndef PARSER_H
      #define PARSER_H

      #include "scanner.h"    /* struct token_s */
      #include "source.h"     /* struct source_s */

      struct node_s *parse_simple_command(struct token_s *tok);

      #endif

Ничего особенного, просто объявляем нашу единственную функцию парсера.

Затем создайте parser.c и добавьте к нему следующий код:

      #include <unistd.h>
      #include "shell.h"
      #include "parser.h"
      #include "scanner.h"
      #include "node.h"
      #include "source.h"

      struct node_s *parse_simple_command(struct token_s *tok)
      {
            if(!tok)
            {
                  return NULL;
            }
            
            struct node_s *cmd = new_node(NODE_COMMAND);
            if(!cmd)
            {
                  free_token(tok);
                  return NULL;
            }
            
            struct source_s *src = tok->src;
            
            do
            {
                  if(tok->text[0] == '\n')
                  {
                        free_token(tok);
                        break;
                  }
                  struct node_s *word = new_node(NODE_VAR);
                  if(!word)
                  {
                        free_node_tree(cmd);
                        free_token(tok);
                        return NULL;
                  }
                  set_node_val_str(word, tok->text);
                  add_child_node(cmd, word);
                  free_token(tok);
            } while((tok = tokenize(src)) != &eof_token);
            return cmd;
      }

Довольно просто, правда? Чтобы разобрать простую команду, нам нужно только вызвать tokenize() для извлечения входных токенов, один за другим, пока мы не получим токен новой строки (который мы тестируем в строке, которая гласит: if(tok->text[0] == '\n')), или мы достигнем конца нашего ввода (мы знаем, что это произошло, когда мы получили токен eof_token. См. условное выражение цикла в нижней части предыдущего списка). Мы используем входные маркеры для создания AST, который представляет собой древовидную структуру, содержащую информацию о компонентах команды. Деталей должно быть достаточно, чтобы исполнитель мог правильно выполнить команду. 

Каждый узел в AST команды должен содержать информацию о входном токене, который он представляет (например, текст исходного токена). Узел также должен содержать указатели на его дочерние узлы (если узел является корневым узлом), а также на его родственные узлы (если узел является дочерним узлом). Поэтому нам нужно будет определить еще одну структуру, struct node_s, которую мы будем использовать для представления узлов в нашем AST.

Идти вперед и создать новый файл, узел.H и добавьте в него следующий код:

      #ifndef NODE_H
      #define NODE_H

      enum node_type_e
      {
            NODE_COMMAND,           /* simple command */
            NODE_VAR,               /* variable name (or simply, a word) */
      };

      enum val_type_e
      {
            VAL_SINT = 1,       /* signed int */
            VAL_UINT,           /* unsigned int */
            VAL_SLLONG,         /* signed long long */
            VAL_ULLONG,         /* unsigned long long */
            VAL_FLOAT,          /* floating point */
            VAL_LDOUBLE,        /* long double */
            VAL_CHR,            /* char */
            VAL_STR,            /* str (char pointer) */
      };

      union symval_u
      {
            long               sint;
            unsigned long      uint;
            long long          sllong;
            unsigned long long ullong;
            double             sfloat;
            long double        ldouble;
            char               chr;
            char              *str;
      };
      
      struct node_s
      {
            enum   node_type_e type;    /* type of this node */
            enum   val_type_e val_type; /* type of this node's val field */
            union  symval_u val;        /* value of this node */
            int    children;            /* number of child nodes */
            struct node_s *first_child; /* first child node */
            struct node_s *next_sibling, *prev_sibling;
                                          /*
                                          * if this is a child node, keep
                                          * pointers to prev/next siblings
                                          */
      };

      struct  node_s *new_node(enum node_type_e type);
      void    add_child_node(struct node_s *parent, struct node_s *child);
      void    free_node_tree(struct node_s *node);
      void    set_node_val_str(struct node_s *node, char *val);

      #endif

Перечисление node_type_e определяет типы наших узлов AST. На данный момент нам нужны только два типа. Первый тип представляет корневой узел AST простой команды, в то время как второй тип представляет дочерние узлы простой команды (которые содержат имя команды и аргументы). В следующих частях этого руководства мы добавим в это перечисление дополнительные типы узлов.

Перечисление val_type_e представляет типы значений, которые мы можем хранить в данной структуре узлов. Для простых команд мы будем использовать только строки (тип перечисления VAL_STR). Позже в этой серии мы будем использовать другие типы при обработке других типов сложных команд.

Объединение symval_u представляет значение, которое мы можем хранить в данной узловой структуре. Каждый узел может иметь только один тип значения, например символьную строку или числовое значение. Мы получаем доступ к значению узла, ссылаясь на соответствующий член объединения (sint для длинных целых чисел со знаком, str для строк и т. д.).

Структура struct node_s представляет собой узел AST. Он содержит поля, которые сообщают нам о типе узла, типе значения узла, а также само значение. Если это корневой узел, то поле children содержит количество дочерних узлов, а first_child указывает на первый дочерний узел (в противном случае оно будет равно NULL). Если это дочерний узел, мы можем обойти узлы AST, следуя указателям next_sibling и prev_sibling.

Если мы хотим получить значение узла, нам нужно проверить поле val_type и, в соответствии с тем, что мы там находим, получить доступ к соответствующему члену поля val. Для простых команд все узлы будут иметь следующие атрибуты:

type => NODE_COMMAND (корневой узел) или NODE_VAR (имя команды и список аргументов)

val_type => VAL_STR

val.str => указатель на строковое значение

Теперь давайте напишем некоторые функции, которые помогут нам работать со структурами узлов.

Создайте файл с именем node.c и добавьте следующий код:

      #include <stdlib.h>
      #include <string.h>
      #include <stdio.h>
      #include <errno.h>
      #include "shell.h"
      #include "node.h"
      #include "parser.h"

      struct node_s *new_node(enum node_type_e type)
      {
            struct node_s *node = malloc(sizeof(struct node_s));

            if(!node)
            {
                  return NULL;
            }
            
            memset(node, 0, sizeof(struct node_s));
            node->type = type;
            
            return node;
      }

      void add_child_node(struct node_s *parent, struct node_s *child)
      {
            if(!parent || !child)
            {
                  return;
            }
            if(!parent->first_child)
            {
                  parent->first_child = child;
            }
            else
            {
                  struct node_s *sibling = parent->first_child;
            
                  while(sibling->next_sibling)
                  {
                        sibling = sibling->next_sibling;
                  }
            
                  sibling->next_sibling = child;
                  child->prev_sibling = sibling;
            }
            parent->children++;
      }

      void set_node_val_str(struct node_s *node, char *val)
      {
            node->val_type = VAL_STR;
            if(!val)
            {
                  node->val.str = NULL;
            }
            else
            {
                  char *val2 = malloc(strlen(val)+1);
            
                  if(!val2)
                  {
                        node->val.str = NULL;
                  }
                  else
                  {
                        strcpy(val2, val);
                        node->val.str = val2;
                  }
            }
      }

      void free_node_tree(struct node_s *node)
      {
            if(!node)
            {
                  return;
            }
            struct node_s *child = node->first_child;
            
            while(child)
            {
                  struct node_s *next = child->next_sibling;
                  free_node_tree(child);
                  child = next;
            }
            
            if(node->val_type == VAL_STR)
            {
                  if(node->val.str)
                  {
                        free(node->val.str);
                  }
            }
            free(node);
      } 

Функция new_node() создает новый узел и задает его поле типа.

Функция add_child_node() расширяет AST простой команды, добавляя новый дочерний узел и увеличивая поле дочерних узлов корневого узла. Если корневой узел не имеет дочерних элементов, то новый дочерний элемент назначается полю first_child корневого узла. В противном случае ребенок добавляется в конец списка детей.

Функция set_node_val_str() устанавливает значение узла в заданную строку. Он копирует строку во вновь выделенное пространство памяти, а затем устанавливает поля val_type и val.str соответственно. В будущем мы определим аналогичные функции, позволяющие устанавливать значения узлов для различных типов данных, таких как целые числа и плавающие точки.

Функция free_node_tree() освобождает память, используемую узловой структурой. Если у узла есть дочерние элементы, функция вызывается рекурсивно, чтобы освободить каждый из них.

Это все для парсера. Теперь давайте напишем нашу команду executor.
# Выполнение Простых Команд

Подобно нашему синтаксическому анализатору, исполнитель будет содержать только одну функцию do_simple_command(). В следующих частях этого урока мы добавим дополнительные функции, которые позволят нам выполнять все виды команд, такие как циклы и условные выражения.

Создайте файл с именем executor.h и добавьте следующий код:
