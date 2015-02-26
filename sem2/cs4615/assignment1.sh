IP_ADDRESS=$(hostname --ip-address)

#Allow ybind/NIS connections

sudo /sbin/iptables -A INPUT -s 143.239.75.234 -j ACCEPT
sudo /sbin/iptables -A OUTPUT -d 143.239.75.234 -j ACCEPT

sudo /sbin/iptables -A INPUT -s 143.239.75.212 -j ACCEPT
sudo /sbin/iptables -A OUTPUT -d 143.239.75.212 -j ACCEPT

sudo /sbin/iptables -A INPUT -i lo -j ACCEPT
sudo /sbin/iptables -A OUTPUT -o lo -j ACCEPT

#--Default Policy

#Loopback 
sudo /sbin/iptables -A INPUT  -i lo -j ACCEPT 
sudo /sbin/iptables -A OUTPUT -o lo -j ACCEPT

#Established connections
sudo /sbin/iptables -A INPUT -i eth6 -p tcp -m state --state ESTABLISHED -j ACCEPT
sudo /sbin/iptables -A INPUT -i eth6 -p udp -m state --state ESTABLISHED -j ACCEPT
 
sudo /sbin/iptables -A OUTPUT -o eth6 -p tcp -m state --state NEW,ESTABLISHED -j ACCEPT
sudo /sbin/iptables -A OUTPUT -o eth6 -p udp -m state --state NEW,ESTABLISHED -j ACCEPT

#--ICMP Traffic
sudo /sbin/iptables -A OUTPUT -o eth6 -p icmp -j ACCEPT
sudo /sbin/iptables -A INPUT -i eth6 -p icmp -m limit --limit 3/minute -j ACCEPT
sudo /sbin/iptables -A INPUT -i eth6 -p icmp -j DROP

#--SSH
sudo /sbin/iptables -A INPUT -p tcp --dport 22 -i eth6 -s 143.239.72.103 -d $IP_ADDRESS  -m time --timestart 17:00 --timestop 17:15 -j ACCEPT

sudo /sbin/iptables -A OUTPUT -p tcp --dport 22 -o eth6 -j ACCEPT


sudo /sbin/iptables -P INPUT DROP
sudo /sbin/iptables -P OUTPUT DROP
sudo /sbin/iptables -P FORWARD DROP
