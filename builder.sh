#!/bin/sh

#Black        0;30     Dark Gray     1;30
#Red          0;31     Light Red     1;31
#Green        0;32     Light Green   1;32
#Brown/Orange 0;33     Yellow        1;33
#Blue         0;34     Light Blue    1;34
#Purple       0;35     Light Purple  1;35
#Cyan         0;36     Light Cyan    1;36
#Light Gray   0;37     White         1;37

RED='\033[0;31m'
GREEN='\033[0;32m'
ORANGE='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

############################################################
# Help                                                     #
############################################################

help()
{
        # Display Help
        echo "This is the help option for builder.sh. "
        echo
        echo "Syntax: build.sh [-b|i|a]"
        echo "options:"
        echo "b      build plugin using mod plugin builder in a docker container"
        echo "i      install plugin from mpd-docker-mount dir to moddwarf"
        echo "a      run build and install function"
}

############################################################
# Build                                                    #
############################################################

build()
{
    # copy plugin dir to docker mount
    cp -r ../patternCV/ /c/Users/steve/OneDrive/Development/mpd-docker-mount/plugins/sve-plugins
    # start docker container
    docker start -i moddwarf-new
    # in docker copy from mount to plugin builder / plugins/package dir
    cp -r ../../mpd-docker-mount/plugins/sve-plugins/patternCV/source/ plugins/package/pattern/
    # set source to moddwarf-new and go into plugin/package dir
    source local.env moddwarf-new
    cd plugins/package/patternCV
    # run cmake and make
    cmake . && make
    # copy lv2 dir to docker mount
    cp -r bin/sve-patternCV.lv2/ ../../../../../mpd-docker-mount/mdw/
    # exit docker environment
    exit

    echo "${GREEN}build function is completed!${NC}"
}

############################################################
# Install                                                  #
############################################################

install()
{
    # install plugin on moddwarf
    cd /c/Users/steve/OneDrive/Development/mpd-docker-mount/mdw
    # make tar.gz and
    tar czf sve-patternCV.lv2.tar.gz sve-patternCV.lv2
    cat *.tar.gz | base64 | curl -F 'package=@-' http://192.168.51.1/sdk/install && echo
    cd -

    echo "${GREEN}install function is completed!${NC}"
}

############################################################
# All                                                      #
############################################################

all()
{
    build
    install
    echo "${GREEN}all function is completed!${NC}"
}

############################################################
# handling options                                         #
############################################################

while getopts ":abhi" option; do
    case $option in
        a)  # run all function
            all
            exit;;
        b)  # run build function
            build
            exit;;
        h) # display Help
            help
            exit;;
        i)  # run install function
            install
            exit;;
       \?) # Invalid option
           echo "${RED}Error: Invalid option${NC}"
           exit;;
    esac
done
