<h1>Help and Usage</h1>
<div align="center">
  <table>
    <tr>
      <th>Usage</th>
      <th>./hawk [options]</th>
    </tr>
  </table>
  <table>
    <tr>
      <th>Get all options</th>
      <th>./hawk --help / hawk -?</th>
    </tr>
    <tr>
      <th>Get the program usage</th>
      <th>./hawk --usage</th>
    </tr>
  </table>
</div>

<h1>Options</h1>
<table>
  <tr>
    <th>Options</th>
    <th>Short Option</th>
    <th>Long Option</th>
    <th>Description</th>
    <th>Example</th>
    <th>Default Value</th>
    <th>Possible Values</th>
  </tr>
  <tr>
    <th>Target</th>
    <td>-t IP</td>
    <td>--target IP</td>
    <td>The target specified with IPv4 or IPv6</td>
    <td>-t 127.0.0.1</td>
    <td>Required</td>
    <td>IPv4 or IPv6 Address</td>
  </tr>
  <tr>
    <th>Port</th>
    <td>-p PORTS</td>
    <td>--port PORTS</td>
    <td>Specify one or more ports to scan</td>
    <td>-p 1-22,56,100</td>
    <td>1-1000</td>
    <td>Any positive integer between 0 and 65535</td>
  </tr>
  <tr>
    <th>Method</th>
    <td>-m METHOD</td>
    <td>--method METHOD</td>
    <td>Specify that scanning method to use</td>
    <td>-m conn</td>
    <td>Required</td>
    <td><a href="#methods">Methods</a></td>
  </tr>
  <tr>
    <th>Interface</th>
    <td>-i INTER</td>
    <td>--interface INTER</td>
    <td>Specify the network interface to scan</td>
    <td>-i lo</td>
    <td>None (Program will guess)</td>
    <td>Any network interface (Max 16 Chars long) </td>
  </tr>
  <tr>
    <th>No Ping</th>
    <td>None</td>
    <td>--no-ping</td>
    <td>Will not ping the target before a scan. Pinging the target only happens then running the program as root.</td>
    <td>--no-ping</td>
    <td>None</td>
    <td>None</td>
  </tr>
  <tr>
    <th>Verbose</th>
    <td></td>
    <td>--no-ping</td>
    <td>Will not ping the target before a scan. Pinging the target only happens then running the program as root.</td>
    <td>--no-ping</td>
    <td>None</td>
    <td>None</td>
  </tr>
  <tr>
    <th>Workers</th>
    <td>-n WORKERS</td>
    <td>--threading-workers WORKERS</td>
    <td>The amount of parallel scans</td>
    <td>-n 5</td>
    <td>3</td>
    <td>Any positive integer between 0 and MAX_INT.</td>
  </tr>
  <tr>
    <th>Timeout</th>
    <td>-w TIME</td>
    <td>--timeout TIME</td>
    <td>This is the time (in SECONDS) before a port is declared as closed.</td>
    <td>-w 10</td>
    <td>3</td>
    <td>Any positive integer between 0 and MAX_INT</td>
  </tr>
</table>


<h2 id="methods">Methods</h2>

<p>

`conn` or connect() is the regular way to connect to a TCP service though a 3 way handshake.</p>

<p>

`icmp` or ICMP Echo reply is used for UDP port scanning. This method utilized the fact that some firewalls reply with "Port Unreachable" or "Destination Unreachable" for UDP ports that are not open.
This works mostly on older firewalls because modern firewalls will not responde at all for this very reason.
</p>

<p>

`syn` or SYN Scan utilizes that in a TCP 3 way handshake, the target will respond with a TCP packet with the SYN flag set to 1 if a connection is possible, but you will not respond with an ACK, possibly making this scanning method more "sneaky". Older firewalls used to ignore non established connection, but modern firewalls will log these types of scans.
</p>
