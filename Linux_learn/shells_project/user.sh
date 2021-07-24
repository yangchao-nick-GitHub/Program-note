#!/bin/bash

MAX_NAME_LEN=10

function Usage
{
cat <<EOF
    user.sh -a -u username -p passwd [-i homedir] [-s shell]        # add a user
    user.sh -a -S username -p passwd [-i homedir] [-s shell]        # add a super user
    user.sh -c -u username [-p passwd] [-i homedir] [-s shell]      # change the user setting
    user.sh -d -u username                                          # delete a user     
    user.sh -R                                                      # recover the root  passwd                                     
EOF
    exit 0
}



function add_user
{
    if [[ `echo $username | wc -L` -gt $MAX_NAME_LEN ]];then
        echo "[Error]: username length is too long"
        exit 1
    fi

    grep "$username" /etc/passwd >/dev/null 2>&1
    if [ $? -eq 0 ];then   
        echo "[Error]:$username is exist in /etc/passwd"
        exit 1
    fi

    if [[ -z $password ]];then
        echo "[Error]: password is required"
        exit 1
    fi

    ADD_CMD="useradd $username"

    if [[ ! -z $homedir ]];then
        mkdir -p $homedir
        ADD_CMD="$ADD_CMD -d $homedir"
    fi

    if [[ ! -z $shell ]];then
        ADD_CMD="$ADD_CMD -s $shell"
    fi

    $ADD_CMD 

    #passwd $username
    echo $password$ | passwd -stdin $username
     

    echo "add $username OK"
    exit 0

}





# check cmd arg
SHORTOPTS="acdhRu:S:p:i:s:"
ARGS=`getopt -o $SHORTOPTS -n "$0" -- "$@"`
[ $? -ne 0 ] && Usage
eval set -- "${ARGS}"

while :
do
    case $1 in
        -h)
            Usage
            shift;;
        -a)
            action=a
            shift;;
        -S)
            action=S
            shift;;
        -c)
            action=c
            shift;;
        -d)
            action=d
            shift;;
        -R)
            action=R
            shift;;
        -u)
            username=$2
            shift 2;;
        -p)
            password=$2
            shift 2;;
        -i)
            home_dir=$2
            shift 2;;
        -s)
            shell=$2
            shift 2;;
        --)
            break;;
    esac
done

if [[ -z $username ]];then
    echo "[Error]: username is required"
fi

case $action in 
    a)
        add_user
        ;;
    S)
        add_super_user
        ;;
    c)
        change_user_settings
        ;;
    d)
        delete_user
        ;;
esac






        


