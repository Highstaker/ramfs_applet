//Compile with
//g++ appind-test1.cpp `pkg-config --cflags --libs appindicator-0.1`

#include <libappindicator/app-indicator.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace std;

AppIndicator *indicator;

static void activate_action (GtkAction *action);
gboolean read_statistics(gpointer data);
long get_free_RAM();

static GtkActionEntry entries[] = {
 { "FileMenu", NULL, "_File" },  
 { "Quit",     "application-exit", "_Quit", "<control>Q",
 "Exit the application", G_CALLBACK (gtk_main_quit) },
};
static guint n_entries = G_N_ELEMENTS (entries);

static const gchar *ui_info =
"<ui>"
"  <menubar name='MenuBar'>"
"    <menu action='FileMenu'>"
"      <separator/>"
"      <menuitem action='Quit'/>"
"    </menu>"
"  </menubar>"
"  <popup name='IndicatorPopup'>"
"    <menuitem action='Quit' />"
"  </popup>"
"</ui>";

static void
activate_action (GtkAction *action)
{
  const gchar *name = gtk_action_get_name (action);
  GtkWidget *dialog;

  dialog = gtk_message_dialog_new (NULL,
   GTK_DIALOG_DESTROY_WITH_PARENT,
   GTK_MESSAGE_INFO,
   GTK_BUTTONS_CLOSE,
   "You activated action: \"%s\"",
   name);

  g_signal_connect (dialog, "response",
    G_CALLBACK (gtk_widget_destroy), NULL);

  gtk_widget_show (dialog);
}

int main (int argc, char **argv)
{
  GtkWidget *window;
  GtkWidget *menubar;
  GtkWidget *table;
  GtkWidget *sw;
  GtkWidget *contents;
  GtkWidget *statusbar;
  GtkWidget *indicator_menu;
  GtkActionGroup *action_group;
  GtkUIManager *uim;
  
  GError *error = NULL;

  gtk_init (&argc, &argv);

  // /* main window */
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL); 
  g_signal_connect (G_OBJECT (window),
    "destroy",
    G_CALLBACK (gtk_main_quit),
    NULL);
  

  /* Menus */
  action_group = gtk_action_group_new ("AppActions");
  gtk_action_group_add_actions (action_group,
    entries, n_entries,
    window);

  uim = gtk_ui_manager_new ();
  g_object_set_data_full (G_OBJECT (window),
    "ui-manager", uim,
    g_object_unref);
  gtk_ui_manager_insert_action_group (uim, action_group, 0);
  gtk_window_add_accel_group (GTK_WINDOW (window),
    gtk_ui_manager_get_accel_group (uim));

  if (!gtk_ui_manager_add_ui_from_string (uim, ui_info, -1, &error))
  {
    g_message ("Failed to build menus: %s\n", error->message);
    g_error_free (error);
    error = NULL;
  }


  /* Indicator */

  indicator = app_indicator_new ("example-simple-client",
   "indicator-system",
   APP_INDICATOR_CATEGORY_SYSTEM_SERVICES);



  indicator_menu = gtk_ui_manager_get_widget (uim, "/ui/IndicatorPopup");

  app_indicator_set_status (indicator, APP_INDICATOR_STATUS_ACTIVE);

  app_indicator_set_menu (indicator, GTK_MENU (indicator_menu));

  app_indicator_set_label(indicator,"label-test","");

  g_timeout_add_seconds(1,read_statistics,NULL);

  gtk_main ();

  return 0;
}



gboolean read_statistics(gpointer data)
{
  //all operations on reading system statistics ang setting them to indicator label go here

long free_RAM = get_free_RAM()/1024;

//std::cerr << free_RAM << " " << rand() << std::endl;//debug

gchar label[100];
sprintf(label,"Free RAM:%ld MB", free_RAM);
app_indicator_set_label(indicator,label,"");

return true;
}


long get_free_RAM()
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
    //printf("%s", path);   
  }

  pclose(fp);
  
  string pathS = path;
  int cut_begin = pathS.find("\t");
  pathS.resize(cut_begin);

  long ramfs_size = atoi(pathS.c_str());

  //cout << " path 1 "<< pathS << "  cut_begin " << cut_begin << endl;//debug
  //cout << "ramfs_size " << ramfs_size << endl;//debug


  long totalRAM, freeRAM;


  fp = popen("free", "r");
    
  if (fp == NULL) 
    {
      printf("Failed to run command\n" );
  }

  //int result;char path[1035];
  for(int i=0;i<3;i++)
  {   
    if(fgets(path, sizeof(path)-1, fp) == NULL)break;
pathS = path;
    if(i==1)
    {
      cut_begin = pathS.find_first_of("123456789");
      
      pathS = pathS.substr(cut_begin,strlen(pathS.c_str()));

      cut_begin = pathS.find_first_of(" ");
      pathS.resize(cut_begin);
      totalRAM = atoi(pathS.c_str());

  //cout << "totalRAM " << totalRAM << endl;//debug
    }

    if(i==2)
    {
      
      cut_begin = pathS.find_last_of(" ") + 1;
      pathS = pathS.substr(cut_begin,strlen(pathS.c_str()) - cut_begin - 1);
    freeRAM = atoi(pathS.c_str());

  //cout << "pathS " << pathS << "  cut_begin " << cut_begin  << " length " << strlen(pathS.c_str()) - cut_begin - 1 << endl;//debug
  //cout << "pathS " << pathS << endl;//debug
  //cout << "freeRAM " << freeRAM << " ramfs_size " << ramfs_size << endl;//debug
    }

  }

  pclose(fp);

  
result = freeRAM - ramfs_size;


  return result;
}