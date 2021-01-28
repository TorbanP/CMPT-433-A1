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

            Workstation Complete

    A.3 - Configure a kernel for each of your hardware configurations.
        TODO
    A.4 - Write a C module that can be inserted as a kernel module that prints "Hello, world" when the module is initialized and installed, and "Goodbye, world" when the module is removed.
        TODO
    A.5 - Write a C program that reads the /proc filesystem to get status information about the machine. Justify what information you are obtaining and why.
        TODO

Part B
    B.1 - Obtain a version of the Apache web server and ensure that PHP capabilities are provided in your system.
        TODO
    B.2 - Write a simple script in PHP that accesses the /proc filesystem on the web server machine and prints out the same information that the C program did in Part A.5. This must be able to be run under Apache
        TODO
    B.3 - Write a simple script in python (or other scripting language) that accesses the /proc filesystem on the web server machine and prints out the same information that the C program did in Part A.5.
        TODO
    B.4 - Show that you can access the script in Part B.2. from a browser on the VM, or from another computer (real or virtual).
        TODO