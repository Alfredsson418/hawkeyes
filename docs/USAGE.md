<h1>Help and Usage</h1>
<div>
  <table>
    <tr>
      <th></th>
      <th>Help or Usage</th>
      <th></th>
    </tr>
    <tr>
      <th>./hawk --help </th>
      <th>./hawk -? </th>
      <th>./hawk --usage </th>
    </tr>
  </table>
</div>

<h1>Options</h1>
<h2>Target</h2>
<table>
    <tr>
      <th>Short Option</th>
      <th>Long Option</th>
      <th>Description</th>
      <th>Default Value</th>
      <th>Possible values</th>
    </tr>
    <tr>
      <td>-t IP</td>
      <td>--target IP</td>
      <td>The target specified with IPv4, IPv6 or a domain name. Default behavior for domains goes as IPv4 as default, and IPv6 as a backup if a IPv4 adress is not available. You can also specify specific adress family by using "example.com<b>:4</b>" for IPv4 or "example.com<b>:6</b>"</td>
      <td>No default value, must be specified</td>
      <td>Any IPv4 and IPv6 adresses, or any domain name</td>
    </tr>
</table>


<h2>Ports</h2>
<table>
    <tr>
      <th>Short Option</th>
      <th>Long Option</th>
      <th>Description</th>
      <th>Default Value</th>
      <th>Possible values</th>
    </tr>
    <tr>
      <td>-p PORTS</td>
      <td>--port PORTS</td>
      <td>Specify one or more ports to scan</td>
      <td>1-1000</td>
      <td>Any positive integer between 0 and 65535</td>
    </tr>
</table>


<h2>Methods</h2>
<table>
    <tr>
      <th>Short Option</th>
      <th>Long Option</th>
      <th>Description</th>
      <th>Default Value</th>
      <th>Possible values</th>
    </tr>
    <tr>
    <td>-m METHOD</td>
    <td>--method METHOD</td>
    <td>Specify that scanning method to use</td>
    <td>Required</td>
    <td><a href="#methods">Methods</a></td>
    </tr>
</table>

<h3 id="methods">Supported Methods</h3>
<p>

`conn` or connect() is the regular way to connect to a TCP service though a 3 way handshake.
</p>

<p>

`icmp` or ICMP Echo reply is used for UDP port scanning. This method utilized the fact that some firewalls reply with "Port Unreachable" or "Destination Unreachable" for UDP ports that are not open.
This works mostly on older firewalls because modern firewalls will not responde at all for this very reason.
</p>

<p>

`syn` or SYN Scan utilizes that in a TCP 3 way handshake, the target will respond with a TCP packet with the SYN flag set to 1 if a connection is possible, but you will not respond with an ACK, possibly making this scanning method more "sneaky". Older firewalls used to ignore non established connection, but modern firewalls will log these types of scans.
</p>


<h2>Network interfaces</h2>
<table>
    <tr>
      <th>Short Option</th>
      <th>Long Option</th>
      <th>Description</th>
      <th>Default Value</th>
      <th>Possible values</th>
    </tr>
    <tr>
      <td>-i INTER</td>
      <td>--interface INTER</td>
      <td>Specify the network interface to scan</td>
      <td>None (Program will guess)</td>
      <td>Any network interface (Max 16 Chars long) </td>
    </tr>
</table>


<h2>Verbose</h2>
<table>
    <tr>
      <th>Short Option</th>
      <th>Long Option</th>
      <th>Description</th>
      <th>Default Value</th>
      <th>Possible values</th>
    </tr>
    <tr>
      <th>Verbose</th>
      <td></td>
      <td>--no-ping</td>
      <td>Will not ping the target before a scan. Pinging the target only happens then running the program as root.</td>
      <td>None</td>
      <td>None</td>
    </tr>
</table>


<h2>Timeout</h2>
<table>
    <tr>
      <th>Short Option</th>
      <th>Long Option</th>
      <th>Description</th>
      <th>Default Value</th>
      <th>Possible values</th>
    </tr>
    <tr>
      <td>-w TIME</td>
      <td>--timeout TIME</td>
      <td>This is the time (in SECONDS) before a port is declared as closed.</td>
      <td>3</td>
      <td>Any positive integer between 0 and MAX_INT</td>
    </tr>
</table>


<h2>Timeout</h2>
<table>
    <tr>
      <th>Short Option</th>
      <th>Long Option</th>
      <th>Description</th>
      <th>Default Value</th>
      <th>Possible values</th>
    </tr>
    <tr>
      <td>-w TIME</td>
      <td>--timeout TIME</td>
      <td>This is the time (in SECONDS) before a port is declared as closed.</td>
      <td>3 seconds</td>
      <td>Any positive integer between 0 and MAX_INT</td>
    </tr>
</table>


<h2>Workers</h2>
<table>
    <tr>
      <th>Short Option</th>
      <th>Long Option</th>
      <th>Description</th>
      <th>Default Value</th>
      <th>Possible values</th>
    </tr>
    <tr>
      <td>-n WORKERS</td>
      <td>--threading-workers WORKERS</td>
      <td>The amount of parallel scans</td>
      <td>3 threads</td>
      <td>Any positive integer between 0 and MAX_INT.</td>
    </tr>
</table>


<h2>No ping</h2>
<table>
    <tr>
      <th>Short Option</th>
      <th>Long Option</th>
      <th>Description</th>
      <th>Default Value</th>
      <th>Possible values</th>
    </tr>
    <tr>
      <td>None</td>
      <td>--no-ping</td>
      <td>Will not ping the target before a scan. Pinging the target only happens then running the program as root.</td>
      <td>None</td>
      <td>None</td>
    </tr>
</table>


<h1>Examples</h1>
<table>
  <tr>
    <td>Description</td>
    <td>Usage</td>
  </tr>
  <tr>
    <td>Target localhost with an IPv4 adress and scan ports 50 to 100 using the "conn" method</td>
    <td>hawk -t 127.0.0.1 -m conn -p 50-100</td>
  </tr>
  <tr>
    <td>Target localhost with an IPv6 adress and scan ports 50 to 100 using the "conn" method</td>
    <td>hawk -t ::1 -m conn -p 50-100</td>
  </tr>
  <tr>
    <td>Target localhost and fetch the IPv6 adress and scan ports 50 to 100 using the "conn" method</td>
    <td>hawk -t localhost:6 -m conn -p 50-100</td>
  </tr>
</table>
