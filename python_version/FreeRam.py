RAMFS_LOCATION_PATH = "/mnt/ramtemp"

import subprocess

def get_ramfs_size():
    """gets the ramfs size"""

    ramfs_size_popen = subprocess.Popen(["du", "-s", RAMFS_LOCATION_PATH],stdout=subprocess.PIPE)

    ramfs_size = ramfs_size_popen.communicate()[0].decode('ASCII')

    ramfs_size = int(ramfs_size[0:(ramfs_size.find('\t')-0)])

    return ramfs_size

def get_free_RAM():
    """get the amount of free RAM"""

    free_RAM_popen = subprocess.Popen(["free"],stdout=subprocess.PIPE)

    free_RAM = free_RAM_popen.communicate()[0]
    free_RAM = free_RAM.decode('ASCII')

    free_RAM = free_RAM[(free_RAM.find('\n')+1):]
    free_RAM = free_RAM[(free_RAM.find('\n')+1):]
    free_RAM = free_RAM[:(free_RAM.find('\n')+0)]
    free_RAM = int(free_RAM[(free_RAM.rfind(' ')+1):])

    return free_RAM

#MAIN

if __name__ == "__main__":

    #total RAM including ramdisk
    total_RAM = get_free_RAM() - get_ramfs_size()


    print("Ramfs size: " + str(get_ramfs_size()))
    print("Free RAM: " + str(get_free_RAM()))
    print("Total RAM: " + str(total_RAM) + "\t" + str(total_RAM//1024) + "Mb")