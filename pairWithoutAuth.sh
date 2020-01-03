#!/bin/bash
{
printf 'power on\n'
sleep 2
printf 'discoverable on\n'
sleep 2
printf 'quit\n'
}|bluetoothctl

pulseaudio --start
bt-agent -c NoInputNoOutput -p /root/Desktop/config.cfg
