## 1. Introduction
The objective of this lab was to perform a Man-in-the-Middle (MITM) attack within a switched Ethernet network. Acting as the attacker "Mallory," the goal was to intercept traffic between two other nodes, "Alice" and "Bob," to steal a secret flag. This was achieved using **ARP Spoofing** (also known as ARP Cache Poisoning).

## 2. Network Reconnaissance
The first step was to identify the targets on the local network (interface `eth1`).
* **Method:** I used `ifconfig` to identify my own details and `ping` sweeps combined with `arp` cache inspection to identify Alice and Bob.
* **Findings:**

| Role | IP Address | MAC Address (Hardware) |
| :--- | :--- | :--- |
| **Mallory (Attacker)** | `10.10.49.4` | `02:42:0a:0a:31:04` |
| **Alice (Victim)** | `10.10.49.2` | `02:42:0a:0a:31:02` |
| **Bob (Target)** | `10.10.49.3` | `02:42:0a:0a:31:03` |

## 3. Attack Strategy: ARP Spoofing
To become the "Man in the Middle," I needed to poison the ARP cache of both victims.
1.  **Poisoning Alice:** I sent a forged ARP Reply telling Alice that **Bob's IP** is at **Mallory's MAC**.
2.  **Poisoning Bob:** I sent a forged ARP Reply telling Bob that **Alice's IP** is at **Mallory's MAC**.

### 3.1 Payload Construction
Using the provided `raw_packet` tool and `hexedit`, I constructed two binary payload files containing the raw hex bytes for the spoofed Ethernet/ARP frames.

**Payload 1: `spoof_alice` (Trick Alice)**
* **Sender MAC:** `02:42:0a:0a:31:04` (Mallory - The Trap)
* **Sender IP:** `10.10.49.3` (Bob - The Lie)
* **Target:** Alice

**Payload 2: `spoof_bob` (Trick Bob)**
* **Sender MAC:** `02:42:0a:0a:31:04` (Mallory - The Trap)
* **Sender IP:** `10.10.49.2` (Alice - The Lie)
* **Target:** Bob

## 4. Execution
Because ARP cache entries expire quickly (often within seconds), a single packet is insufficient. I created a Bash script to continuously send these spoofed packets.

**Script: `mitm.sh`**
```bash
#!/bin/bash
echo "Starting MITM Attack..."
while true; do
    # Tell Alice I am Bob
    sudo ./raw_packet eth1 02:42:0a:0a:31:02 0x0806 spoof_alice
    # Tell Bob I am Alice
    sudo ./raw_packet eth1 02:42:0a:0a:31:03 0x0806 spoof_bob
    sleep 2
done

Here is the complete content for your Writeup.md file in a copy-paste friendly block.

Markdown

# ICS Lab 02: Man-in-the-Middle Attack (ARP Spoofing)

**Student Name:** Younus Saberi
**Lab Instance:** 49
**Date:** November 23, 2025

## 1. Introduction
The objective of this lab was to perform a Man-in-the-Middle (MITM) attack within a switched Ethernet network. Acting as the attacker "Mallory," the goal was to intercept traffic between two other nodes, "Alice" and "Bob," to steal a secret flag. This was achieved using **ARP Spoofing** (also known as ARP Cache Poisoning).

## 2. Network Reconnaissance
The first step was to identify the targets on the local network (interface `eth1`).
* **Method:** I used `ifconfig` to identify my own details and `ping` sweeps combined with `arp` cache inspection to identify Alice and Bob.
* **Findings:**

| Role | IP Address | MAC Address (Hardware) |
| :--- | :--- | :--- |
| **Mallory (Attacker)** | `10.10.49.4` | `02:42:0a:0a:31:04` |
| **Alice (Victim)** | `10.10.49.2` | `02:42:0a:0a:31:02` |
| **Bob (Target)** | `10.10.49.3` | `02:42:0a:0a:31:03` |

## 3. Attack Strategy: ARP Spoofing
To become the "Man in the Middle," I needed to poison the ARP cache of both victims.
1.  **Poisoning Alice:** I sent a forged ARP Reply telling Alice that **Bob's IP** is at **Mallory's MAC**.
2.  **Poisoning Bob:** I sent a forged ARP Reply telling Bob that **Alice's IP** is at **Mallory's MAC**.

### 3.1 Payload Construction
Using the provided `raw_packet` tool and `hexedit`, I constructed two binary payload files containing the raw hex bytes for the spoofed Ethernet/ARP frames.

**Payload 1: `spoof_alice` (Trick Alice)**
* **Sender MAC:** `02:42:0a:0a:31:04` (Mallory - The Trap)
* **Sender IP:** `10.10.49.3` (Bob - The Lie)
* **Target:** Alice

**Payload 2: `spoof_bob` (Trick Bob)**
* **Sender MAC:** `02:42:0a:0a:31:04` (Mallory - The Trap)
* **Sender IP:** `10.10.49.2` (Alice - The Lie)
* **Target:** Bob

## 4. Execution
Because ARP cache entries expire quickly (often within seconds), a single packet is insufficient. I created a Bash script to continuously send these spoofed packets.

**Script: `mitm.sh`**
```bash
#!/bin/bash
echo "Starting MITM Attack..."
while true; do
    # Tell Alice I am Bob
    sudo ./raw_packet eth1 02:42:0a:0a:31:02 0x0806 spoof_alice
    # Tell Bob I am Alice
    sudo ./raw_packet eth1 02:42:0a:0a:31:03 0x0806 spoof_bob
    sleep 2
done
Traffic Interception: With the script running in the background, I used tcpdump to listen to the traffic passing through my interface.

Bash

sudo tcpdump -i eth1 -A | grep "CTF"
5. Results
The attack was successful. I intercepted the communication from Alice intended for Bob and captured the secret flag.

Captured Secret:

CTF{secret-fSkqpILT51TM3B6OQZ9x}

6. Technical Challenges & Observations
During the attack, I attempted to enable IP forwarding to allow traffic to flow transparently between Alice and Bob:

Bash

sudo sysctl -w net.ipv4.ip_forward=1
# Output: "read-only file system" / "permission denied"
Observation: Due to container/permissions restrictions in the lab environment, I was unable to enable Kernel IP Forwarding.

Impact: Because forwarding was disabled, my machine acted as a traffic "black hole." Packets from Alice arrived at my interface (where tcpdump successfully captured them) but were subsequently dropped by the kernel instead of being forwarded to Bob. While this resulted in a Denial of Service (DoS) for the victims, it was sufficient to satisfy the primary objective of intercepting the secret data.
