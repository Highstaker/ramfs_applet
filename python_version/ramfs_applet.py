import FreeRam


from gi.repository import Gtk, GLib
from gi.repository import AppIndicator3 as appindicator

class menuRefresher:

    
    def set_indicating_menus(self,ind):
        """Sets the values in menus on each reresh"""
        free_RAM = FreeRam.get_free_RAM()/1024
        ramfs_size = FreeRam.get_ramfs_size()/1024

        #total RAM including ramdisk
        total_RAM = int(round(free_RAM - ramfs_size))

        #print("Total RAM: " + str(total_RAM) + "Mb")

        # create a menu
        menu = Gtk.Menu()

     
        menu_items = Gtk.MenuItem("Total free RAM: %d Mb" % round(free_RAM))
        menu.append(menu_items)        
        menu_items.show()

        menu_items = Gtk.MenuItem("Ramfs size: %d Mb" % round(ramfs_size))
        menu.append(menu_items)        
        menu_items.show()

        menu_items = Gtk.MenuItem("Exit") 
        menu_items.connect("activate",Gtk.main_quit)
        menu.append(menu_items)        
        menu_items.show()


        ind.set_menu(menu)

        ind.set_label("Free RAM: %d Mb" % total_RAM,"")

        return True
     
    def __init__(self,ind):
        GLib.timeout_add_seconds(1,self.set_indicating_menus,ind)


if __name__ == "__main__":

    

    #Creating the application indicator

    ind = appindicator.Indicator.new (
                        "ramfs_applet",
                        "",
                        appindicator.IndicatorCategory.SYSTEM_SERVICES)
    ind.set_status (appindicator.IndicatorStatus.ACTIVE)
    ind.set_attention_icon ("indicator-messages-new")

    menuRefresher(ind)

    #set_indicating_menus(ind)
 
    #GLib.timeout_add_seconds(1,set_indicating_menus(ind))

    Gtk.main()