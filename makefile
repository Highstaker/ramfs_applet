all:
	g++ ramfs_applet.cpp -o /mnt/ramtemp/ramfs-applet `pkg-config --cflags --libs appindicator-0.1`