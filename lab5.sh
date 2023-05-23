#!/bin/bash
if [ $# -ne 1 ]
then
echo "Usage: usr_monitor  username"
exit 0
fi
input_user_name=$1
user_names=`who | awk '{print $1}'`
echo "Existing user(s) is(are) $user_names"
if echo "$user_names" | grep -qw "$input_user_name";then
  # 用户已登录
  echo "User $input_user_name is logged on"
  exit 0
else
  # 等待用户登录
  echo "Waiting for user $input_user_name ..."
fi
while [ 1 ]
do
    echo "Waiting for user $input_user_name ..."
    sleep 5
    user_names=`who | awk '{print $1}'`
    if echo "$user_names" | grep -qw "$input_user_name";
    then
    echo "$input_user_name is log on"
    exit 0
    fi
done

