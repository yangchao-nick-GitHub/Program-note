#!/bin/bash

E_OK=0
E_ERROR=3


function func1
{
    if [[ $1 == 10 ]];then 
        echo "10"
        return $E_OK
    else 
        echo "other"
        return $E_ERROR
    fi
    echo "finished"
}

function func2
{
    func1 10

    if [[ $? == $E_OK ]];then 
        echo "return E_OK"
    elif [[ $? == $E_ERROR ]];then
        echo "return E_ERROR"
    fi

    echo "finished"
}

func2 

