/*
Copyright (C) 2014 Toni Spets

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

typedef struct tmenu_item {
	char *text;
	char *command;
	qbool alt;

	struct tmenu_item *next;
} tmenu_item;

typedef struct tmenu_header {
	char *name;

	int cmdcount;
	struct tmenu_item *items;
	struct tmenu_header *next;
} tmenu_header;

void TMenu_Init (void);
qbool TMenu_IsOpen (void);
void TMenu_Draw (void);

#define TMENU_INSERT(list, el)							\
	(el)->next = (list);							\
	(list) = (el);								\

#define TMENU_LAST(list, el)							\
	el = (list);								\
	while (el && el->next) el = el->next;					\

#define TMENU_FOREACH(list, el)							\
	for (el = (list); el != NULL; el = el->next)

#define TMENU_REMOVE(list, el)							\
	if ((list) == (el))							\
	{									\
		(list) = (el)->next;						\
	}									\
	else									\
	{									\
		void *_eltmp = (el);						\
		(el) = (list);							\
		do {								\
			if ((el)->next == _eltmp)				\
			{							\
				(el)->next = (el)->next->next;			\
				(el) = _eltmp;					\
				break;						\
			}							\
			(el) = (el)->next;					\
		} while(el);							\
		(el) = _eltmp;							\
	}
