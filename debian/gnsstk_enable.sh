# This file should be sourced and not executed as bash script.  Ex. source /etc/profile.d/gnsstk_enable.sh
GNSSTK_VER=__VER__

# User created file that overwrites GNSSTK_VER variable.
# Use kv pair GNSSTK_VER=<major_version> in file /etc/toolkits.conf to overwrite.
# By default, /etc/toolkits.conf file doesn't exist so this gets ignored. 
FILE=/etc/toolkits.conf

if [ -f "$FILE" ]; then
    echo "found $FILE so sourcing it"
    set -o allexport && . $FILE && set +o allexport
fi

echo "gnsstk major version being used is $GNSSTK_VER"
. /usr/share/gnsstk__VER__/enable

echo -e "\nThe following env variables have been set or are already set"
env | grep 'LD_LIBRARY_PATH\|CPLUS_INCLUDE_PATH\|PYTHONPATH\|^PATH'
