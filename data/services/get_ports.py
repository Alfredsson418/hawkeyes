from time import sleep

f_output_udp = "udp.txt"
f_output_tcp = "tcp.txt"

with open("/etc/services", "r") as file, open(f_output_tcp, "w") as tcp_file, open(f_output_udp, "w") as udp_file:
    for line in file:
        # Skippes comments, new lines and empty space
        if (line[0] == '#' or line[0] == ' ' or line[0] == '\n'):
            continue;

        # Removes the many spaces
        result = " ".join(line.split())

        # Splits into array
        result = result.split(" ")

        port, protocol = result[1].split("/")
        service = result[0]

        print(port)
        match protocol:
            case "tcp":
                tcp_file.write(port+ "," + service + "\n")
            case "udp":
                udp_file.write(port + "," + service + "\n")
