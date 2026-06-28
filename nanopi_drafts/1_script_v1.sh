#!/bin/bash
set -euo pipefail

LAN_IP="192.168.2.1"
LAN_IP_WITH_MASK="192.168.2.1/24"
DHCP_START="101"
DHCP_SIZE="154"
WIFI_SSID_2G="NanoPi_2G"
WIFI_SSID_5G="NanoPi_5G"
WIFI_COUNTRY_CODE_5G="RU"

DEFAULT_WIFI_PASS="qwerty123"

WIFI_PASS="${WIFI_PASS:-$DEFAULT_WIFI_PASS}"
WIFI_PASS_2G="${WIFI_PASS_2G:-$WIFI_PASS}"
WIFI_PASS_5G="${WIFI_PASS_5G:-$WIFI_PASS}"
if [ "$WIFI_PASS_2G" = "qwerty123" ] || [ "$WIFI_PASS_5G" = "qwerty123" ]; then
  echo "ERROR: detected default Wi-Fi password! Please replace qwerty123 with strong 8-63 symbols." >&2
  echo "Change DEFAULT_WIFI_PASS in the script variable or pass WIFI_PASS via env." >&2
  echo "Pro tip: you may use different WIFI_PASS_2G and WIFI_PASS_5G passwords." >&2
  exit 1
fi

if [ "$EUID" -ne 0 ]; then
  echo "ERROR: run me as root!" >&2
  exit 1
fi

echo "Checking required packages..."
REQUIRED_PKGS=("nftables" "hostapd")
MISSING_PKGS=()
for pkg in "${REQUIRED_PKGS[@]}"; do
  if ! dpkg-query -W -f='${Status}' "$pkg" 2>/dev/null | grep -q "ok installed"; then
    MISSING_PKGS+=("$pkg")
  fi
done
if [ ${#MISSING_PKGS[@]} -ne 0 ]; then
  echo "ERROR: The following required packages are missing: ${MISSING_PKGS[*]}" >&2
  echo "Please install them first using: apt-get update && apt-get install -y ${MISSING_PKGS[*]}" >&2
  exit 1
else
  echo "All required packages are installed."
fi

echo "Network settings in /etc/systemd/network"
mkdir -p /etc/systemd/network
echo "20-wan.network"
cat << 'EOF' > /etc/systemd/network/20-wan.network
[Match]
Name=eth0

[Network]
DHCP=yes
IPv6AcceptRA=no
LLMNR=no
MulticastDNS=no
EOF
echo "10-br0.netdev"
cat << 'EOF' > /etc/systemd/network/10-br0.netdev
[NetDev]
Name=br0
Kind=bridge
EOF
echo "15-eth1-to-bridge.network"
cat << 'EOF' > /etc/systemd/network/15-eth1-to-bridge.network
[Match]
Name=eth1

[Network]
Bridge=br0
EOF
echo "25-br0.network"
cat << EOF > /etc/systemd/network/25-br0.network
[Match]
Name=br0

[Network]
Address=${LAN_IP_WITH_MASK}
DHCPServer=yes
IPv6SendRA=no

[DHCPServer]
PoolOffset=${DHCP_START}
PoolSize=${DHCP_SIZE}
DNS=${LAN_IP}
Router=${LAN_IP}
EmitDNS=yes
EmitRouter=yes
EOF

echo "DNS settings in /etc/systemd/resolved.conf.d/dot.conf"
mkdir -p /etc/systemd/resolved.conf.d
cat << EOF > /etc/systemd/resolved.conf.d/dot.conf
[Resolve]
DNS=8.8.8.8#dns.google 8.8.4.4#dns.google
FallbackDNS=1.1.1.1#cloudflare-dns.com 1.0.0.1#cloudflare-dns.com 9.9.9.9#dns.quad9.net
DNSOverTLS=yes
LLMNR=no
MulticastDNS=no
DNSStubListenerExtra=${LAN_IP}
EOF
ln -sf /run/systemd/resolve/stub-resolv.conf /etc/resolv.conf

echo "Setting /etc/nftables.conf"
# TODO !!!
#https://wiki.nftables.org/wiki-nftables/index.php/Mangling_packet_headers
#nft add rule ip filter forward tcp flags syn tcp option maxseg size set rt mtu
cat << 'EOF' > /etc/nftables.conf
#!/usr/sbin/nft -f

flush ruleset

table ip filter {
    chain input {
        type filter hook input priority filter; policy drop;
        ct state established,related accept
        ct state invalid drop
        iifname "lo" accept
        iifname "br0" accept
        icmp type echo-request limit rate 5/second accept
    }
    chain forward {
        type filter hook forward priority filter; policy drop;
        ct state established,related accept
        ct state invalid drop
        iifname "br0" oifname "eth0" accept
    }
    chain output {
        type filter hook output priority filter; policy accept;
    }
}
table ip nat {
    chain postrouting {
        type nat hook postrouting priority srcnat; policy accept;
        oifname "eth0" masquerade
    }
}
EOF

echo "Wi-Fi AP setting via hostapd..."
mkdir -p /etc/hostapd
echo "2.4 GHz config in /etc/hostapd/hostapd_2g.conf"
cat << EOF > /etc/hostapd/hostapd_2g.conf
interface=wlan0
bridge=br0
driver=nl80211
ssid=${WIFI_SSID_2G}
hw_mode=g
channel=7
ieee80211n=1
wpa=2
wpa_passphrase=${WIFI_PASS_2G}
wpa_key_mgmt=WPA-PSK
wpa_pairwise=CCMP
rsn_pairwise=CCMP
ht_capab=[HT40+][SHORT-GI-20][SHORT-GI-40]
EOF
echo "5 GHz config in /etc/hostapd/hostapd_5g.conf"
cat << EOF > /etc/hostapd/hostapd_5g.conf
interface=wlan0
bridge=br0
driver=nl80211
ssid=${WIFI_SSID_5G}
country_code=${WIFI_COUNTRY_CODE_5G}
ieee80211d=1
hw_mode=a
channel=36
ieee80211n=1
ieee80211ac=1
vht_oper_chwidth=1
vht_oper_centr_ch_freqa=42
wpa=2
wpa_passphrase=${WIFI_PASS_5G}
wpa_key_mgmt=WPA-PSK
wpa_pairwise=CCMP
rsn_pairwise=CCMP
EOF
echo "Real config is /etc/hostapd/hostapd.conf and it is symlink to 2.4 GHz by default."
echo "You can easily change symlink to 5 GHz, don't forget systemctl restart hostapd."
ln -sf /etc/hostapd/hostapd_2g.conf /etc/hostapd/hostapd.conf
systemctl unmask hostapd || echo "INFO: hostapd is already unmasked."

echo "Enabling ipv4 forwarding, disabling ipv6 (persistent settings in /etc/sysctl.d/99-ipv4forward-ipv6disable.conf)"
cat << 'EOF' > /etc/sysctl.d/99-ipv4forward-ipv6disable.conf
net.ipv4.ip_forward=1
net.ipv6.conf.all.disable_ipv6=1
net.ipv6.conf.default.disable_ipv6=1
net.ipv6.conf.lo.disable_ipv6=1
EOF
sysctl --system || echo "WARNING: need reboot to enable sysctl settings!"

echo "Stopping NetworkManager..."
systemctl stop NetworkManager || echo "Already stopped!"
systemctl mask NetworkManager || echo "Already masked!"
echo "Stopping networking..."
systemctl stop networking || echo "Already stopped!"
systemctl disable networking || echo "Already disabled!"

echo "Starting nftables, systemd (networkd and resolved) and hostapd..."
systemctl enable --now nftables systemd-networkd systemd-resolved hostapd

echo "Script is finished!"
