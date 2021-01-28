CMPT 433 A1

Assignment 1

Part A
    A.1 - Obtain a Linux distribution of your choice that you can install on a VM. 
        From a selection of tested Distributions:
            Debian 10.7.0
            Ubuntu 20.04.1
            Linuxmint 20.1
            CentOS 8.3.2011
            A few random ones from distrowatch.com selected by asking coworkers random numbers between 1-100
                Stumbled on http://www.linuxfromscratch.org/, decided I should carefully back away for now.
        
        Due to time constraints, focused on what was familiar - Debian based
            Ruling out CentOS
                Im really used to apt, not a lot of fun
            Just like university, and work, the fundamental constraint is always time.

        Due to time constraints using Ubuntu 20.04.1
            Raw debian wasnt particularly exciting, a lot older kernel than ubuntu and mint
            Available in both a Server and Desktop distribution
            Available kernel source
            Extreemly common preconfigured VPS client on Digitalocean which i use for my other servers
            Traditionally I use Linux Mint for some machines. Stuck to using windows normally due to specific work software (CAD)
            Ubuntu desktop using same kernel as server (from what i can tell)


    A.2 - Install and configure VBox to be able to load that image to run.
        Installing from command line on a fresh Linux Mint base OS accessed using Teamviewer to work location.
            TP-FATBOI - 4U 20 bay server rack with old scrap parts
            6 core Intel 3930k @ 3.2GHz with a bent up Noctua cooler to fit inside
            16GB DDR3 Ram
            500GB SATA ssd + 2x8TB + 2x3TB + 1x1.5TB Mechaincal drives (future offsite backup for Synology NAS)

        Setting up Virtualbox: (package manager version was old so dl from oracle, removed old, and got latest version)
            crudely follow for ideas (was out of date) https://www.cloudsavvyit.com/3118/how-to-create-virtualbox-vms-from-the-linux-terminal/
            $ wget https://download.virtualbox.org/virtualbox/6.1.18/virtualbox-6.1_6.1.18-142142~Ubuntu~eoan_amd64.deb
            $ sudo VBoxManage extpack install Oracle_VM_VirtualBox_Extension_Pack-5.2.42.extpack        # install debian package
            $ sudo apt install htop               # monitoring base system performance
            $ VBoxManage -v                       # Checking version installed
                6.1.18r142142                     # https://www.virtualbox.org/wiki/Linux_Downloads = Location for finding current extension pack
            
            Note - Why do guides always blindly link in wget statements to versions that are out of date almost as soon as the guide is made?

            $ wget https://download.virtualbox.org/virtualbox/6.1.18/Oracle_VM_VirtualBox_Extension_Pack-6.1.18.vbox-extpack
            $ sudo VBoxManage extpack install Oracle_VM_VirtualBox_Extension_Pack-6.1.18.vbox-extpack # install extension pack
                accept eula
            
            Setup Base Config for Server
            $ sudo VBoxManage createvm --name "Ubuntu Server" --ostype Ubuntu_64 --register # set up server VM
            $ sudo VBoxManage modifyvm "Ubuntu_server" --memory 8192 # 8 GB ram (edited: increased to 8GB for compiling kernel and not hating life)
            $ sudo VBoxManage createhd --filename "Ubuntu_server.vdi" --size 32000 # 32GB HDD on SSD
            $ sudo VBoxManage modifyvm "Ubuntu_server" --cpus 6 # 6 CPU threads

            Setup HDD
            $ sudo VBoxManage storagectl "Ubuntu_server" --name "SAS Controller" --add sas --controller LSILogicSAS
            $ sudo VBoxManage storageattach "Ubuntu_server" --storagectl "SAS Controller" --port 0 --device 0 --type hdd --medium "Ubuntu_server.vdi"
            $ sudo VBoxManage storagectl "Ubuntu_server" --name "SAS Controller" --hostiocache on

            Setup install iso directory
            $ sudo VBoxManage storagectl "Ubuntu_server" --name "IDE Controller" --add ide --controller PIIX4
            $ sudo VBoxManage storageattach "Ubuntu_server" --storagectl "IDE Controller" --port 0 --device 0 --type dvddrive --medium "/home/torban/Desktop/ubuntu-20.04.1-live-server-amd64.iso"

            Ram tweaks?
            $ sudo VBoxManage modifyvm "Ubuntu_server" --nestedpaging on
            $ sudo VBoxManage modifyvm "Ubuntu_server" --largepages on

            Setup remote access
            $ sudo VBoxManage modifyvm "Ubuntu_server" --vrde on
            $ sudo VBoxManage modifyvm "Ubuntu_server" --vrdeport 5587

            Setup a NAT Network
            $ sudo VBoxManage natnetwork add --netname natnet1 --network "10.0.0.0/24" --enable
            $ sudo VBoxManage natnetwork start --netname natnet1

            Start it up!
            $ sudo VBoxManage startvm "Ubuntu_server" --type headless

            Using RDP client
            $ sudo apt install remmina
            Use Remmina to RDP to localhost:5587
            RDP sorta works, walk through installation with half the screen missing, setup username, password, machine name, etc
            Edited settings in Remmina to force a higher resolution after installer failed so i could complete the installer
            Fought fora  few hours trying to get the server to reboot after install.

            Log in for first time & update
            $ sudo apt update
            $ sudo apt upgrade

            Enable SSH
            $ sudo ufw allow ssh

            Server Complete

            Setup Base Config for Workstation ( Identical to server except 4gb ram & called Ubuntu_workstation)
            $ sudo VBoxManage createvm --name "Ubuntu_workstation" --ostype Ubuntu_64 --register # set up server VM
            $ sudo VBoxManage modifyvm "Ubuntu_workstation" --memory 8192 # 8 GB ram (edited: increased to 8GB for compiling kernel and not hating life)
            $ sudo VBoxManage createhd --filename "Ubuntu_workstation.vdi" --size 32000 # 32GB HDD on SSD
            $ sudo VBoxManage modifyvm "Ubuntu_workstation" --cpus 6 # 6 CPU threads

            Setup HDD
            $ sudo VBoxManage storagectl "Ubuntu_workstation" --name "SAS Controller" --add sas --controller LSILogicSAS
            $ sudo VBoxManage storageattach "Ubuntu_workstation" --storagectl "SAS Controller" --port 0 --device 0 --type hdd --medium "Ubuntu_workstation.vdi"
            $ sudo VBoxManage storagectl "Ubuntu_workstation" --name "SAS Controller" --hostiocache on

            Setup install iso directory
            $ sudo VBoxManage storagectl "Ubuntu_workstation" --name "IDE Controller" --add ide --controller PIIX4
            $ sudo VBoxManage storageattach "Ubuntu_workstation" --storagectl "IDE Controller" --port 0 --device 0 --type dvddrive --medium "/home/torban/Desktop/ubuntu-20.04.1-desktop-amd64.iso"

            Ram tweaks?
            $ sudo VBoxManage modifyvm "Ubuntu_workstation" --nestedpaging on
            $ sudo VBoxManage modifyvm "Ubuntu_workstation" --largepages on

            Setup remote access
            $ sudo VBoxManage modifyvm "Ubuntu_workstation" --vrde on
            $ sudo VBoxManage modifyvm "Ubuntu_workstation" --vrdeport 5588

            Start it up!
            $ sudo VBoxManage startvm "Ubuntu_workstation" --type headless

            Use Remmina to RDP to localhost:5588
            Desktop install went a lot smoother than the server install! did have to force screen resolution to complete setup though

            Once installed,
            Log in for first time & update
            $ sudo apt update
            $ sudo apt upgrade

            Enable SSH
            $ sudo ufw allow ssh
            Workstation Complete

    A.3 - Configure a kernel for each of your hardware configurations.
        The joy of selecting ubuntu is the wealth of resources on the internet to tweak it
        using https://wiki.ubuntu.com/Kernel/BuildYourOwnKernel as a reference
        using https://linuxguides.co.uk/ubuntu-guides/how-to-compile-kernel-ubuntu-20-04/ as a better reference

        Install dependencies for compiling
        $ sudo apt-get install -y build-essential libncurses5-dev gcc libssl-dev grub2 bc bison flex libelf-dev

        Install the latest kernel 5.9.9 for maximum fun
        $ wget https://cdn.kernel.org/pub/linux/kernel/v5.x/linux-5.9.9.tar.xz
        $ tar -xvf linux-5.9.9.tar.xz
        $ ls -l /boot

        move current kernel config file to working directory
        $ cp /boot/config-5.4.0-65-generic /home/torban/linux-5.9.9/.config

        $ make menuconfig
        I looked through a lot of the kernel options but couldnt really see anything worth tweaking at the risk of breaking things
        Loaded the .config file into menuconfig
        $ make -j12 deb-pkg # 12 cores of goodness
        Press enter blindly about 500 times
            apparently i didnt save the config file properly which is why it asked so many questions
        Heat the room for a while compiling while starting workststion kernel build








        Workstation Kernel, Same process mostly
        Workstation uses a new kernel due to an update so adapt code slightly

        move current kernel config file to working directory
        $ cp /boot/config-5.8.0-40-generic /home/torban/linux-5.9.9/.config

        $ make menuconfig
        Loaded the .config file into menuconfig
        $ make -j12 deb-pkg # 12 cores of goodness
        Press enter blindly about 500 times
        go to bed





    A.4 - Write a C module that can be inserted as a kernel module that prints "Hello, world" when the module is initialized and installed, and "Goodbye, world" when the module is removed.
        TODO
    A.5 - Write a C program that reads the /proc filesystem to get status information about the machine. Justify what information you are obtaining and why.
        
        First install gcc compiler
        $sudo apt install gcc

        Write hello world to confirm everythings working
        $ torban@ubuntu-server:~$ ./helloworld
        $ hello World!
        Success! Code pushed to GIT

        Find some example code to run proc stuffs
        a good start https://stackoverflow.com/questions/56114598/read-proc-cpuinfo-and-print-matching-line-in-c

        Created code (procstats.c) that shows the following useful information
            CPU
            CPU Clock
            CPU Cores
            Total Memory
            Free memory
            Linux version incl. kernel
            Would like HDD space readout but short on time

Part B
    B.1 - Obtain a version of the Apache web server and ensure that PHP capabilities are provided in your system.
        
        Install Apache
        $ sudo apt install apache2
        Configure Firewall
        $ sudo ufw allow "Apache Full"
        $ sudo ufw enable
        Check to see if server is running by accessing webpage
        Working on port 80, not https though

        Install PHP
        using guide https://linuxize.com/post/how-to-install-php-on-ubuntu-20-04/
        $ sudo apt install php libapache2-mod-php # install apache2 php plugin
        $ sudo systemctl restart apache2 # restart

        Verify it works by creating example php page taht displays phpinfo();
        Successfully running PHP Version 7.4.3

    B.2 - Write a simple script in PHP that accesses the /proc filesystem on the web server machine and prints out the same information that the C program did in Part A.5. This must be able to be run under Apache
        TODO
    B.3 - Write a simple script in python (or other scripting language) that accesses the /proc filesystem on the web server machine and prints out the same information that the C program did in Part A.5.
        TODO
    B.4 - Show that you can access the script in Part B.2. from a browser on the VM, or from another computer (real or virtual).
        TODO