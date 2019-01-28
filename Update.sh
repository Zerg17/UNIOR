#! /bin/bash
name=$1
reset
User=$(whoami)
if [[  $User =~ 'root' ]]
then
    echo " "
    echo "*******************************************************************"
    echo -e "Выполните команду не из под \033[1;5;31m[root]\033[0m пользователя"
    echo "*******************************************************************"
    echo " "
    exit
fi

cd /home/$User

backDir="/home/$User/backup"
tempDir="$backDir/temp"
gitDriver="/UNIOR/Unior_driver"
gitEmote="/UNIOR/emote"


if [ -z $1 ]
then
    name=catkin_ws
fi

if [ -d /home/$User/$name/src/unior-driver ]
then
    echo "*******************************************************************"
    echo "Удаляю старый драйвер Unior-2, и устанавливаю новый"
    dirDriver="/home/$User/$name/src/unior-driver"
    echo -e "\033[1;5;31m[$dirDriver/]\033[0m"
    echo "*******************************************************************"
    if [ -d $tempDir ]
    then
        rm -rf $tempDir
    fi
    mkdir $tempDir
    mkdir -p $backDir
    sleep 3
    cp -f --remove-destination ./Update.sh $backDir
    cp -rf --remove-destination $dirDriver $backDir
    cp -rf --remove-destination /home/$User/emote $backDir
    rm -rf $dirDriver/src/*
    rm -rf /home/$User/emote/*
else
    echo "*******************************************************************"
    echo "Возможно у вас RosWorkSpase называется по другому"
    echo -e "Введите имя RosWorkSpase в виде \033[1;5;31m[./update 'Имя каталога']\033[0m"
    echo "*******************************************************************"
    exit
fi 

cd $tempDir
git clone https://github.com/Zerg17/UNIOR.git
echo "*******************************************************************"

cp $tempDir$gitDriver/* $dirDriver/src/
cp $tempDir$gitEmote/* /home/$User/emote/

cd $dirDriver/../..
catkin_make

rm -r -f $tempDir
echo "*******************************************************************"
echo "Удаляю временные файлы"
echo "*******************************************************************"
