//Compile with
//g++ appind-test1.cpp `pkg-config --cflags --libs appindicator-0.1`

#include <libappindicator/app-indicator.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>


AppIndicator *indicator;
GtkWidget *window;
	GtkWidget *indicator_menu;
	GtkWidget *ramfs_item;
	GtkWidget *freemem_item;
	GtkWidget *quit_item;


gboolean read_statistics(gpointer data);
long get_free_RAM(int);
void set_indicating_menu_items(long,long);


///////////////////////////
///////////////////MAIN
//////////////////////////

int main (int argc, char **argv)
{
		
	gtk_init (&argc, &argv);
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL); 

	//connect destroy signal on quit
	g_signal_connect (G_OBJECT (window),
	                  "destroy",
	                  G_CALLBACK (gtk_main_quit),
	                  NULL);

	//Indicator creation
	indicator = app_indicator_new ("ramfs-applet",
	                               "",
	                               APP_INDICATOR_CATEGORY_SYSTEM_SERVICES);

	app_indicator_set_status (indicator, APP_INDICATOR_STATUS_ACTIVE);
	
	set_indicating_menu_items(0,0);
	
	g_timeout_add_seconds(1,read_statistics,NULL);

	gtk_main ();

	return 0;
}



gboolean read_statistics(gpointer data)
{
	//all operations on reading system statistics ang setting them to indicator label go here

	long free_RAM = get_free_RAM(0)/1024;

	gchar label[100];
	sprintf(label,"Free RAM:%ld MB", free_RAM);
	app_indicator_set_label(indicator,label,"");

	return true;
}

void set_indicating_menu_items(long ramfs, long mem)
{

	char ramfs_text[100]; 
	sprintf(ramfs_text,"ramfs size: %ld MB",ramfs/1024);

	char mem_text[100]; 
	sprintf(mem_text,"free RAM: %ld MB", mem/1024);	

	indicator_menu = gtk_menu_new();
	ramfs_item = gtk_menu_item_new_with_label ((const char*)ramfs_text);
	freemem_item = gtk_menu_item_new_with_label ((const char*)mem_text);
	quit_item = gtk_menu_item_new_with_label ("Quit");

	//add menu item to menu
	gtk_menu_append(GTK_MENU(indicator_menu),ramfs_item);
	gtk_menu_append(GTK_MENU(indicator_menu),freemem_item);
	gtk_menu_append(GTK_MENU(indicator_menu),quit_item);	

	g_signal_connect (G_OBJECT (quit_item),
	                  "activate",
	                  G_CALLBACK (gtk_main_quit),
	                  NULL);

	gtk_widget_show (freemem_item);
	gtk_widget_show (ramfs_item);	
	gtk_widget_show (quit_item);
	

	app_indicator_set_menu (indicator, GTK_MENU (indicator_menu));
}


long get_free_RAM(int type)
{

	FILE *fp;
	fp = popen("du -s /mnt/ramtemp", "r");

	if (fp == NULL) 
	{
		printf("Failed to run command\n" );
	}

	int result;char path[1035];
	while (true) 
	{   
		if(fgets(path, sizeof(path)-1, fp) == NULL)break;      
	}

	pclose(fp);

	std::string pathS = path;
	int cut_begin = pathS.find("\t");
	pathS.resize(cut_begin);

	long ramfs_size = atoi(pathS.c_str());

	long freeRAM;


	fp = popen("free", "r");

	if (fp == NULL) 
	{
		printf("Failed to run command\n" );
	}


	for(int i=0;i<3;i++)
	{   
		if(fgets(path, sizeof(path)-1, fp) == NULL)break;
		pathS = path;

		if(i==2)
		{
			cut_begin = pathS.find_last_of(" ") + 1;
			pathS = pathS.substr(cut_begin,strlen(pathS.c_str()) - cut_begin - 1);
			freeRAM = atoi(pathS.c_str());
		}

	}

	pclose(fp);

	set_indicating_menu_items(ramfs_size,freeRAM);

	result = freeRAM - ramfs_size;

	switch(type)
	{
		case 0:
		{
			return result;
		}

		case 1:
		{
			return freeRAM;
		}

		case 2:
		{
			return ramfs_size;
		}
	}
}