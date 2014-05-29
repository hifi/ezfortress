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

#include "quakedef.h"
#include "cl_tmenu.h"

static tmenu_header *tmenu_list;
static tmenu_header *tmenu_current;

static tmenu_header *tmenu_find_by_name(const char *name)
{
	tmenu_header *menu;

	TMENU_FOREACH(tmenu_list, menu)
	{
		if (strcmp(menu->name, name) == 0)
		{
			return menu;
		}
	}

	return NULL;
}

static void tmenu_del_all()
{
	tmenu_header *menu;
	tmenu_item *item;

	menu = tmenu_list;
	tmenu_list = NULL;

	while (menu)
	{
		tmenu_header *next_menu = menu->next;

		item = menu->items;

		while (item)
		{
			tmenu_item *next_item = item->next;
			Q_free(item->text);
			Q_free(item->command);
			Q_free(item);
			item = next_item;
		}

		Q_free(menu->name);
		Q_free(menu);

		menu = next_menu;
	}
}

void TMenu_Del_f (void)
{
	tmenu_header *menu;
	tmenu_item *item;

	if (Cmd_Argc() < 2)
	{
		Com_Printf("usage: %s <name>\n", Cmd_Argv(0));
		return;
	}

	if (strcmp(Cmd_Argv(1), "all") == 0)
	{
		tmenu_del_all();
		Com_Printf("All text menus have been deleted.\n");
		return;
	}

	if (!(menu = tmenu_find_by_name(Cmd_Argv(1))))
	{
		return;
	}

	item = menu->items;

	while (item)
	{
		tmenu_item *next = item->next;
		Q_free(item->text);
		Q_free(item->command);
		Q_free(item);
		item = next;
	}

	TMENU_REMOVE(tmenu_list, menu);

	Q_free(menu->name);
	Q_free(menu);
}

void TMenu_Add_f (void)
{
	tmenu_header *menu, *last;
	int i;

	if (Cmd_Argc() < 2)
	{
		Com_Printf("usage: %s <name>\n", Cmd_Argv(0));
		return;
	}

	TMenu_Del_f();

	menu = Q_calloc(1, sizeof(tmenu_header));
	menu->name = Q_strdup(Cmd_Argv(1));

	TMENU_LAST(tmenu_list, last);

	if (last)
		last->next = menu;
	else
		tmenu_list = menu;
}

void TMenu_List_f (void)
{
	tmenu_header *menu;

	Com_Printf("Defined text menus:\n");
	TMENU_FOREACH(tmenu_list, menu)
	{
		Com_Printf("  %s\n", menu->name);
	}
}

void TMenu_PushCmd_f (void)
{
	tmenu_header *menu;
	tmenu_item *item, *last;

	if (Cmd_Argc() < 3)
	{
		Com_Printf("usage: %s <name> <command> [text]\n", Cmd_Argv(0));
		return;
	}

	if (!(menu = tmenu_find_by_name(Cmd_Argv(1))))
	{
		Com_Printf("No such menu.\n");
		return;
	}

	if (menu->cmdcount >= 10)
	{
		return;
	}

	item = Q_calloc(1, sizeof(tmenu_item));
	item->command = Q_strdup(Cmd_Argv(2));
	if (Cmd_Argc() > 3)
		item->text = Q_strdup(Cmd_Argv(3));

	TMENU_LAST(menu->items, last);

	if (last)
		last->next = item;
	else
		menu->items = item;

	menu->cmdcount++;
}

void TMenu_PushBlank_f (void)
{
	tmenu_header *menu;
	tmenu_item *item, *last;

	if (Cmd_Argc() < 2)
	{
		Com_Printf("usage: %s <name>\n", Cmd_Argv(0));
		return;
	}

	if (!(menu = tmenu_find_by_name(Cmd_Argv(1))))
	{
		Com_Printf("No such menu.\n");
		return;
	}

	item = Q_calloc(1, sizeof(tmenu_item));

	TMENU_LAST(menu->items, last);

	if (last)
		last->next = item;
	else
		menu->items = item;
}

void TMenu_PushText_f (void)
{
	tmenu_header *menu;
	tmenu_item *item, *last;

	if (Cmd_Argc() < 2)
	{
		Com_Printf("usage: %s <name> <text>\n", Cmd_Argv(0));
		return;
	}

	if (!(menu = tmenu_find_by_name(Cmd_Argv(1))))
	{
		Com_Printf("No such menu.\n");
		return;
	}

	item = Q_calloc(1, sizeof(tmenu_item));

	TMENU_LAST(menu->items, last);
	item->text = Q_strdup(Cmd_Argv(2));

	if (last)
		last->next = item;
	else
		menu->items = item;
}

void TMenu_PushTextAlt_f (void)
{
	tmenu_header *menu;
	tmenu_item *item, *last;

	if (Cmd_Argc() < 2)
	{
		Com_Printf("usage: %s <name> <text>\n", Cmd_Argv(0));
		return;
	}

	if (!(menu = tmenu_find_by_name(Cmd_Argv(1))))
	{
		Com_Printf("No such menu.\n");
		return;
	}

	item = Q_calloc(1, sizeof(tmenu_item));

	TMENU_LAST(menu->items, last);
	item->text = Q_strdup(Cmd_Argv(2));
	item->alt = true;

	if (last)
		last->next = item;
	else
		menu->items = item;
}

void TMenu_Dump_f (void)
{
	tmenu_header *menu;
	tmenu_item *item;

	if (Cmd_Argc() < 2)
	{
		Com_Printf("usage: %s <name>\n", Cmd_Argv(0));
		return;
	}

	if (!(menu = tmenu_find_by_name(Cmd_Argv(1))))
	{
		Com_Printf("No such menu.\n");
		return;
	}

	Com_Printf("Text menu \"%s\":\n", menu->name);
	TMENU_FOREACH(menu->items, item)
	{
		if (item->command && item->text)
			Com_Printf("  tmenupushcmd \"%s\" \"%s\" \"%s\"\n", menu->name, item->command, item->text);
		if (item->command)
			Com_Printf("  tmenupushcmd \"%s\" \"%s\"\n", menu->name, item->command);
		else if (item->text && item->alt)
			Com_Printf("  tmenupushtextalt \"%s\" \"%s\"\n", menu->name, item->text);
		else if (item->text)
			Com_Printf("  tmenupushtext \"%s\" \"%s\"\n", menu->name, item->text);
		else
			Com_Printf("  tmenupushblank \"%s\"\n", menu->name);
	}
}

void TMenu_Open_f (void)
{
	tmenu_header *menu;

	if (Cmd_Argc() < 2)
	{
		Com_Printf("usage: %s <name>\n", Cmd_Argv(0));
		return;
	}

	if (!(menu = tmenu_find_by_name(Cmd_Argv(1))))
	{
		Com_Printf("No such menu.\n");
		return;
	}

	tmenu_current = menu;
}

void TMenu_Toggle_f (void)
{
	tmenu_header *menu;

	if (Cmd_Argc() < 2)
	{
		Com_Printf("usage: %s <name>\n", Cmd_Argv(0));
		return;
	}

	if (!(menu = tmenu_find_by_name(Cmd_Argv(1))))
	{
		Com_Printf("No such menu.\n");
		return;
	}

	if (tmenu_current == menu)
		tmenu_current = NULL;
	else
		tmenu_current = menu;
}

void TMenu_Close_f (void)
{
	tmenu_current = NULL;
}

void TMenu_Select_f (void)
{
	tmenu_header *menu = tmenu_current;
	tmenu_item *item;
	int i = 0;
	int selection;

	if (Cmd_Argc() < 2)
	{
		Com_Printf("usage: %s <selection>\n", Cmd_Argv(0));
		return;
	}

	if (menu == NULL)
		return;

	tmenu_current = NULL;
	selection = Q_atoi(Cmd_Argv(1));
	if (selection == 0)
		selection = 10;

	TMENU_FOREACH(menu->items, item)
	{
		if (item->command && ++i == selection)
		{
			Cbuf_AddText(item->command);
			Cbuf_AddText("\n");
			break;
		}
	}
}

qbool TMenu_IsOpen(void)
{
	return !(tmenu_current == NULL);
}

void TMenu_Draw (void)
{
	extern cvar_t scr_centershift;
        int charsize = (int) (8.f * vid.height / vid.conheight);
        int i = 0, lines = 0, draw_y, max_len = 0;
	tmenu_item *item;
	char buf[256];

	if (tmenu_current == NULL)
		return;

	TMENU_FOREACH(tmenu_current->items, item)
	{
		if (item->command && item->text && strlen(item->text) > max_len)
			max_len = strlen(item->text);
		lines++;
	}

	draw_y = ((lines <= 4) ? vid.height * 0.35 : 48) + scr_centershift.value * 8;

	TMENU_FOREACH(tmenu_current->items, item)
	{
		if (item->command && item->text)
		{
			i++;
			snprintf(buf, sizeof(buf), "%c%c%c %s", 0x90, (i == 10 ? 0 : i) + 0x92, 0x91, item->text);
			Draw_String(vid.width / 2 - (max_len * 4), draw_y, buf);
			draw_y += charsize;
		}
		else if (item->command)
		{
			i++;
		}
		else if (item->text)
		{
			if (item->alt)
				Draw_Alt_String(vid.width / 2 - strlen(item->text) * 4, draw_y, item->text);
			else
				Draw_String(vid.width / 2 - strlen(item->text) * 4, draw_y, item->text);
			draw_y += charsize;
		}
		else
		{
			draw_y += charsize;
		}
	}
}

void TMenu_Init (void)
{
	Cmd_AddCommand ("tmenuadd", TMenu_Add_f);
	Cmd_AddCommand ("tmenupushcmd", TMenu_PushCmd_f);
	Cmd_AddCommand ("tmenupushtext", TMenu_PushText_f);
	Cmd_AddCommand ("tmenupushtextalt", TMenu_PushTextAlt_f);
	Cmd_AddCommand ("tmenupushblank", TMenu_PushBlank_f);
	Cmd_AddCommand ("tmenudump", TMenu_Dump_f);
	Cmd_AddCommand ("tmenulist", TMenu_List_f);
	Cmd_AddCommand ("tmenudel", TMenu_Del_f);
	Cmd_AddCommand ("tmenuopen", TMenu_Open_f);
	Cmd_AddCommand ("tmenutoggle", TMenu_Toggle_f);
	Cmd_AddCommand ("tmenuclose", TMenu_Close_f);
	Cmd_AddCommand ("tmenuselect", TMenu_Select_f);
}
