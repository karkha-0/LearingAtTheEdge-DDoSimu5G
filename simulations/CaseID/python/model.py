#!/usr/bin/env python3
# For running ML prediction App 

import json
import pyshark  

def main():
    ip_list = ["192.168.9.1", "192.168.9.2", "192.168.9.3", "192.168.9.4"]
    print(json.dumps(ip_list))  # Convert list to JSON string

if __name__ == "__main__":
    main()
   # parse_pcap(path="../results/THESIS/PCAP", filename="localRouter.pcap")
    
